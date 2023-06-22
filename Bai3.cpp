

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <regex>
#include "Point.cpp"
#include "Shape.cpp"
#include "TimeExpandedNode.cpp"
#include <assert.h>

using namespace std;

TimeExpandedNode *isAvailable(vector<vector<TimeExpandedNode *>> graph,
                              Point *origin, double time)
{
    for (vector<TimeExpandedNode *> it : graph)
    {
        for (TimeExpandedNode *TENode : it)
        {
            if (TENode->origin->equals(origin) && TENode->time == time)
            {
                return TENode;
            }
        }
    }
    return nullptr;
}

bool isAvailable(vector<vector<TimeExpandedNode *>> graph, double time, int *index)
{
    int count = 0;
    for (vector<TimeExpandedNode *> it : graph)
    {
        for (TimeExpandedNode *TENode : it)
        {
            if (TENode->time != time)
            {
                *index = count;
                return false;
            }
        }
        count++;
    }
    *index = 0;
    return true;
}

void insert(vector<vector<TimeExpandedNode *>> &graph, TimeExpandedNode *ten)
{
    for (vector<TimeExpandedNode *> it : graph)
    {
        if (it.at(0)->time == ten->time)
        {
            it.push_back(ten);
            return;
        }
    }
    vector<TimeExpandedNode *> tempTEN;
    tempTEN.push_back(ten);
    graph.push_back(tempTEN);
}

void sortGraph(vector<vector<TimeExpandedNode *>> &graph)
{
    sort(graph.begin(), graph.end(),
         [](vector<TimeExpandedNode *> a, vector<TimeExpandedNode *> b)
         {
             return a.at(0)->time < b.at(0)->time;
         });
}

void spread(vector<vector<TimeExpandedNode *>> &graph, int m, int n, double H)
{
    // int count = 0;
    TimeExpandedNode *node = graph.at(m).at(n);
    // node->origin->printPoint();
    queue<TimeExpandedNode *> Q;
    Q.push(node);

    while (!Q.empty())
    {
        TimeExpandedNode *temp = Q.front();
        Q.pop();
        // cout << temp->origin->x << " - " << temp->origin->y << endl;
        for (pair<TimeExpandedNode *, Shape *> pair : temp->tgts)
        {
            Shape *s = pair.second;
            double time = temp->time + s->getTime();
            if (time < H)
            {
                // count++;
                Point *origin = s->end;
                TimeExpandedNode *TENNode = pair.first;
                TimeExpandedNode *foundItem = isAvailable(graph, origin, time);
                if (foundItem == nullptr)
                {
                    TimeExpandedNode *newNode =
                        new TimeExpandedNode(origin, time, TENNode->tgts);
                    pair.first = newNode;
                    foundItem = newNode;
                }
                int index = foundItem->indexInSources(s);
                if (index != -1)
                {
                    foundItem->srcs.at(index).first = temp;
                }
                else
                {
                    foundItem->srcs.push_back(make_pair(temp, s));
                }

                insert(graph, foundItem); // gọi hàm đã làm ở câu (d)
                Q.push(foundItem);
            }
            // cout << "Spread_1" << endl;
        }
        // cout << "Spread_2" << endl;
    }
    // cout << "count: " << count << endl;
    // cout << "Spread_3" << endl;
}

vector<pair<int, int>> filter(vector<vector<TimeExpandedNode *>> graph)
{
    vector<pair<int, int>> filters;
    int count = 0;
    for (TimeExpandedNode *TENode : graph.at(0))
    {
        for (auto pair : TENode->srcs)
        {
            if (pair.first->time + pair.second->getTime() > TENode->time)
            {
                filters.push_back(make_pair(0, count));
                break;
            }
        }
        count++;
    }
    return filters;
}

void remove(vector<pair<int, int>> filters,
            vector<vector<TimeExpandedNode *>> &graph)
{
    int length = filters.size();
    for (int i = length - 1; i >= 0; i--)
    {
        pair<int, int> p = filters.at(i);
        // cout << graph.at(0).at(p.second)->origin->x << " - "
        //      << graph.at(0).at(p.second)->origin->y << " --- " << p.second << endl;

        graph.at(0).erase(graph.at(0).begin() + p.second);
    }
}

void assertTime(vector<vector<TimeExpandedNode *>> graph)
{
    for (vector<TimeExpandedNode *> it : graph)
    {
        for (TimeExpandedNode *TENode : it)
        {
            if (!TENode->srcs.empty())
            {
                for (auto elem : TENode->srcs)
                {
                    assert(elem.first->time + elem.second->getTime() == TENode->time);
                }
            }
        }
    }
}

vector<int> getStartedNodes(vector<vector<TimeExpandedNode *>> graph)
{
    int count = 0;
    vector<int> result;

    for (TimeExpandedNode *TENode : graph.at(0))
    {
        if (TENode->time == 0 && TENode->srcs.empty())
        {
            result.push_back(count);
        }
        count++;
    }

    return result;
}

// Hàm getCoincedence như sau
vector<pair<int, int>> getConcidences(vector<pair<TimeExpandedNode *, Shape *>> *srcs,
                                      vector<pair<TimeExpandedNode *, Shape *>> *tgts)
{
    vector<pair<int, int>> result;
    for (int i = 0; i < srcs->size(); i++)
    {
        for (int j = 0; j < tgts->size(); j++)
        {
            if (srcs->at(i).second->equals(tgts->at(j).second))
            {
                result.push_back(make_pair(i, j));
            }
        }
    }
    return result;
}

// Hàm dùng Regular Expression để bắt chuỗi
string strSmatch(string str, string W_regex)
{
    regex word_regex(W_regex);
    smatch match;
    regex_search(str, match, word_regex);
    return match.str();
}

// Hàm kiểm tra xem điểm point có trong vector P hay không
bool checkPoint(Point *point, vector<Point *> P)
{
    if (P.empty())
        return false;
    for (Point *p : P)
    {
        if (point->equals(p))
            return true;
    }
    return false;
}

// Hàm kiểm tra xem shape có trong vector S hay không
bool checkShape(Shape *shape, vector<Shape *> S)
{
    if (S.empty())
        return false;
    for (Shape *s : S)
    {
        if (shape->equals(s))
            return true;
    }
    return false;
}

int main()
{
    vector<Point *> P;
    vector<Shape *> S;
    vector<vector<TimeExpandedNode *>> allTENs;
    vector<TimeExpandedNode *> tempTENs;
    allTENs.push_back(tempTENs);

    ifstream input("AllParts.txt");
    input.is_open();
    int counter = 0;

    while (!input.eof())
    {
        string str;
        getline(input, str);
        if (strSmatch(str, "[_A-Z0-9]+(?= )") == "")
            break;
        Shape *shape = new Shape();

        shape->setName(strSmatch(str, "[_A-Z0-9]+(?= )"));

        string point_1 = strSmatch(str, "[_][-0-9.,]+(?=_)");
        string point_2 = strSmatch(str, "[^_A-Z ][-0-9.,]+(?=$)");

        Point *firstPoint = new Point();
        firstPoint->setPoint(stof(strSmatch(point_1, "[^_ ][-0-9.,]+(?=,)")),
                             stof(strSmatch(point_1, "[^_,][-0-9.]+(?=$)")));
        Point *lastPoint = new Point();
        lastPoint->setPoint(stof(strSmatch(point_2, "[^_ ][-0-9.,]+(?=,)")),
                            stof(strSmatch(point_2, "[^_,][-0-9.]+(?=$)")));

        shape->setPoint(firstPoint, lastPoint);

        if (!checkPoint(firstPoint, P))
        {
            P.push_back(firstPoint);
            TimeExpandedNode *n1 = new TimeExpandedNode();
            n1->setTENode(firstPoint);
            allTENs.at(0).push_back(n1);
        }

        if (!checkPoint(lastPoint, P))
        {
            P.push_back(lastPoint);
            TimeExpandedNode *n2 = new TimeExpandedNode();
            n2->setTENode(lastPoint);
            allTENs.at(0).push_back(n2);
        }

        if (!checkShape(shape, S))
        {
            S.push_back(shape);
        }
    }

    for (TimeExpandedNode *n : allTENs.at(0))
    {
        for (Shape *s : S)
        {
            if (s->start->equals(n->origin))
            {
                n->insertTarget(s);
            }
            else if (s->end->equals(n->origin))
            {
                n->insertSource(s);
            }
        }
    }
    cout << "End of first nested-for loops" << endl;
    // cout << "allTENs.at(0).size( ) = " << allTENs.at(0).size() << endl;

    for (TimeExpandedNode *nA : allTENs.at(0))
    {
        for (TimeExpandedNode *nB : allTENs.at(0))
        {
            vector<pair<int, int>> nA_to_nB;
            vector<pair<int, int>> nB_to_nA;
            if (!nA->equals(nB))
            {
                counter++;
                // cout<<//"A("<<nA->origin->x<<", "<<nA->origin->y<<") to ("<<nB->origin->x<<", "<<nB->origin->y<<") "<<
                //         counter<<endl;
                if ( // counter == 105875 ||
                     // counter == 220553 //||
                    counter % (10 * 1000 * 1000) == 0
                    // counter == 67116
                )
                {
                    // cout<<"BDEUG"<<endl;
                    cout << counter << endl;
                }
                nB_to_nA = getConcidences(&(nA->srcs), &(nB->tgts));
                nA_to_nB = getConcidences(&(nB->srcs), &(nA->tgts));
                nA->insertSourcesAndTargets(nB, nB_to_nA, nA_to_nB);
            }
        }
    }
    cout << "End of second nested-for loop" << endl;

    vector<int> initializations = getStartedNodes(allTENs);
    cout << "End of 3 nested-for loop" << endl;

    for (auto index : initializations)
    {
        spread(allTENs, 0, index, 1000);
    }
    sortGraph(allTENs);
    cout << "End of 4 nested-for loop" << endl;

    vector<pair<int, int>> redundants = filter(allTENs);
    cout << "End of 5 nested-for loop" << endl;

    remove(redundants, allTENs);
    cout << "End of 6 nested-for loop" << endl;

    assertTime(allTENs);
    cout << "End of 7 nested-for loop" << endl;

    int count_TEN = 0;
    for (vector<TimeExpandedNode *> it : allTENs)
    {
        for (TimeExpandedNode *TENode : it)
        {
            count_TEN++;
        }
    }
    cout << "Number of TENode in allTENS: " << count_TEN << endl;
}