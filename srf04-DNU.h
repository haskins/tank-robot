// ---
// srf04.h
// Ver 1.0.0
// ---

// Sample code for the PIC 18f4320 Microprocessor.
// Copyright (C) 2007 University of Northern British Columbia

// This file is part of the UNBC Robotics Library.  This library is free
// software; you can redistribute it and/or modify it under the terms of
// the GNU General Public License as published by the Free Software
// Foundation; either version 2, or (at your option) any later version.

// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 59 Temple Place - Suite 330, Boston, MA  02111-1307, 
// USA.

// As a special exception, if you use this code with files compiled
// with a GNU compiler to produce an executable, this does not cause the
// resulting executable to be covered by the GNU General Public License.
// This exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

// ---
// Written by Allan Kranz
// Senior laboratory Instructor (Computer Science)
// University of Northern British Columbia
// ---

#include <portb.h>
#include <timers.h>

unsigned int ticksRight;
unsigned int ticksLeft;
unsigned char portb;
unsigned char triggerRL = 1; // Set to 1 for left and 2 for right trigger
unsigned char ticksRL = 1; // Set to 1 for left and 2 for right trigger

void low_isr(void);

/*
* For PIC18 devices the low interrupt vector is found at
* 00000018h. The following code will branch to the
* low_interrupt_service_routine function to handle
* interrupts that occur at the low vector.
*/
#pragma code low_vector=0x18

void interrupt_at_low_vector(void)
{
	_asm GOTO low_isr _endasm
}


#pragma code /* return to the default code section */

#pragma interruptlow low_isr

void low_isr (void)
{
    if( INTCONbits.RBIF == 1 ) // Interrupt raised by change in one of PORTB<7:4>
    {
		// right sonar echo just went high so start clock
		if( PORTBbits.RB4 == 1 && PORTBbits.RB5 == 0)
        {
            WriteTimer0( 0 );
            ticksRL = 2; // Set to 1 for left and 2 for right return pulse
        }
		// left sonar echo just went high so start clock
		else if( PORTBbits.RB4 == 0 && PORTBbits.RB5 == 1 )
        {
            WriteTimer0( 0 );
            ticksRL = 1; // Set to 1 for left and 2 for right return pulse
        }
		
		// left and right have finished
		if( PORTBbits.RB4 == 0 && PORTBbits.RB5 == 0 )
        {
            if( ticksRL == 2 )// Set to 1 for left and 2 for right return pulse
			{
				ticksRight = ReadTimer0();
			}
            if( ticksRL == 1 )// Set to 1 for left and 2 for right return pulse
			{
				ticksLeft = ReadTimer0();
			}
        }

        portb = PORTB; // Clear the mismatch on PORTB
    	INTCONbits.RBIF = 0; // reset the interrupt flag
    } 
	// Check for any other low level interrupts
}


void OpenSRF04(void)
{
    unsigned char temp = PORTB;
    ADCON1 |= 0b00001111;
    TRISB = 0b11110000; // This too

    OpenTimer0(TIMER_INT_OFF & T0_16BIT & T0_SOURCE_INT & T0_PS_1_2);

	/* ---------
	In general, each interrupt source has three bits to
	control its operation. The functions of these bits are:
		- Flag bit to indicate that an interrupt event
			occurred
		- Enable bit that allows program execution to
			branch to the interrupt vector address when the
			flag bit is set
		- Priority bit to select high priority or low priority
			(most interrupt sources have priority bits)
	---------- 	*/
	

	/* ----------
	The RCON register contains bits used to determine the
	cause of the last Reset or wake-up from powermanaged
	mode. RCON also contains the bit that enables interrupt 
	priorities (IPEN).

	IPEN: Interrupt Priority Enable bit

	1 = Enable priority levels on interrupts

	The interrupt priority feature is enabled by setting the
	IPEN bit (RCON<7>). When interrupt priority is
	enabled, there are two bits which enable interrupts
	globally. Setting the GIEH bit (INTCON<7>) enables all
	interrupts that have the priority bit set (high priority).
	Setting the GIEL bit (INTCON<6>) enables all interrupts
	that have the priority bit cleared (low priority).
	When the interrupt flag, enable bit and appropriate
	global interrupt enable bit are set, the interrupt will vector
	immediately to address 000008h or 000018h,
	depending on the priority bit setting. Individual interrupts
	can be disabled through their corresponding
	enable bits.

	0 = Disable priority levels on interrupts (PIC16CXXX Compatibility mode)

	When the IPEN bit is cleared (default state), the
	interrupt priority feature is disabled and interrupts are
	compatible with PIC® mid-range devices. In Compatibility
	mode, the interrupt priority bits for each source
	have no effect. INTCON<6> is the PEIE bit which
	enables/disables all peripheral interrupt sources.
	INTCON<7> is the GIE bit which enables/disables all
	interrupt sources. All interrupts branch to address
	000008h in Compatibility mode.
	------------- */
    RCONbits.IPEN = 1; /* Enable interrupt priority */

    /* -----------
	RBIP: RB Port Change Interrupt Priority bit
		1 = High priority
		0 = Low priority
	---------- */	
	INTCON2bits.RBIP = 0; 

	/* ----------
	RBIE: RB Port Change Interrupt Enable bit
		1 = Enables the RB port change interrupt
		0 = Disables the RB port change interrupt
	---------- */
	INTCONbits.RBIE = 1; 

	/* ----------	
	RBPU: PORTB Pull-up Enable bit
		1 = All PORTB pull-ups are disabled
		0 = PORTB pull-ups are enabled by individual port latch values
	--------- */
	INTCON2bits.RBPU = 1;
    
	/* ----------
	PEIE/GIEL: Peripheral Interrupt Enable bit
		When IPEN = 0:
			1 = Enables all unmasked peripheral interrupts
			0 = Disables all peripheral interrupts
		When IPEN = 1:
			1 = Enables all low-priority peripheral interrupts
			0 = Disables all low-priority peripheral interrupts
	---------- */
	INTCONbits.GIEL = 1;

	/* ----------
	GIE/GIEH: Global Interrupt Enable bit
		When IPEN = 0:
			1 = Enables all unmasked interrupts
			0 = Disables all interrupts
		When IPEN = 1:
			1 = Enables all high-priority interrupts
			0 = Disables all high-priority interrupts
	---------- */
	INTCONbits.GIEH = 1;






}

void CloseSRF04(void)
{


}

unsigned char TriggerLeftSRF04()
{
	        /* --------- 
		From the SRF04 manual we need a 10
		us pulse minimum so lets send a 40
		clock cycle pulse, about 16us.
		----------- */
		//if(triggerRL == 1)
		//{
		PORTBbits.RB2 = 1; // Trigger the left sonar
		Delay10TCY();
		Delay10TCY();
		Delay10TCY();
		Delay10TCY();
        	PORTBbits.RB2 = 0;
		//	triggerRL = 2; // Set to 1 for left and 2 for right
		//}
}

unsigned char TriggerRightSRF04()
{
	        /* --------- 
		From the SRF04 manual we need a 10
		us pulse minimum so lets send a 40
		clock cycle pulse, about 16us.
		----------- */
		//if(triggerRL == 2)
		//{
		PORTBbits.RB3 = 1; // trigger the right sonar 
		Delay10TCY();
		Delay10TCY();
		Delay10TCY();
		Delay10TCY();
        	PORTBbits.RB3 = 0;
		//	triggerRL = 1; // Set to 1 for left and 2 for right
		//}

}

