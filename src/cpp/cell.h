typedef unsigned char uchar;
#define UCHAR_MAX 255

struct Cell
{
    uchar R;
    uchar G;
    uchar B;

    float get_fitness() const
    {
        return ((float)R + (float)G + (float)B);
    }

    float get_selection_probability() const
    {
        return get_fitness() / 3.0f * (float)UCHAR_MAX;
    }

    float get_target() const
    {
        return ((3.0f * (float)UCHAR_MAX) - get_fitness());
    }
};
