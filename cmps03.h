/*
 * File:   cmps03.h
 * Authors: Josh Haskins, Devon Harker, Vincent Tennant
 * Version: 1.0
 * Date: March 27, 2015
 * Notes: Controls functionality of the compass. The user
 * can turn on the compass as well as initialize all of the hardware required
 * for the compass to operate correctly.
 * References: cmps03.h by Allen Kranz
 */


#include <i2c.h>

#ifndef CMPS03_H
#define	CMPS03_H

#ifdef	__cplusplus
extern "C" {
#endif

    /*
     * Initializes the hardware that the compass requires in
     * order to operate correctly. This method must be called before the
     * compass is able to be used.
     */
    void OpenCMPS03(void) {
        // PIC18F2220/2320/4220/4320 Data Sheet DS39599G-page 164
        // Two pins are used for data transfer:
        // Serial Clock (SCL)  RC3/SCK/SCL
        // Serial Data (SDA)  RC4/SDI/SDA
        // The user must configure these pins as inputs using the
        // TRISC<4:3> bits.
        TRISCbits.TRISC3 = 1;
        TRISCbits.TRISC4 = 1;

        // PIC18F2220/2320/4220/4320 Data Sheet DS39599F, page 201
        // TABLE 18-5: BAUD RATES FOR SYNCHRONOUS MODE (SYNC = 1)
        // 96.15 kHz @ 10MHz SSPADD = 25
        SSPADD = 0x09;

        //Configures the I2C in SSP2 module.
        OpenI2C(MASTER, SLEW_OFF);
    }

    /*
     * Disables the serial communication from the robot to the compass.
     */
    void CloseCMPS03(void) {
        //This function turns off the I2C module
        CloseI2C();
    }

    /*
     * Gets the current bearing that the compass is reading. It accomplishes
     * this by communicating through the serial communication module.
     */
    unsigned char ReadCMPS03(void) {
        unsigned char byte = 0;

        //This function generates Wait condition until I2C bus is Idle
        IdleI2C();

        //initializes serial communication module
        StartI2C();
        IdleI2C();

        //This function is used to write out a single data byte to the
        //I2C device.
        WriteI2C(0xC0);
        IdleI2C();

        //This function is used to write out a single data byte to the
        //I2C device.
        WriteI2C(0x01);
        IdleI2C();

        //restart I2C communication
        RestartI2C();
        IdleI2C();
        WriteI2C(0xC1);
        IdleI2C();

        //This function is used to read a single byte from I2C bus
        byte = ReadI2C();

        //send the end of transmission signal through nack
        NotAckI2C();

        IdleI2C();

        //stop I2C communication
        StopI2C();

        return (byte);
    }



#ifdef	__cplusplus
}
#endif

#endif	/* CMPS03_H */