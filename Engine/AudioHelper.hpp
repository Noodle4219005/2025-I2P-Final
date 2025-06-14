#ifndef AUDIOHELPER_HPP
#define AUDIOHELPER_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include <string>

class AudioHelper {
private:
    AudioHelper() = default;

public:
    static double BGMVolume;
    static double SFXVolume;
    static ALLEGRO_SAMPLE_ID PlayAudio(const std::string &audio);
    static ALLEGRO_SAMPLE_ID PlayAudio(const std::string &audio, const double& volumeScale);
    static ALLEGRO_SAMPLE_ID PlayBGM(const std::string &audio);
    static void StopBGM(ALLEGRO_SAMPLE_ID sample);
    static std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> PlaySample(const std::string &audio, bool loop = false, double volume = 1, double position = 0);
    static void StopSample(std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> sample_instance);
    static void ChangeSampleVolume(std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> sample_instance, double volume);
    static void ChangeSamplePosition(std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> sample_instance, double position);
    static void ChangeSampleSpeed(std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> sample_instance, double speed);
    static unsigned int GetSampleLength(std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> sample_instance);
    static double GetSamplePosition(std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> sample_instance);
};
#endif   // AUDIOHELPER_HPP
