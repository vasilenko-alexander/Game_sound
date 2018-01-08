#pragma once

#include <SDL2/SDL_audio.h>

namespace ge
{
    bool operator!=(const SDL_AudioSpec& frst, const SDL_AudioSpec& scnd);
}
