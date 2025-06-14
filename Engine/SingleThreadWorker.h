#pragma once;

#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>

class SingleThreadWorker
{
public:
    SingleThreadWorker();
    ~SingleThreadWorker();
    SingleThreadWorker(const SingleThreadWorker&) = delete;
    SingleThreadWorker& operator=(const SingleThreadWorker&) = delete;
    bool Enqueue(std::function<void()> func);
private:
    std::thread m_worker;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::queue<std::function<void()>> m_tasks;
    bool m_isFinished=false;
    void ThreadLoop();
};