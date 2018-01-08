#pragma once

#include <vector>
#include <string>

namespace ge
{
    class ImageLoader
    {
    public:
        ImageLoader();

        std::vector<unsigned char> load_file(const std::string& path);
        std::vector<unsigned char> load_image(const std::string& path,
                                              unsigned long& width,
                                              unsigned long& height);
        std::vector<unsigned char>
        reverse_image(const std::vector<unsigned char>& img,
                      unsigned long width,
                      unsigned long height);
    };
}
