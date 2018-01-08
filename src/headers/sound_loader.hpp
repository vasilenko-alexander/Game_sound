#pragma once

#include <SDL2/SDL_audio.h>
#include "sound_types.hpp"
#include <string>
#include <map>

namespace ge
{
    class Sound_loader
    {
        static const std::string config_path;
        AudioDevice device;
        std::map<std::string, AudioObject*> sounds;

    public:
        Sound_loader();
        ~Sound_loader();
        void init();
        void play(const std::string& sound_name);
        void stop(const std::string& sound_name);

    private:
        static void refill_audio(void* userdata, Uint8* stream, int len);
        void init_device();
        void load_sounds();
        void play_stop(const std::string& sound_name, bool playing);
    };
}
