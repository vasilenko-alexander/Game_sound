#include "headers/sound_loader.hpp"
#include "headers/sound_operators.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility>

namespace ge
{
    const std::string Sound_loader::config_path = "config/sound_config.txt";

    Sound_loader::Sound_loader()
    {
    }

    Sound_loader::~Sound_loader()
    {
        for (auto data : sounds)
            delete data.second;
    }

    void Sound_loader::init()
    {
        init_device();
        load_sounds();
    }

    void Sound_loader::init_device()
    {
        device.spec.freq     = 48000;
        device.spec.channels = 2;
        device.spec.format   = AUDIO_S16;
        device.spec.samples  = 1024;
        device.spec.callback = refill_audio;
        device.spec.userdata = this;

        device.id = SDL_OpenAudioDevice(
            nullptr, 0, &device.spec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);

        if (device.id == 0)
        {
            std::cerr << "Can't open audio device: " << SDL_GetError()
                      << std::endl;
            return;
        }

        SDL_PauseAudioDevice(device.id, ss_play);
    }

    void Sound_loader::play(const std::string& sound_name)
    {
        play_stop(sound_name, true);
    }

    void Sound_loader::stop(const std::string& sound_name)
    {
        play_stop(sound_name, false);
    }

    void Sound_loader::play_stop(const std::string& sound_name, bool playing)
    {
        SDL_LockAudioDevice(device.id);

        try
        {
            AudioObject* audio = sounds.at(sound_name);
            audio->data_pos    = 0;
            audio->is_playing  = playing;
        }
        catch (const std::exception& ex)
        {
            std::clog << "Can't find " << sound_name
                      << " music/sound:" << ex.what() << std::endl;
        }

        SDL_UnlockAudioDevice(device.id);
    }

    void
    Sound_loader::refill_audio(void* userdata, Uint8* stream, int stream_len)
    {
        Sound_loader* s_loader = static_cast<Sound_loader*>(userdata);

        if (s_loader == nullptr)
            return;

        SDL_memset(stream, 0, stream_len);

        for (auto& pair : s_loader->sounds)
        {
            AudioObject* sound = pair.second;
            if (sound->length == 0)
                continue;
            if (sound->is_playing)
            {
                uint32_t rest_len     = sound->length - sound->data_pos;
                uint32_t s_len        = static_cast<uint32_t>(stream_len);
                rest_len              = rest_len > s_len ? s_len : rest_len;
                uint8_t* audio_buffer = &sound->data[sound->data_pos];

                SDL_MixAudioFormat(stream,
                                   audio_buffer,
                                   s_loader->device.spec.format,
                                   rest_len,
                                   SDL_MIX_MAXVOLUME);

                sound->data_pos += rest_len;
                if (sound->data_pos == sound->length)
                {
                    sound->data_pos = 0;
                    if (!sound->is_looped)
                        sound->is_playing = false;
                }
            }
        }
    }

    void Sound_loader::load_sounds()
    {
        std::ifstream config_file(config_path.c_str());

        if (!config_file.is_open())
        {
            std::cerr << "Config file placed in " << config_path
                      << " can't be opened" << std::endl;
            return;
        }

        for (std::string line; std::getline(config_file, line);)
        {
            std::istringstream line_parser(line);
            std::string s_name;
            std::string s_path;
            int s_type = 0;
            line_parser >> s_name >> s_path >> s_type;
            if (line_parser.bad())
            {
                std::cerr << "Read config data failed" << std::endl;
                continue;
            }
            SDL_AudioSpec wav_spec;
            SDL_zero(wav_spec);
            Uint8* wav_data = nullptr;
            Uint32 wav_len  = 0;
            if (SDL_LoadWAV(s_path.c_str(), &wav_spec, &wav_data, &wav_len) ==
                nullptr)
            {
                std::cerr << "Can't open audio record placed in " << s_path
                          << " :" << SDL_GetError() << std::endl;
                continue;
            }

            sounds[s_name]            = new AudioObject();
            sounds[s_name]->is_looped = s_type == st_music;

            if (wav_spec != device.spec)
            {
                SDL_AudioCVT cvt;
                SDL_zero(cvt);
                SDL_BuildAudioCVT(&cvt,
                                  wav_spec.format,
                                  wav_spec.channels,
                                  wav_spec.freq,
                                  device.spec.format,
                                  device.spec.channels,
                                  device.spec.freq);
                SDL_assert(cvt.needed);
                cvt.len = wav_len;
                cvt.buf = new uint8_t[cvt.len * cvt.len_mult];
                std::copy_n(wav_data, wav_len, cvt.buf);
                if (SDL_ConvertAudio(&cvt) != 0)
                {
                    std::cerr
                        << "Conversion of music data failed:" << SDL_GetError()
                        << std::endl;
                    continue;
                }
                sounds[s_name]->data   = cvt.buf;
                sounds[s_name]->length = cvt.len_cvt;
                SDL_FreeWAV(wav_data);
            }
            else
            {
                sounds[s_name]->data   = wav_data;
                sounds[s_name]->length = wav_len;
            }
        }
    }
}
