/*
 * File:   delay.h
 * Authors: Josh Haskins, Devon Harker, Vincent Tennant
 * Version: 1.0
 * Date: January 30, 2015
 * Notes: A delay function call. There are both milisecond and second delay
 * functions.
 *
 */

#include <xc.h>  //general purpose header file, inclde in all projects
#include <delays.h>  //general purpose header file, inclde in all projects

#ifndef DELAYS_H
#define	DELAYS_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*
     * This function will cause a delay of of a length equal to the value
     * passed in miliseconds.
     *
     * @param x the number of miliseconds to delay.
     */
    void delay_ms(unsigned int x) {
        Delay10KTCYx(x / 8);
    }

    /*
     * This function will cause a delay of of a length equal to the value
     * passed in seconds.
     *
     * @param x the number of seconds to delay.
     */
    void delay_s(unsigned int x) {
        Delay10KTCYx(x * 100);
    }


#ifdef	__cplusplus
}
#endif

#endif	/* DELAYS_H */

