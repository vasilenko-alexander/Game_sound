#pragma once

#include <SDL2/SDL.h>

namespace ge
{
    struct AudioDevice
    {
        SDL_AudioDeviceID id = 0;
        SDL_AudioSpec spec;
        AudioDevice()
        {
            SDL_zero(spec);
        }
    };

    struct AudioObject
    {
        uint8_t* data     = nullptr;
        uint32_t length   = 0;
        uint32_t data_pos = 0;
        float volume      = 1.0f;
        bool is_looped    = false;
        bool is_playing   = false;
        ~AudioObject()
        {
            SDL_FreeWAV(data);
        }
    };

    enum gsound_types
    {
        st_music,
        st_sound
    };

    enum gsound_state
    {
        ss_play,
        ss_pause
    };
}
