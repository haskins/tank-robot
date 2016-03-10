/*
 * File:   srf04.h
 * Authors: Josh Haskins, Devon Harker, Vincent Tennant
 * Version: 1.0
 * Date: March 27, 2015
 * Notes: Controls functionality of the sonar sensors on the robot. The user
 * can turn on the sonars as well as initialize all of the hardware required
 * for the sonars to operate correctly.
 */

#include <delays.h>

#ifndef SRF04_H
#define	SRF04_H

#ifdef	__cplusplus
extern "C" {
#endif

    static volatile unsigned int ticksRight;    //Number of clock ticks 
                                                //between the right sonar
                                                //sending and recieving the
                                                //pulse.
    static volatile unsigned int ticksLeft;     //Number of clock ticks 
                                                //between the left sonar
                                                //sending and recieving the
                                                //pulse.
    static volatile unsigned char portB;        
    static volatile unsigned char ticksRL = 1;  //Used in the low priority
                                                //ISR to differentiate between
                                                //the left and right sonars.

    /*
     * Initializes the hardware that both the left and right sonars require in
     * order to operate correctly. This method must be called before either
     * sonar is able to be used.
     */
    void OpenSRF04() {
         //Sets the bottom 4 pins of A/D control register 1 so that the port
         //pins are reconfigured as digital I/O pins.
        ADCON1 |= 0b00001111;

         //Sets the data direction register B in such a way that the 4 pins
         //are digital input/output pins.
        TRISBbits.TRISB2 = 0; //output on right
        TRISBbits.TRISB3 = 0; //output on left
        TRISBbits.TRISB4 = 1; //input on left
        TRISBbits.TRISB5 = 1; //input on right

        OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_1);

        RCONbits.IPEN = 1; //Enable priority levels on interrupts
        INTCON2bits.RBIP = 0; //low priority for Change Interrupt Priority bit
        INTCONbits.RBIE = 1; //enables Change Interrupt Enable bit
        INTCON2bits.RBPU = 1; //all port b pull ups disabled
        /*
            Peripheral Interrupt Enable bit
            When IPEN = 0:
            1 = Enables all unmasked peripheral interrupts
            0 = Disables all peripheral interrupts
            When IPEN = 1:
            1 = Enables all low-priority peripheral interrupts
            0 = Disables all low-priority peripheral interrupts
         */
        INTCONbits.GIEL = 1;
        /*
            Global Interrupt Enable bit
            When IPEN = 0:
            1 = Enables all unmasked interrupts
            0 = Disables all interrupts
            When IPEN = 1:
            1 = Enables all high-priority interrupts
            0 = Disables all high-priority interrupts
         */
        INTCONbits.GIEH = 1;
    }

    /*
     * This is unused.
     */
    void CloseSRF04() { }

    /*
     * Triggers the right sonar on the robot. This causes the right sonar to
     * send out a sonar chirp. The chirp is then used to determine the number
     * of clock ticks between the sending and recieveing of said sonar chirp.
     */
    void TriggerRightSRF04() {
        /* ---------
        From the SRF04 manual we need a 10
        us pulse minimum so lets send a 40
        clock cycle pulse, about 16us.
        ----------- */
        LATBbits.LATB2 = 1; // Trigger the right sonar
        Delay10TCY();
	Delay10TCY();
	Delay10TCY();
	Delay10TCY();
        LATBbits.LATB2 = 0;
    }

    /*
     * Triggers the left sonar on the robot. This causes the left sonar to
     * send out a sonar chirp. The chirp is then used to determine the number
     * of clock ticks between the sending and recieveing of said sonar chirp.
     */
    void TriggerLeftSRF04() {
        /* ---------
        From the SRF04 manual we need a 10
        us pulse minimum so lets send a 40
        clock cycle pulse, about 16us.
        ----------- */
        LATBbits.LATB3 = 1; // trigger the left sonar
        Delay10TCY();
	Delay10TCY();
	Delay10TCY();
	Delay10TCY();
        LATBbits.LATB3 = 0;
    }

#ifdef	__cplusplus
}
#endif

#endif	/* SRF04_H */
