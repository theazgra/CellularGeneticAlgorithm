#pragma once
struct Point
{
    int x;
    int y;
    bool isSet;

    Point()
    {
        isSet = false;
    }

    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
        isSet = true;
    }

    void set(int x, int y)
    {
        this->x = x;
        this->y = y;
        isSet = true;
    }
};