#include "Point.h"
class Edge {
public:
    Point begin = Point(0, 0);
    Point end = Point(0, 0);
    float dx;
    float scan_x;
    Edge(Point p1, Point p2);
};