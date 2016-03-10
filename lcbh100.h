/*
 * File:   lcdhb100.h
 * Author: Devon Harker, Joshua Haskins, Vincent Tennant
 * Version: 1.0
 * Date: February 6, 2015
 *
 * LCHB-100 is the board we are using to control the motor. This header file
 * provides the functionality for the two motors. This functionaitly includes:
 * opening the motor so that it can be used, closing the motors by disabling
 * both forward and backwards movement, and allows both motors to be set to
 * different speed.
 *
 * In order for Run to work correctly, Open should be called first to setup
 * up the motors for use. Currently, the state of both motors must be altered
 * at the same time. While values between positive and negative 0 and 255
 * (inclusive) are accepted by Run, values must be greater than +190 or
 * less than -190 in order for there to be an observable effect. This may be
 * machine dependent.
 *
 * PORTA for direction
 *
 * 1 FWD WHT - A0
 * 1 EN BRN - RC2
 * 1 REV ORG - A1
 * 2 FWD BLU - A2
 * 2 EN GRN - RC1
 * 2 REV PUR - A3
 *
 */

#include <timers.h> //functionality for the timers
#include <pwm.h>    //functionality for pulse width modulation
#include <xc.h>  //general purpose header file, inclde in all projects
#include <stdlib.h> //used for absolute value


#ifndef LCHB100_H
#define	LCHB100_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*
     * Sets the 4 pins that the motor will use to digital output pins. This
     * method must be called prior to the Run method in order for it to work
     * correctly.
     */
    Openlchb100() {

        /*
         * sets the bottom 4 pins of A/D control register 1 so that the port
         * pins are reconfigured as digital I/O pins.
         */
        ADCON1 = 0b00001111;

        /*
         * Sets the data direction register A in such a way that the 4 pins
         * are digital output pins.
         */
        DDRAbits.RA0 = 0;
        DDRAbits.RA1 = 0;
        DDRAbits.RA2 = 0;
        DDRAbits.RA3 = 0;

        /*
         * Configures open timer 2 by disabling interrupts from this timer
         * as well as setting the pre and post scaler to 1
         */
        OpenTimer2(TIMER_INT_OFF & T2_PS_1_1 & T2_POST_1_1);

        /*
         * Configures the two PWM channels for the motors. The value passed
         * (63)is used to determine the PWM frequency.
         */
        OpenPWM1(0x3F);
        OpenPWM2(0x3F);
    }

    /*
     * Sets the 4 pins that are used by the motors to low, effectivly stopping
     * the motors and the robot's movement.
     */
    Closelchb100() {
        LATAbits.LATA0 = 0; //forward enable for motor 1
        LATAbits.LATA1 = 0; //reverse enable for motor 1
        LATAbits.LATA2 = 0; //forward enable for motor 2
        LATAbits.LATA3 = 0; //reverse enable for motor 2
    }

    /*
     * Controls the speed and direction of travel. While values between
     * positive and negative 0 and 255 (inclusive) are accepted by Run, values
     * must be greater than +190 or less than -190 in order for there tobe an
     * observable effect. This may be machine dependent.
     *
     * @param port The speed at which the left hand motor operates. Positive
     * values are used for travelling forward while negative values are used
     * for travelling backwards.
     * @param starboard The speed at which the right hand motor operates.
     * Positive values are used for travelling forward while negative values
     * are used for travelling backwards.
     */
    Runlchb100(int starboard, int port) {
        Closelchb100(); //Disable both motors to avoid overloading.

        //Case 1: both motors forward
        if (port >= 0 && starboard >= 0) {
            LATAbits.LATA0 = 1;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 0;

        }
        //Case 2: left forward, right reverse
        else if (port >= 0 && starboard < 0) {
            LATAbits.LATA0 = 1;
            LATAbits.LATA1 = 0;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 1;

        }
        //Case 3: left reverse, right forward
        else if (port < 0 && starboard >= 0) {
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 1;
            LATAbits.LATA2 = 1;
            LATAbits.LATA3 = 0;

        }
        //Case 4: both motors reverse
        else if (port < 0 && starboard < 0) {
            LATAbits.LATA0 = 0;
            LATAbits.LATA1 = 1;
            LATAbits.LATA2 = 0;
            LATAbits.LATA3 = 1;
        }

        //Set each engine to the appropriate magnitude.
        SetDCPWM1(abs(port));
        SetDCPWM2(abs(starboard));
    }

#ifdef	__cplusplus
}
#endif

#endif	/* LCHB100_H */