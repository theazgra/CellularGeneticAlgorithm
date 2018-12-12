#define cimg_display 0
#include "CImg.h"

typedef unsigned char uchar;

struct RgbPixel
{
    uchar R;
    uchar G;
    uchar B;

    RgbPixel()
    {
        R = 0;
        G = 0;
        B = 0;
    }

    RgbPixel(uchar r, uchar g, uchar b)
    {
        R = r;
        G = g;
        B = b;
    }

    RgbPixel(uchar value)
    {
        R = value;
        G = value;
        B = value;
    }
};

inline void set_pixel(cimg_library::CImg<uchar> &img, const uint &row, const uint &col, const RgbPixel &rgb)
{
    img(col, row, 0) = rgb.R;
    img(col, row, 1) = rgb.G;
    img(col, row, 2) = rgb.B;
}

inline void set_pixel(cimg_library::CImg<uchar> &img, const uint &row, const uint &col, const uchar &px)
{
    img(col, row, 0) = px;
}

inline RgbPixel get_pixel(cimg_library::CImg<uchar> &img, const uint &row, const uint &col)
{
    RgbPixel rgb;
    rgb.R = img(col, row, 0);
    rgb.G = img(col, row, 1);
    rgb.B = img(col, row, 2);
    return rgb;
}

cimg_library::CImg<uchar> create_grayscale_image(const uint width, const uint height)
{
    cimg_library::CImg<uchar> image(width, height, 1, 1);
    return image;
}

cimg_library::CImg<uchar> create_color_image(const uint width, const uint height)
{
    cimg_library::CImg<uchar> image(width, height, 1, 3);
    return image;
}