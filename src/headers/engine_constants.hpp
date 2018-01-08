#pragma once

#include <string>

#define DECL_STR_CONST(name) const std::string name = #name;

namespace ge
{
    /**
     * engine initialize constants
     */
    DECL_STR_CONST(video)
    DECL_STR_CONST(audio)
    DECL_STR_CONST(timer)
    DECL_STR_CONST(joystick)
    DECL_STR_CONST(gamecontroller)
    DECL_STR_CONST(everything)
    DECL_STR_CONST(events)
    DECL_STR_CONST(haptic)

    DECL_STR_CONST(background_music)
    DECL_STR_CONST(tank_move)
    DECL_STR_CONST(tank_shoot)
    DECL_STR_CONST(tank_explotion)
}

#undef DECL_STR_CONST
