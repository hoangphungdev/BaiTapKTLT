#pragma once
#include <iostream>
#include "Point.cpp"
#include "Shape.cpp"
#include <vector>
#include <utility>
#include <queue>
using namespace std;

class TimeExpandedNode
{
public:
    vector<pair<TimeExpandedNode *, Shape *>> srcs;
    Point *origin;
    double time = 0;
    int layer;
    vector<pair<TimeExpandedNode *, Shape *>> tgts;

    TimeExpandedNode() {}
    TimeExpandedNode(Point *origin, double time,
                     vector<pair<TimeExpandedNode *, Shape *>> tgts)
    {
        this->origin = origin;
        this->time = time;
        for (auto it : tgts)
        {
            this->tgts.push_back(it);
        }
    }

    int indexInSources(Shape *s)
    {
        int index = 0;
        bool found = false;
        for (auto it : tgts)
        {
            Shape *shape = it.second;
            if (shape->equals(s))
            {
                found = true;
                return index;
            }
            index++;
        }
        if (!found)
            return -1;
        return index;
    }

    void setTENode(Point *point)
    {
        this->origin = point;
    }

    void insertSource(Shape *s)
    {
        // Nếu srcs có chứa một pair mà phần tử đầu của nó giống hệt s->start
        //     thì không thêm nullptr và s vào
        //         Ngược lại
        {
            pair<TimeExpandedNode *, Shape *> pair =
                make_pair(nullptr, s);
            this->srcs.push_back(pair);
        }
    }

    void insertTarget(Shape *s)
    {
        // Nếu tgts có chứa một pair mà phần tử đầu của nó giống hệt s->end
        //     thì không thêm cặp nullptr và s vào
        //         Ngược lại
        {
            pair<TimeExpandedNode *, Shape *> pair =
                make_pair(nullptr, s);
            this->tgts.push_back(pair);
        }
    }

    bool equals(TimeExpandedNode *n)
    {
        return this->origin->equals(n->origin);
    }

    void insertSourcesAndTargets(TimeExpandedNode *n, vector<pair<int, int>> fromN,
                                 vector<pair<int, int>> toN)
    {
        for (int i = 0; i < fromN.size(); i++)
        {
            int atN = fromN.at(i).first;
            int toThis = fromN.at(i).second;
            if (this->srcs.at(atN).first == nullptr)
            {
                this->srcs.at(atN).first = n;
            }
            if (n->tgts.at(toThis).first == nullptr)
            {
                n->tgts.at(toThis).first = this;
            }
        }

        for (int i = 0; i < toN.size(); i++)
        {
            int atN = toN.at(i).first;
            int fromThis = toN.at(i).second;
            if (this->tgts.at(fromThis).first == nullptr)
            {
                this->tgts.at(fromThis).first = n;
            }
            if (n->srcs.at(atN).first == nullptr)
            {
                n->srcs.at(atN).first = this;
            }
        }
    }
};
