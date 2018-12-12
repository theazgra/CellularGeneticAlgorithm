#pragma once
#include "point.h"
#include <assert.h>

typedef unsigned int uint;
typedef unsigned char uchar;

constexpr int UCHAR_MAX_AS_INT = 255;
constexpr double UCHAR_MAX = 255.0;
constexpr double MAX_FITNESS_VALUE = 3.0 * UCHAR_MAX;

struct Cell
{
    uchar R;
    uchar G;
    uchar B;

    bool isEmpty;
    Point cellLocation;
    Point cellToReplaceLocation;

    Cell()
    {
        isEmpty = true;
    }

    Cell(Point location)
    {
        isEmpty = false;
        cellLocation = location;
        R = 0;
        G = 0;
        B = 0;
    }

    Cell(Point location, const uchar r, const uchar g, const uchar b)
    {
        isEmpty = false;
        cellLocation = location;
        R = r;
        G = g;
        B = b;
    }

    double get_fitness() const
    {
        return ((double)R + (double)G + (double)B);
    }

    double get_objective() const
    {
        return (MAX_FITNESS_VALUE - get_fitness());
    }
};