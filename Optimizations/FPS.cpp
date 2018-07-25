#include "FPS.h"
FPS::FPS() {
    sum_times = 0;
    times.assign(COUNT_OF_FRAMES_TO_AVERAGE, 0);
    is_full = false;
    index = 0;
}
void FPS::set_timer() {
    struct rusage rusage;
    if ( getrusage( RUSAGE_SELF, &rusage ) != -1 )
        time_fps = (double)rusage.ru_utime.tv_sec + (double)rusage.ru_utime.tv_usec / 1000000.0;
}

void FPS::add_record() {
    struct rusage rusage;
    double time1;
    if ( getrusage( RUSAGE_SELF, &rusage ) != -1 )
        time1 = (double)rusage.ru_utime.tv_sec + (double)rusage.ru_utime.tv_usec / 1000000.0;

    double time = time1 - time_fps;

    if (is_full) {
        sum_times -= times[index];
        sum_times += time;
        times[index] = time;
        index = (index + 1) % COUNT_OF_FRAMES_TO_AVERAGE;
    } else {
        times[index++] = time;
        sum_times += time;
        if (index == COUNT_OF_FRAMES_TO_AVERAGE) {
            is_full = true;
            index = 0;
        }
    }

}


double FPS::get_average_fps() {
    if (is_full)
        return (double)COUNT_OF_FRAMES_TO_AVERAGE / sum_times;
    else
        return (double)index / sum_times;
}