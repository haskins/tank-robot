/*
 * File:   clock.h
 * Authors: Josh Haskins, Devon Harker, Vincent Tennant
 * Version: 1.0
 * Date: January 30, 2015
 * Notes: The header file provides fuctionality for the TIMER3 register on the
 * robot. TIMER3 can be opened, ie. enabled for use. Closed to prevent use
 * as well as get the current clock tick. This clock runs in approximatly 1/16
 * second within 2.4% accuracy. The clock has an approximate maxmium lifespan
 * of 70 minutes. Both high and low interrupts are capable of being used,
 * though we have only implemented the high interrupts for the time being.
 *
 */
#include <xc.h>
#include <timers.h>


#ifndef CLOCK_H
#define	CLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif


    /**
     * The current clock tick, incremented by highISR. Can store a maximum
     * of 65535 clock ticks.
     */
    static volatile unsigned int tick_units = 0;

    /**
     * Opens the clock, TIMER3, to be enable to be used.
     */
    void OpenClock() {

        OpenTimer3(TIMER_INT_ON & //enables inturrupts for the timer
                T3_16BIT_RW & //configs as a 16-bit wide timer
                T3_SOURCE_INT & //Sets the clock source for the timer
                T3_PS_1_1 & //sets the prescaler value to 1
                T3_SYNC_EXT_OFF); //disables sychronized clock input

        /**
         * interrupt priority enable bit, changes to 2 levels of interrupts
         * system (high and low)
         */
        RCONbits.IPEN = 1;
        INTCONbits.GIEL = 1; //globally enabling low interrupt
        INTCONbits.GIEH = 1; //globally enabling high interrupt
        IPR2bits.TMR3IP = 1; //so clock can start generating interrupts
    }

    /**
     * Disables TIMER3. Prevents it from being used.
     */
    void CloseClock() {
        CloseTimer3();
    }

    /**
     * Returns the approximate 1/16 second ticks since startup.
     * @return tick_units
     */
    unsigned int GetClock() {
        return tick_units;
    }



#ifdef	__cplusplus
}
#endif

#endif	/* CLOCK_H */

