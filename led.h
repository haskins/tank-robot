/* 
 * File:   led.h
 * Authors: Josh Haskins, Devon Harker, Vincent Tennant
 * Version: 1.0
 * Date: January 23, 2015
 * Notes: Controls the functionality of LEDs on the robot. The user can turn 
 * all LEDs off with closeLED(). The user is able to set which LEDs to turn
 * off/on by passing an usigned char as a parameter to setLED(). openLED()
 * must be called before setLED() will work correctly. setLED() overwites the
 * current state of all LEDs on port D on the robot.
 *
 */

#include <xc.h>
#include <delays.h>

#ifndef LED_H
#define	LED_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*
     * This function sets up the LEDs for output on the robot. This method
     * must be called prior to calling any other methods in this file.
     */
    void openLED() {
        //Sets all the pins for port D to digital output.
        DDRD = 0;
    }

    /*
     * This function sets the state of all LEDs on port D of the robot to off.
     */
    void closeLED() {
        LATD = 0b00000000;
    }

    /*
     * This function uses a user supplied unsigned char to set the state of 
     * the LEDs. This function will overwrite the existing state of all of
     * the LEDs.
     *
     * @param value 8-bit value to set LEDs.
     */
    void setLED(unsigned char value) {
        LATD = value;
    }


#ifdef	__cplusplus
}
#endif

#endif	/* LED_H */

