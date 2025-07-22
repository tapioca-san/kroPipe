#include "kroPipe_audio.hpp"

namespace KP {
namespace UTILS {

void Audio::Init() {
    if (!Device) {
        Device = alcOpenDevice(nullptr);
        if (!Device) throw std::runtime_error("Core::Audio::Init(): Device was not created!");
    }

    if (!Context && Device) {
        Context = alcCreateContext(Device, nullptr);
        if (!Context) throw std::runtime_error("Core::Audio::Init(): Context was not created!");
        alcMakeContextCurrent(Context);
    }

    EAXSupport = alIsExtensionPresent("EAX2.0");

    if (AudioNames.empty()) {
        AudioNames.reserve(2);
        AudioNames.push_back("Anthrax_Mastered.wav");
        AudioNames.push_back("astral-creepy.wav");
    }
}

int32_t convert_to_int(char* buffer, size_t len) {
    int32_t a = 0;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    std::memcpy(&a, buffer, len);
#else
    for (std::size_t i = 0; i < len; ++i)
        reinterpret_cast<char*>(&a)[3 - i] = buffer[i];
#endif
    return a;
}

bool Audio::LoadWAV(const std::string& name, std::uint8_t& channels, std::int32_t& sample_rate,
                    std::uint8_t& bits_per_sample, std::vector<char>& sound_data) 
{
    std::ifstream file(name, std::ios::binary);
    if (!file.is_open()) return false;

    char buffer[4];

    if (!file.read(buffer, 4) || std::strncmp(buffer, "RIFF", 4) != 0) return false;

    if (!file.read(buffer, 4)) return false; // size ignored

    if (!file.read(buffer, 4) || std::strncmp(buffer, "WAVE", 4) != 0) return false;

    if (!file.read(buffer, 4)) return false; // fmt chunk
    if (!file.read(buffer, 4)) return false; // subchunk size

    if (!file.read(buffer, 2)) return false; // audio format

    if (!file.read(buffer, 2)) return false;
    channels = convert_to_int(buffer, 2);

    if (!file.read(buffer, 4)) return false;
    sample_rate = convert_to_int(buffer, 4);

    if (!file.read(buffer, 4)) return false; // byte rate
    if (!file.read(buffer, 2)) return false; // block align

    if (!file.read(buffer, 2)) return false;
    bits_per_sample = convert_to_int(buffer, 2);

    if (!file.read(buffer, 4) || std::strncmp(buffer, "data", 4) != 0) return false;

    if (!file.read(buffer, 4)) return false;
    ALsizei size = convert_to_int(buffer, 4);

    sound_data.resize(size);
    if (!file.read(sound_data.data(), size)) return false;

    return true;
}

void Audio::Load(const std::string& name) {
    if (State == AL_PLAYING) {
        Release();
        Init();
    }

    CurrentSound = name;

    std::uint8_t channels = 0;
    std::int32_t sample_rate = 0;
    std::uint8_t bits_per_sample = 0;
    std::vector<char> sound_data;

    bool res = LoadWAV(name, channels, sample_rate, bits_per_sample, sound_data);
    if (!res) throw std::runtime_error("Core::Audio::Load(): Failed to load WAV!");

    ALenum format;
    if (channels == 1 && bits_per_sample == 8)
        format = AL_FORMAT_MONO8;
    else if (channels == 1 && bits_per_sample == 16)
        format = AL_FORMAT_MONO16;
    else if (channels == 2 && bits_per_sample == 8)
        format = AL_FORMAT_STEREO8;
    else if (channels == 2 && bits_per_sample == 16)
        format = AL_FORMAT_STEREO16;
    else
        throw std::runtime_error("Core::Audio::Load(): Unknown file format!");

    alGenBuffers(1, &Buffer);
    if (alGetError() != AL_NO_ERROR) throw std::runtime_error("Core::Audio::Load(): alGenBuffers failed");

    alBufferData(Buffer, format, sound_data.data(), static_cast<ALsizei>(sound_data.size()), sample_rate);

    alGenSources(1, &Source);
    alSourcef(Source, AL_PITCH, 1.0f);
    alSourcef(Source, AL_GAIN, 1.0f);
    alSource3f(Source, AL_POSITION, 0, 0, 0);
    alSource3f(Source, AL_VELOCITY, 0, 0, 0);
    alSourcei(Source, AL_LOOPING, AL_FALSE);
    alSourcei(Source, AL_BUFFER, Buffer);
    alSourcePlay(Source);

    State = AL_PLAYING;
}

void Audio::SetState(bool state) {
    if (state) {
        if (State != AL_PLAYING) {
            alSourcePlay(Source);
            State = AL_PLAYING;
        }
    } else {
        alSourcePause(Source);
        State = AL_PAUSED;
    }
}

void Audio::Release() {
    alDeleteSources(1, &Source);
    alDeleteBuffers(1, &Buffer);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(Context);
    alcCloseDevice(Device);
    Context = nullptr;
    Device = nullptr;
    State = AL_STOPPED;
}

void Audio::Play() {
    ALint currentState;
    alGetSourcei(Source, AL_SOURCE_STATE, &currentState);

    if (currentState != AL_PLAYING) {
        alSourcePlay(Source);
        State = AL_PLAYING;
    }
}

} // namespace UTILS
} // namespace KP
