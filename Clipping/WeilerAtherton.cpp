#include "WeilerAtherton.h"
#include <cmath>
#include <iostream>
WeilerAtherton::WeilerAtherton(vector<Polygon> sub, vector<Polygon> clip, int width, int height) {
    this->width = width;
    this->height = height;
    subject = sub;
    clipper = clip;
    for(int i=0; i<subject.size(); i++)
        change_orientation(&subject[i]);
    for(int i=0; i<clipper.size(); i++)
        change_orientation(&clipper[i]);
    intersection();
    bool in_polygon, in_hole;
    for(Polygon s : subject){
        if(!s.get_inter()){
            in_polygon = in_hole = false;
            for(Polygon c : clipper)
                if(c.get_type() != 3 && is_point_in(s.points[0], c))
                    in_polygon = true;
                else if(c.get_type() == 3 && is_point_in(s.points[0], c))
                    in_hole = true;
            if(in_polygon && !in_hole)
                result.emplace_back(s);
        }
    }
    for(Polygon c : clipper){
        if(!c.get_inter()){
            in_polygon = in_hole = false;
            for(Polygon s : subject)
                if(s.get_type() != 1 && is_point_in(c.points[0], s))
                    in_polygon = true;
                else if(s.get_type() == 1 && is_point_in(c.points[0], s))
                    in_hole = true;
            if(in_polygon && !in_hole)
                result.emplace_back(c);
        }
    }
    while(!enter_points.empty()){
        vector<Point> polygon;
        Point start = enter_points.back();
        Point point;
        enter_points.pop_back();
        polygon.emplace_back(start);
        Polygon s = subject[start.index_subject];
        int index = find(start.get_gl_x(), start.get_gl_y(), s.points);
        if(index < s.points.size() - 1)
            point = s.points[index+1];
        else
            point = s.points[0];
        while(!(point.get_gl_x() == start.get_gl_x() && point.get_gl_y() == start.get_gl_y())){
            index = find(point.get_gl_x(), point.get_gl_y(), s.points);
            point = s.points[index];
            while(point.exit == 0) {
                if (point.get_gl_x() == start.get_gl_x() and point.get_gl_y() == start.get_gl_y())
                    break;
                polygon.emplace_back(point);
                index += 1;
                if(index < s.points.size())
                    point = s.points[index];
                else{
                    point = s.points[0];
                    index = 0;
                }
            }
            Polygon c = clipper[point.index_clipper];
            index = find(point.get_gl_x(), point.get_gl_y(), c.points);
            point = c.points[index];
            while (point.enter == 0) {
                if (point.get_gl_x() == start.get_gl_x() && point.get_gl_y() == start.get_gl_y())
                    break;
                polygon.emplace_back(point);
                index += 1;
                if (index < c.points.size())
                    point = c.points[index];
                else{
                    point = c.points[0];
                    index = 0;
                }
            }
            s = subject[point.index_subject];
        }
        Polygon poly(0, 0);
        poly.points = polygon;
        result.emplace_back(poly);
    }
}
void WeilerAtherton::intersection() {
    vector<Polygon> subject_new;
    vector<Polygon> clipper_new;
    clipper_new = clipper;
    for(Polygon s : subject){
        subject_new.emplace_back(s);
        for(int i=0; i<s.points.size(); i++){
            int next1 = (i == s.points.size() - 1) ? 0 : i+1;
            GLfloat a1 = s.points[i].get_gl_y() - s.points[next1].get_gl_y();
            GLfloat b1 = s.points[next1].get_gl_x() - s.points[i].get_gl_x();
            GLfloat c1 = s.points[i].get_gl_x() * s.points[next1].get_gl_y() - s.points[next1].get_gl_x() * s.points[i].get_gl_y();
            for(Polygon c : clipper){
                for(int j=0; j<c.points.size(); j++){
                    int next2  = (j == c.points.size() - 1) ? 0 : j+1;
                    GLfloat a2 = c.points[j].get_gl_y() - c.points[next2].get_gl_y();
                    GLfloat b2 = c.points[next2].get_gl_x() - c.points[j].get_gl_x();
                    GLfloat c2 = c.points[j].get_gl_x() * c.points[next2].get_gl_y() - c.points[next2].get_gl_x() * c.points[j].get_gl_y();
                    GLfloat seg1_l2_st = a2*s.points[i].get_gl_x() + b2*s.points[i].get_gl_y() + c2;
                    GLfloat seg1_l2_e = a2*s.points[next1].get_gl_x() + b2*s.points[next1].get_gl_y()  + c2;
                    GLfloat seg2_l1_st = a1 * c.points[j].get_gl_x() + b1 * c.points[j].get_gl_y()  + c1;
                    GLfloat seg2_l1_e = a1 * c.points[next2].get_gl_x() + b1 * c.points[next2].get_gl_y()  + c1;
                    if (seg1_l2_st*seg1_l2_e >= 0 || seg2_l1_st*seg2_l1_e >= 0) continue;
                    GLfloat u = seg1_l2_st/(seg1_l2_st-seg1_l2_e);
                    Point inter(s.points[i].get_gl_x() + u * b1, s.points[i].get_gl_y() + u * (-a1), width, height, 0, 0, s.get_id(), c.get_id());
                    subject_new.back().set_inter();
                    clipper_new[c.get_id()].set_inter();
                    insert(clipper_new[c.get_id()].points, c.points[j].get_gl_x(), c.points[j].get_gl_y(), inter);
                    insert(subject_new.back().points, s.points[i].get_gl_x(), s.points[i].get_gl_y(), inter);
                }
            }
        }
    }
    bool exit = false, flag = false, inside;
    for(Polygon &s : subject_new){
        for(int i=0; i<s.points.size(); i++){
            int index = find(s.points[i].get_gl_x(), s.points[i].get_gl_y(), subject[s.get_id()].points);
            if(index >= 0){
                for(Polygon &c : clipper_new) {
                    inside = is_point_in(s.points[i], c);
                    if(c.get_type() == 3) {
                        if(inside) {
                            flag = true;
                            exit = false;
                            break;
                        }
                    }
                    else if(inside)
                        flag = exit = true;
                }
                if (!flag)
                    exit = false;
                flag = false;
            }else{
                index = find(s.points[i].get_gl_x(), s.points[i].get_gl_y(), clipper_new[s.points[i].index_clipper].points);
                if(exit) {
                    s.points[i].exit = true;
                    clipper_new[s.points[i].index_clipper].points[index].exit = true;
                    exit_points.emplace_back(s.points[i]);
                }else{
                    s.points[i].enter = true;
                    clipper_new[s.points[i].index_clipper].points[index].enter = true;
                    enter_points.emplace_back(s.points[i]);
                }
                exit = !exit;
            }
        }
    }
    clipper = clipper_new;
    subject = subject_new;
}
vector<Polygon> WeilerAtherton::get_result() {
    return result;
}
void WeilerAtherton::insert(vector<Point> &points, GLfloat x, GLfloat y, Point inter) {
    int i;
    for(i = 0; i < points.size(); i++)
        if(points[i].get_gl_x() == x && points[i].get_gl_y() == y)
            break;
    while(i+1 != points.size() && sqrt((inter.get_gl_x()-x)*(inter.get_gl_x()-x)+(inter.get_gl_y()-y)*(inter.get_gl_y()-y)) >
          sqrt((points[i+1].get_gl_x()-x)*(points[i+1].get_gl_x()-x)+(points[i+1].get_gl_y()-y)*(points[i+1].get_gl_y()-y)))
        i++;
    points.insert(points.begin()+i+1, inter);
}
int WeilerAtherton::find(GLfloat x, GLfloat y, vector<Point> &points) {
    for(int i=0; i<points.size(); i++)
        if(points[i].get_gl_x() == x && points[i].get_gl_y() == y)
            return i;
    return -1;
}
void WeilerAtherton::change_orientation(Polygon *polygon) {
    GLfloat sum=0;
    int i;
    for(i=0; i<polygon->points.size()-1; i++)
        sum += polygon->points[i].get_gl_x() * polygon->points[i + 1].get_gl_y() - polygon->points[i + 1].get_gl_x() * polygon->points[i].get_gl_y();
    sum += polygon->points[i].get_gl_x() * polygon->points[0].get_gl_y() - polygon->points[0].get_gl_x() * polygon->points[i].get_gl_y();
    if(polygon->get_type() == 1 || polygon->get_type() == 3)
        if(sum < 0)
            reverse(polygon->points.begin(), polygon->points.end());
    if(polygon->get_type() == 0 || polygon->get_type() == 2)
        if(sum > 0)
            reverse(polygon->points.begin(), polygon->points.end());
}
bool WeilerAtherton::is_point_in(Point point, Polygon &polygon){
    bool c = false;
    int size = (int)polygon.points.size();
    for(int i=0, j = size - 1; i < size; j = i++){
        if((((polygon.points[i].get_gl_y()<=point.get_gl_y()) &&
             (point.get_gl_y()<polygon.points[j].get_gl_y())) ||
            ((polygon.points[j].get_gl_y()<=point.get_gl_y()) &&
             (point.get_gl_y()<polygon.points[i].get_gl_y()))) &&
           (point.get_gl_x() > (polygon.points[j].get_gl_x() - polygon.points[i].get_gl_x()) *
                                       (point.get_gl_y() - polygon.points[i].get_gl_y()) / (polygon.points[j].get_gl_y() -
                                                                                    polygon.points[i].get_gl_y()) + polygon.points[i].get_gl_x()))
            c = !c;
    }
    return c;
}