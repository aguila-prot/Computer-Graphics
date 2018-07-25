#include <sys/resource.h>
#include <sys/times.h>
#include <vector>
#define COUNT_OF_FRAMES_TO_AVERAGE 200
using namespace std;
class FPS {
private:
    double sum_times;
    vector<double> times;
    bool is_full;
    int index = 0;
    double time_fps;
public:
    FPS();
    void set_timer();
    void add_record();
    double get_average_fps();
};