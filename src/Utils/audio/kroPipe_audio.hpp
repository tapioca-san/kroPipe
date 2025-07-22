#ifndef KROPIPE_AUDIO_H
#define KROPIPE_AUDIO_H

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace UTILS {


class Audio 
{
    public:
    Audio() {}
    ~Audio() { ALCboolean closed = alcCloseDevice(Device); }
    void Init();
    void Load(const std::string& name);
    void SetVolume(float volume) { Volume = volume; alSourcef(Source, AL_GAIN, volume); }
    float GetVolume() { return Volume; }
    void Play();
    const std::vector<std::string>& GetAudioNames() const { return AudioNames; }
    const std::string& GetCurrentSound() const { return CurrentSound; }
    void ResetState() { State = AL_STOPPED; }
    void SetState(bool state);//{ if (state) { State = AL_PLAYING; } else { State = AL_PAUSED; } }
    bool GetState() { return State == AL_PLAYING; }
    
    private:
    ALCdevice* Device = nullptr;
    ALCcontext* Context = nullptr;
    ALCboolean EAXSupport = false;
    ALuint Buffer;
    ALint State;
    ALuint Source;
    float Volume = 0.0f;
    std::string CurrentSound;
    std::vector<std::string> AudioNames;
    void Release();
    bool LoadWAV(const std::string& name, std::uint8_t& channels, std::int32_t& sample_rate, std::uint8_t& bits_per_sample, std::vector<char>& sound_data);
};

} // namespace UTILS
} // namespace KP

#endif//KROPIPE_AUDIO_H