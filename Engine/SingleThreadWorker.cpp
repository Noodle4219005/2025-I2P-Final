#include "SingleThreadWorker.h"

#include <iostream>

SingleThreadWorker::SingleThreadWorker() : m_isFinished(false)
{
    m_worker=std::thread([this]() {this->ThreadLoop(); });
}

SingleThreadWorker::~SingleThreadWorker()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_isFinished=true;
    }
    m_cv.notify_all();
    if (m_worker.joinable()) m_worker.join();
    std::cout<<"I'm destructed\n";
}

bool SingleThreadWorker::Enqueue(std::function<void()> func)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_isFinished) return false;
    m_tasks.push(std::move(func));
    m_cv.notify_one();
    return true;
}

void SingleThreadWorker::ThreadLoop()
{
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this]() {return m_isFinished || m_tasks.size(); });
            if (m_isFinished && m_tasks.empty()) break;
            if (m_tasks.size()) {
                task=std::move(m_tasks.front());
                m_tasks.pop();
            }
        }

        try {
            task();
        } catch (_exception e) {
            break;
        }
    }

}