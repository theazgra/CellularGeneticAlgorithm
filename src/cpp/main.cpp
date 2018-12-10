#include "image.cpp"

int main(int, char **)
{
    using namespace cimg_library;

    CImg<unsigned char> img = create_color_image(100, 500);
    img.fill(0);

    for (uint row = 0; row < img.height(); row++)
    {
        for (uint col = 0; col < img.width(); col++)
        {
            set_pixel(img, row, col, RgbPixel(125, 80, 186));
        }
    }

    img.save_png("Result.png");
    return 0;
}
