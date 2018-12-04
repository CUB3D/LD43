//
// Created by cub3d on 04/12/18.
//

#ifndef LD43_TIMER_H
#define LD43_TIMER_H

#include <UK.h>
class Timer
{
public:
    long startTime;
    long endtime;

    Timer();

    virtual void render() const;

    static Timer& getTimer() {
        static Timer t;
        return t;
    }
};


#endif //LD43_TIMER_H
