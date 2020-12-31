#ifndef SW_TIMER_H
#define SW_TIMER_H

#include "stdint.h"
#include "etl/delegate.h"
typedef uint64_t (*f_int_t) (void); // Definovanie typu funkcie int fun_name(void);

class SwTimer
{
public:
	using callback_f = etl::delegate<uint64_t(void)>;
	/*! Constructor */
    SwTimer(callback_f func) :GlTimeFn(func) {}; //Konstruktor
    bool isExpired();
    void startTimer(uint64_t delay);
    void restartTimer();

private:
    callback_f GlTimeFn;  //Odkaz na funciu ktora vracia globalny cas
    uint64_t endTime; //Pomocna premnena
    uint64_t _delay; //Perioda casovaca
};

#endif
