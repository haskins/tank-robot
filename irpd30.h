/* 
 * File:   irpd30.h
 * Author: haskins
 *
 * Created on February 13, 2015, 11:40 AM
 */

#include <xc.h>



#ifndef IRPD30_H
#define	IRPD30_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef char BOOL;
#define TRUE 1;
#define FALSE 0;
#define INPUT 1;
#define OUTPUT 0;
#define ENABLE 1;
#define DISABLE 0;

    OpenIRPD30() {

        ADCON1 = 0b00001010;

        /*
         * Sets the data direction register A in such a way that the 4 pins
         * are digital output pins.
         */
        DDREbits.RE0 = INPUT;
        DDREbits.RE1 = OUTPUT;
        DDREbits.RE2 = OUTPUT;


    }

    CloseIRPD30() {
        LATEbits.LATE0 = 0;
        LATEbits.LATE1 = 0;
        LATEbits.LATE2 = 0;
    }

    unsigned char DetectIRPD30() {

        unsigned char x = 't';
        int right = FALSE;
        int left = FALSE;

        /*
         Violet = Left LED Enable E2
         Blue = Right LED Enable E1
         Yellow = Sensor Output E0
         */

        int action = 0;

        //check right
        LATEbits.LATE1 = ENABLE;
        LATEbits.LATE2 = DISABLE;

        for (int i = 0; i < 1000; i++) {
            if (PORTEbits.RE0 == 0) {
                right = TRUE;
                break;
            }
        }

        //avoid error state
        LATEbits.LATE1 = DISABLE;
        LATEbits.LATE2 = DISABLE;
        Delay1KTCYx(1);

        //check left
        LATEbits.LATE1 = DISABLE;
        LATEbits.LATE2 = ENABLE;

        for (int i = 0; i < 1000; i++) {
            if (PORTEbits.RE0 == 0) {
                left = TRUE;
                break;
            }
        }

        if (left == 0 && right == 0) {
            return 0b00000000;
        } else if (left == 0 && right == 1) {
            return 0b00000001;
        } else if (left == 1 && right == 0) {
            return 0b00000010;
        } else if (left == 1 && right == 1) {
            return 0b00000011;
        }
        return 0b11111111;
    }


#ifdef	__cplusplus
}
#endif

#endif	/* IRPD30_H */

