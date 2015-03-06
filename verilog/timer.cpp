#include "timer.h"
#include "assert.h"
#include <iomanip>

bsim_timer_t timer;

double bsim_timer_t::get_time_diff()
{
    assert(current_task.size() > 0);

    timeval now;
    gettimeofday(&now, NULL);

    double del_sec = now.tv_sec - task_start.tv_sec;
    double del_usec = now.tv_usec - task_start.tv_usec;

    return del_sec + del_usec*1e-6;
}

double bsim_timer_t::get_total_time()
{
    timeval now;
    gettimeofday(&now, NULL);

    double del_sec = now.tv_sec - prog_start.tv_sec;
    double del_usec = now.tv_usec - prog_start.tv_usec;

    return del_sec + del_usec*1e-6;
}

void bsim_timer_t::start(const char* name)
{
    assert(current_task.size() == 0);
    current_task = name;

    gettimeofday(&task_start, NULL);
}

void bsim_timer_t::stop(const char* name)
{
    assert(current_task == name);
    double del = get_time_diff();
    times[current_task] += del;
    current_task.clear(); 
}

void bsim_timer_t::report(std::ostream& out)
{
    out << "============================" << std::endl;
    out << "        TIMER REPORT        " << std::endl;
    out << "============================" << std::endl;
    for(map_t::iterator it = times.begin(); it != times.end(); it++) {
        out << std::setw(20) << std::left << it->first << ":" 
            << std::setw(10) << std::setprecision(2) << it->second 
            << std::endl;
    }
}
