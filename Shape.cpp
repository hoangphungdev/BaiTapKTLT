#include <iostream>
#include "Point.cpp"
#include "Constant.cpp"
#include <math.h>

#pragma once
using namespace std;

class Shape
{
public:
    Point *start;
    Point *end;
    double d;
    string name;
    double time = 0;

    void setName(string name)
    {
        this->name = name;
    }

    void setPoint(Point *start, Point *end)
    {
        this->start = start;
        this->end = end;
        this->d = sqrt((start->x - end->x) * (start->x - end->x) + (start->y - end->y) * (start->y - end->y));
    }

    virtual double getTime()
    {
        if (time == 0)
        {
            Constant const_v;
            time = this->d / const_v.v; // Với v là hằng số của lớp Constant
        }
        return time;
    }

    bool equals(Shape *s)
    {
        return ((this->start)->equals(s->start) && (this->end)->equals(s->end));
    }
};
