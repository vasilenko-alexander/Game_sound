#include "headers/image_loader.hpp"
#include "headers/picopng.hxx"

namespace ge
{
    ImageLoader::ImageLoader()
    {
    }

    std::vector<unsigned char> ImageLoader::load_file(const std::string& path)
    {
        using namespace std;
        vector<unsigned char> buffer;

        ifstream file(path.c_str());

        if (!file.is_open())
        {
            cerr << "File " << path << " can't be opened" << endl;
            return buffer;
        }

        streamsize size = 0;

        file.seekg(0, ios::end);
        if (!file.good())
        {
            cerr << "Can't change pointer location to end" << endl;
            return buffer;
        }
        size = file.tellg();

        file.seekg(0, ios::beg);
        if (!file.good())
        {
            cerr << "Can't change pointer location to begin" << endl;
            return buffer;
        }
        size -= file.tellg();

        if (size > 0)
        {
            buffer.resize(size);
            file.read(reinterpret_cast<char*>(&buffer.front()), size);
        }

        return buffer;
    }

    std::vector<unsigned char> ImageLoader::load_image(const std::string& path,
                                                       unsigned long& width,
                                                       unsigned long& height)
    {
        std::vector<unsigned char> buffer = load_file(path);
        std::vector<unsigned char> image, reversed_image;

        int error =
            decodePNG(image, width, height, &buffer.front(), buffer.size());

        if (error != 0)
        {
            std::cerr << "Function decodePNG failed" << std::endl;
        }

        reversed_image = reverse_image(image, width, height);

        return reversed_image;
    }

    std::vector<unsigned char>
    ImageLoader::reverse_image(const std::vector<unsigned char>& img,
                               unsigned long width,
                               unsigned long height)
    {
        std::vector<unsigned char> reversed_image;

        int values_in_row = width * 4;
        reversed_image.reserve(img.size());
        for (int row = height - 1; row >= 0; --row)
        {
            for (int curr_val = 0; curr_val < values_in_row; ++curr_val)
            {
                reversed_image.push_back(img[curr_val + values_in_row * row]);
            }
        }

        return reversed_image;
    }
}
