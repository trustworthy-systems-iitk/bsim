#ifndef _TIMER_H_DEFINED_
#define _TIMER_H_DEFINED_

#include <string>
#include <map>
#include <sys/time.h>
#include <iostream>

struct bsim_timer_t
{
    typedef std::map<std::string, double> map_t;
    map_t times;

    std::string current_task;
    timeval task_start;
    timeval prog_start;

    double get_time_diff();
    double get_total_time();

    bsim_timer_t() {
        gettimeofday(&prog_start, NULL);
    }
    ~bsim_timer_t() {}

    void start(const char* name);
    void stop(const char* name);

    void report(std::ostream& out);
};

extern bsim_timer_t timer;
#endif
