#include "Polygon.h"
Polygon::Polygon(Buffer *buffer, int width, int height) {
    this->mod = true;
    this->drawing = true;
    this->minx = INT_MAX;
    this->miny = INT_MAX;
    this->maxx = 0;
    this->maxy = 0;
    this->width = width;
    this->height = height;
    this->buffer = buffer;
}
void Polygon::set_current_cursor_position(double xpos, double ypos) {
    this->xpos = xpos;
    this->ypos = ypos;
}
void Polygon::set_current_framebuffer_size(int fwidth, int fheight) {
    this->fwidth = fwidth;
    this->fheight = fheight;
}
void Polygon::add_point(int x, int y) {
    if(!drawing) return;
    Point p = Point(x, y);
    minx = (minx > x)?x:minx;
    miny = (miny > y)?y:miny;
    maxx = (maxx < x)?x:maxx;
    maxy = (maxy < y)?y:maxy;
    points.emplace_back(p);
    gl_points.push_back(p.get_gl_x(width, height));
    gl_points.push_back(p.get_gl_y(width, height));
}
void Polygon::clear_data() {
    points.clear();
    edges.clear();
    gl_points.clear();
    buffer->clear_data();
    drawing = true;
    if(!mod)
        mod = true;
}
void Polygon::change_mod() {
    mod = false;
    for(int i=0; i<points.size()-1; i++)
        add_edge(points[i], points[i+1]);
    sort(edges.begin(), edges.end(), cmp);
}
void Polygon::draw_lines() {
    glColor3f(1, 1, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, gl_points.data());
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) gl_points.size() / 2);
    glDisableClientState(GL_VERTEX_ARRAY);
    if(!drawing) return;
    Point p = Point((int)xpos, (int)ypos);
    if(gl_points.size() >= 2){
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glVertex2f(gl_points[0], gl_points[1]);
        glVertex2f(p.get_gl_x(width, height), p.get_gl_y(width, height));
        if(gl_points.size() >= 4) {
            glColor3f(1, 0, 0);
            glVertex2f(gl_points[gl_points.size() - 2], gl_points[gl_points.size() - 1]);
            glVertex2f(p.get_gl_x(width, height), p.get_gl_y(width, height));
        }
        glEnd();
    }
}
void Polygon::draw() {
    if(mod)
        draw_lines();
    else
        draw_rasterize();
}
void Polygon::end_draw() {
    gl_points.push_back(gl_points[0]);
    gl_points.push_back(gl_points[1]);
    points.push_back(points.front());
    drawing = false;
    /*for(int i=0; i<edges.size(); i++){
        cout << edges[i].begin.get_x() << " " << edges[i].begin.get_y() << " :::: ";
        cout << edges[i].end.get_x() << " " << edges[i].end.get_y() << endl;
    }*/
}
void Polygon::resize_callback_after_drawing(int width, int height, int fw, int fh, int w, int h) {
    int new_minx = INT_MAX, new_miny = INT_MAX;
    for(int i=0, k=0; i<points.size(); i++){
        int x = (int)((double)points[i].get_x()) - minx+10;
        int y = (int)((double)points[i].get_y()) - miny+10;
        new_minx = (new_minx > x)?x:new_minx;
        new_miny = (new_miny > y)?y:new_miny;
        maxx = (maxx < x)?x:maxx;
        maxy = (maxy < y)?y:maxy;
        points[i] = Point(x, y);
        gl_points[k++] = points[i].get_gl_x(w, h);
        gl_points[k++] = points[i].get_gl_y(w, h);
    }
    minx = new_minx;
    miny = new_miny;
    buffer->resize(fw, fh);
    glViewport(0, 0, fw, fh);
    this->width = w;
    this->height = h;
    this->fwidth = fw;
    this->fheight = fh;
    double cof;
    if(w < width || h < height){
        cof = (double) w / (double) width;
        if (abs(w - width) > abs(h - height))
            cof = (double) w / (double) width;
        else
            cof = (double) h / (double) height;
    }
    if(w > width || h > height){
        if (abs(w - width) > abs(h - height)) {
            cof = (double) w / (double) width;
            if(cof*(double)maxx > (double)fwidth || cof * (double)maxy > (double)fheight)
                cof = (double) h / (double) height;
            if(cof*(double)maxx > (double)fwidth || cof * (double)maxy > (double)fheight)
                cof = 1.0;
        }
        else {
            cof = (double) h / (double) height;
            if(cof*(double)maxx > (double)fwidth || cof * (double)maxy > (double)fheight)
                cof = (double) w / (double) width;
            if(cof*(double)maxx > (double)fwidth || cof * (double)maxy > (double)fheight)
                cof = 1.0;
        }
    }
    for(int i=0, k=0; i<points.size(); i++){
        int x = (int)((double)points[i].get_x()*cof);
        int y = (int)((double)points[i].get_y()*cof);
        points[i] = Point(x, y);
        minx = (minx > x)?x:minx;
        miny = (miny > y)?y:miny;
        maxx = (maxx < x)?x:maxx;
        maxy = (maxy < y)?y:maxy;
        gl_points[k++] = points[i].get_gl_x(w, h);
        gl_points[k++] = points[i].get_gl_y(w, h);
    }
}
void Polygon::resize_callback_after_raster(int width, int height, int fw, int fh, int w, int h) {
    resize_callback_after_drawing(width, height, fw, fh, w, h);
    edges.clear();
    for(int i=0; i<points.size()-1; i++)
        add_edge(points[i], points[i+1]);
    sort(edges.begin(), edges.end(), cmp);
}
bool Polygon::get_mod() {
    return mod;
}
bool Polygon::cmp(Edge &a,Edge &b) {
    return (a.begin.get_y() < b.begin.get_y());
}
void Polygon::add_edge(Point a, Point b) {
    double cofx = (double)fwidth/(double)width;
    double cofy = (double)fheight/(double)height;
    a = Point((int)((double)a.get_x()*cofx), (int)((double)a.get_y()*cofy));
    b = Point((int)((double)b.get_x()*cofx), (int)((double)b.get_y()*cofy));
    Edge e = Edge(b, a);
    if(a.get_y() < b.get_y())
        e = Edge(a, b);
    if(e.end.get_y() - e.begin.get_y() != 0) {
        e.dx = (e.end.get_x() - e.begin.get_x()) / (float) (e.end.get_y() - e.begin.get_y());
        edges.push_back(e);
    }
}
void Polygon::draw_interval(int x1, int x2, int y) {
    for(int i = x1; i <= x2; i++)
        buffer->draw_pixel(i, y, 1.0f, 1.0f, 1.0f);
}
void Polygon::add_active_edge(int scan_y, vector<Edge> *active_edges) {
    for(int i = 0; i < edges.size(); i++)
        if(edges[i].begin.get_y() == scan_y) {
            edges[i].scan_x = edges[i].begin.get_x();
            active_edges->push_back(edges[i]);
        }
}
void Polygon::delete_passive_edge(int scan_y, vector<Edge> *active_edges) {
    for(int i = 0; i < active_edges->size(); i++)
        if(active_edges->at(i).end.get_y() <= scan_y)
            active_edges->erase(active_edges->begin() + i--);
}
void Polygon::find_intersections(vector<Edge>* active_edges, vector<int>* intersections){
    for(int i = 0; i < active_edges->size(); i++){
        intersections->push_back((int)active_edges->at(i).scan_x);
        active_edges->at(i).scan_x += active_edges->at(i).dx;
    }
}

void Polygon::draw_rasterize() {
    int scan_y = 0;
    vector<Edge> active_edges;
    vector<int> intersections;
    add_active_edge(scan_y, &active_edges);
    while(scan_y < buffer->height){
        delete_passive_edge(scan_y,&active_edges);
        add_active_edge(scan_y,&active_edges);
        find_intersections(&active_edges, &intersections);
        sort(intersections.begin(),intersections.end());
        for(int i = 1; i < intersections.size();i+=2)
            draw_interval(intersections[i - 1], intersections[i], scan_y);
        intersections.clear();
        scan_y++;
    }
    buffer->draw();
}
void Polygon::filter() {
    int width = buffer->width;
    int height = buffer->height;
    buffer->make_acc_buffer();
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int topl = (y != 0 && x != 0) ? 1 : 0;
            int top = (y != 0) ? 2 : 0;
            int topr = (y != 0 && x != width - 1) ? 1 : 0;
            int midl = (x != 0) ? 2 : 0;
            int mid = 4;
            int midr = (x != width-1) ? 2 : 0;
            int botl = (x != 0 && y != height - 1) ? 1 : 0;
            int bot = (y != height-1) ? 2 : 0;
            int botr = (y != height-1 && x != width - 1) ? 1 : 0;
            for(int k = 0; k < 3; k++) {
                float buf1 = ((topl > 0) ? topl * buffer->acc_buffer[(x-1) * 3 + (height-y)*width*3 + k] : 0)
                            + ((top > 0) ? top * buffer->acc_buffer[x * 3 + (height-y)*width*3 + k] : 0)
                            + ((topr > 0) ? topr * buffer->acc_buffer[(x+1) * 3 + (height-y)*width*3 + k] : 0)
                            + ((midl > 0) ? midl * buffer->acc_buffer[(x-1) * 3 + (height-y-1)*width*3 + k] : 0)
                            + (mid) * buffer->acc_buffer[x * 3 + (height-y-1)*width*3 + k]
                            + ((midr > 0) ? midr * buffer->acc_buffer[(x+1) * 3 + (height-y-1)*width*3 + k] : 0)
                            + ((botl > 0) ? botl * buffer->acc_buffer[(x-1) * 3 + (height-y-2)*width*3 + k] : 0)
                            + ((bot > 0) ? bot * buffer->acc_buffer[x * 3 + (height-y-2)*width*3 + k] : 0)
                            + ((botr > 0) ? botr * buffer->acc_buffer[(x+1) * 3 + (height-y-2)*width*3 + k] : 0);

                float del = topl + top + topr + midl + mid + midr + botl + bot + botr;
                buffer->pixels[x * 3 + (height-y-1)*width*3 + k] = buf1 / del;
            }
        }
    }
    buffer->make_acc_buffer();
}