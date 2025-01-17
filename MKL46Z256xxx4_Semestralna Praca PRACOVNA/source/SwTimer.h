#ifndef SW_TIMER_H
#define SW_TIMER_H

#include "stdint.h"
#include "etl/delegate.h"
typedef uint64_t (*f_int_t) (void); // Definovanie typu funkcie int fun_name(void);


class SwTimer /*!< Trieda pracujúca s počítaním času.. */
{
public:
	using callback_f = etl::delegate<uint64_t(void)>;
	/*! Constructor */
    SwTimer(callback_f func) :GlTimeFn(func) {}; //Konstruktor
    //! Vypršanie časovača.
    /*! zistí či časovač nastavený pomocou startTimer(uint64_t delay) už vypršal. */
    bool isExpired();
    //! Štart časovača.
    /*! Parameter sú milisekundy časovača*/
    void startTimer(uint64_t delay);
    void restartTimer();
    //! Zistenie koľko času ubehlo od spustenia startCountTime().
    /*! Vráti ubehnutý čas v milisekundách */
    uint64_t passedTime();
    void starCountTime();
private:
    callback_f GlTimeFn;  //Odkaz na funciu ktora vracia globalny cas
    uint64_t endTime; //Pomocna premnena
    uint64_t _delay; //Perioda casovaca
    uint64_t startTime; // pomocná premenna
};

#endif
