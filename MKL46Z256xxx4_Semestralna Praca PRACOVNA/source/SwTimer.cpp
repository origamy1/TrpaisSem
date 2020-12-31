#include "SwTimer.h"

bool SwTimer::isExpired(){
    return  GlTimeFn() >= (endTime);
}

void SwTimer::startTimer(uint64_t delay){
    _delay=delay;
    endTime =  GlTimeFn() + delay;
}

void SwTimer::restartTimer(){
    SwTimer::startTimer(_delay);
}
