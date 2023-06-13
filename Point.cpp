#include <iostream>
#pragma once
using namespace std;

class Point
{
public:
    double x;
    double y;

    void setPoint(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    bool equals(Point *p)
    {
        return (this->x == p->x) && (this->y == p->y);
    }
};
