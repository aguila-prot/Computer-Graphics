#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include "Polygon.h"
using namespace std;
class WeilerAtherton {
private:
    vector<Point> enter_points;
    vector<Point> exit_points;
    vector<GLfloat> inters;
    vector<Polygon> subject;
    vector<Polygon> clipper;
    vector<Polygon> result;
    void change_orientation(Polygon *polygon);
    void intersection();
    bool is_point_in(Point point, Polygon &polygon);
    void insert(vector<Point> &points, GLfloat x, GLfloat y, Point inter);
    double EPS;
    int width, height;
    int find(GLfloat x, GLfloat y, vector<Point> &points);
public:
    WeilerAtherton(vector<Polygon> s, vector<Polygon> c, int width, int height);
    vector<Polygon> get_result();
};