#include "headers/engine.hpp"
#include "headers/engine_constants.hpp"
#include <fstream>
#include "headers/sound_loader.hpp"

ge::matrix3 rotate_to(ge::keys direct)
{
    using namespace ge;
    rotate_direction r_dir = rotate_direction::clockwise;
    float angle            = 0.f;
    switch (direct)
    {
        case keys::right:
            angle = M_PI_2;
            break;
        case keys::left:
            angle = M_PI_2;
            r_dir = rotate_direction::counterclockwise;
            break;
        case keys::down:
            angle = M_PI;
            break;
        default:
            break;
    }
    return matrix3::rotation(angle, r_dir);
}

ge::matrix3 move_to(ge::keys direct)
{
    using namespace ge;
    static float dx = 0.f;
    static float dy = 0.f;
    switch (direct)
    {
        case keys::up:
            dy += 0.01f;
            break;
        case keys::down:
            dy -= 0.01f;
            break;
        case keys::left:
            dx -= 0.01f;
            break;
        case keys::right:
            dx += 0.01f;
            break;
        default:
            break;
    }
    return matrix3::transport(dx, dy);
}

int main(int /*argn*/, char* /*args*/ [])
{
    ge::IEngine* gameEngine = ge::getInstance();
    std::string errMsg      = gameEngine->init_engine(ge::everything);

    if (!errMsg.empty())
    {
        std::cerr << errMsg << std::endl;
        return EXIT_FAILURE;
    }

    const std::string text_path = "textures/tanks.png";
    gameEngine->draw_texture(text_path);

    ge::Sound_loader player;
    player.init();
    player.play(ge::background_music);

    bool run_loop = true;
    ge::event event;
    ge::matrix3 move   = ge::matrix3::indentity();
    ge::matrix3 rotate = ge::matrix3::indentity();

    while (run_loop)
    {
        while (gameEngine->read_event(event))
        {
            if (!event.msg.empty())
            {
                std::cout << event.msg << std::endl;
            }
            switch (event.type)
            {
                case ge::events_t::shutdown:
                    run_loop = false;
                    break;
                case ge::events_t::pressed:
                    move   = move_to(event.key);
                    rotate = rotate_to(event.key);
                    break;
                default:
                    break;
            }
        }
        const std::string vertexes_path = "config/texture_vertexes.txt";

        std::ifstream file(vertexes_path);
        if (!file.is_open())
        {
            std::cerr << "Can't open file with vertexes values" << std::endl;
            continue;
        }

        ge::texture txLeft, txRight;
        file >> txRight >> txLeft;

        ge::matrix3 scale     = ge::matrix3::scale(0.25);
        ge::matrix3 transform = scale * rotate * move;

        for (size_t v_i = 0, max_i = txRight.coords.size(); v_i < max_i; ++v_i)
        {
            txRight.coords[v_i] = txRight.coords[v_i] * transform;
            txLeft.coords[v_i]  = txLeft.coords[v_i] * transform;
        }

        gameEngine->render(txLeft);
        gameEngine->render(txRight);

        gameEngine->swap_buffers();
    }
    gameEngine->uninit_engine();
    return EXIT_SUCCESS;
}
