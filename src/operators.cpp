#include "headers/engine.hpp"
#include "headers/sound_operators.hpp"

namespace ge
{
    vec3 operator*(const vec3& in, const matrix3& transform)
    {
        vec3 out;

        out.x = in.x * transform.data[0].x + in.y * transform.data[1].x +
                in.w * transform.data[2].x;

        out.y = in.x * transform.data[0].y + in.y * transform.data[1].y +
                in.w * transform.data[2].y;

        out.w = in.x * transform.data[0].w + in.y * transform.data[1].w +
                in.w * transform.data[2].w;

        return out;
    }

    vertex operator*(const vertex& in, const matrix3& transform)
    {
        vertex out;

        out.x = in.x * transform.data[0].x + in.y * transform.data[1].x +
                transform.data[2].x;

        out.y = in.x * transform.data[0].y + in.y * transform.data[1].y +
                transform.data[2].y;

        return out;
    }

    matrix3 operator*(const matrix3& mat1, const matrix3& mat2)
    {
        matrix3 res;
        for (size_t v_i = 0, max_i = 3; v_i < max_i; ++v_i)
        {
            res.data[v_i].x = mat1.data[v_i].x * mat2.data[0].x +
                              mat1.data[v_i].y * mat2.data[1].x +
                              mat1.data[v_i].w * mat2.data[2].x;

            res.data[v_i].y = mat1.data[v_i].x * mat2.data[0].y +
                              mat1.data[v_i].y * mat2.data[1].y +
                              mat1.data[v_i].w * mat2.data[2].y;

            res.data[v_i].w = mat1.data[v_i].x * mat2.data[0].w +
                              mat1.data[v_i].y * mat2.data[1].w +
                              mat1.data[v_i].w * mat2.data[2].w;
        }
        return res;
    }

    std::istream& operator>>(std::istream& is, texture& tx)
    {
        for (size_t i = 0; i < tx.tex_coords.size(); ++i)
        {
            is >> tx.coords[i];
            is >> tx.tex_coords[i];
        }
        return is;
    }

    std::istream& operator>>(std::istream& is, triangle& tr)
    {
        is >> tr.v[0];
        is >> tr.v[1];
        is >> tr.v[2];
        return is;
    }

    std::istream& operator>>(std::istream& is, vertex& v)
    {
        is >> v.x;
        is >> v.y;
        return is;
    }

    bool operator!=(const SDL_AudioSpec& frst, const SDL_AudioSpec& scnd)
    {
        return frst.freq != scnd.freq || frst.channels != scnd.channels ||
               frst.format != scnd.format;
    }
}
