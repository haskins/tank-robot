// -------------------------------------------------------------------
// MPLAB C18 provides the #pragma config directive for setting 
// Configuration bits in C. Examples of use:
//
// #pragma config OSC = HS /* Oscillator Selection: HS */
// #pragma config WDT = ON, WDTPS=128 /* Enable watchdog timer and set postscaler to 1:128 */
//
// MPLAB® C18 C Compiler User’s Guide contains a general description 
// of the #pragma config directive. PIC18 Configuration Settings 
// Addendum contains all available Configuration settings and values for
// all PIC18 devices. MPLAB C18 --help-config command-line option lists
// the available Configuration settings and values of standard output for
// a specific device.
// ------------------------------------
/* PIC18F4320 - Configurations settings

	Taken from help documentation

*/
// Oscillator Selection bits:
	// OSC = LP  	// LP Oscillator  
	// OSC = XT  	// XT Oscillator  
	#pragma config	OSC = HS   	// HS Oscillator  
	// OSC = EC 	// EC oscillator, CLKO function on RA6  
	// OSC = ECIO	// EC oscillator, port function on RA6  
	// OSC = HSPLL	// HS oscillator, PLL enabled (clock frequency = 4 x FOSC1)  
	// OSC = RCIO	// External RC oscillator, port function on RA6  
	// OSC = INTIO2	// Internal RC oscillator, port function on RA6 and port function on RA7  
	// OSC = INTIO1	// Internal RC oscillator, CLKO function on RA6 and port function on RA7  
	// OSC = RC	// External RC oscillator, CLKO function on RA6  

// Fail-Safe Clock Monitor Enable bit:
 	#pragma config FSCM = OFF	// Fail-Safe Clock Monitor disabled  
	// FSCM = ON	// Fail-Safe Clock Monitor enabled  

// Internal/External Switchover bit:
 	#pragma config IESO = OFF 	// Internal/External Switchover mode disabled  
	// IESO = ON	// Internal/External Switchover mode enabled  

// Power-up Timer enable bit:
 	#pragma config PWRT = ON	// PWRT enabled  
	// PWRT = OFF	// PWRT disabled  

// Brown-out Reset enable bit:
 	// BOR = OFF	// Brown-out Reset disabled  
	#pragma config BOR = ON	// Brown-out Reset enabled  

// Brown-out Reset Voltage bits:
 	// BORV = 45	// VBOR set to 4.5V  
	#pragma config BORV = 42	// VBOR set to 4.2V  
	// BORV = 27	// VBOR set to 2.7V  
	// BORV = 20	// VBOR set to 2.0V  

// Watchdog Timer Enable bit:
 	#pragma config WDT = OFF	// WDT disabled (control is placed on the SWDTEN bit)  
	// WDT = ON	// WDT enabled  

// Watchdog Timer Postscale Select bits:
 	// WDTPS = 1	// 1:1  
	// WDTPS = 2	// 1:2  
	// WDTPS = 4	// 1:4  
	// WDTPS = 8	// 1:8  
	// WDTPS = 16	// 1:16  
	// WDTPS = 32	// 1:32  
	// WDTPS = 64	// 1:64  
	// WDTPS = 128	// 1:128  
	// WDTPS = 256	// 1:256  
	// WDTPS = 512	// 1:512  
	// WDTPS = 1024	// 1:1024  
	// WDTPS = 2048	// 1:2048  
	// WDTPS = 4096	// 1:4096  
	// WDTPS = 8192	// 1:8192  
	// WDTPS = 16384	// 1:16384  
	// WDTPS = 32768	// 1:32768  

// CCP2 MUX bit:
 	// CCP2MX = OFF		// CCP2 input/output is multiplexed with RB3  
	#pragma config CCP2MX = ON	// CCP2 input/output is multiplexed with RC1  

// PORTB A/D Enable bit:
 	#pragma config PBAD = DIG	// PORTB<4:0> pins are configured as digital I/O on Reset  
	// PBAD = ANA	// PORTB<4:0> pins are configured as analog input channels on Reset  

// MCLR Pin Enable bit:
 	// MCLRE = OFF	// MCLR disabled; RE3 input is enabled in 40-pin devices only (PIC18F4X20)  
	#pragma config MCLRE = ON	// MCLR pin enabled; RE3 input pin disabled  

// Stack Full/Underflow Reset Enable bit:
 	#pragma config STVR = OFF	// Stack full/underflow will not cause Reset  
	// STVR = ON	// Stack full/underflow will cause Reset  

// Single-Supply ICSP Enable bit:
 	#pragma config LVP = OFF	// Single-Supply ICSP disabled  
	// LVP = ON		// Single-Supply ICSP enabled  

// Background Debugger Enable bit:
 	// DEBUG = ON	// Background debugger enabled, RB6 and RB7 are dedicated to In-Circuit Debug  
	#pragma config DEBUG = OFF	// Background debugger disabled, RB6 and RB7 configured as general purpose I/O pins  

// Code Protection bit:
 	// CP0 = ON	// Block 0 (000200-0007FFh) code-protected  
	#pragma config CP0 = OFF	// Block 0 (000200-0007FFh) not code-protected  

// Code Protection bit:
 	// CP1 = ON		// Block 1 (000800-000FFFh) code-protected  
	#pragma config CP1 = OFF	// Block 1 (000800-000FFFh) not code-protected  

// Code Protection bit:
 	// CP2 = ON		// Block 2 (001000-0017FFh) code-protected  
	#pragma config CP2 = OFF	// Block 2 (001000-0017FFh) not code-protected  

// Code Protection bit:
 	// CP3 = ON		// Block 3 (001800-001FFFh) code-protected  
	#pragma config CP3 = OFF	// Block 3 (001800-001FFFh) not code-protected  

// Boot Block Code Protection bit:
 	// CPB = ON		// Boot block (000000-0001FFh) is code-protected  
	#pragma config CPB = OFF	// Boot block (000000-0001FFh) is not code-protected  

// Data EEPROM Code Protection bit:
 	// CPD = ON		// Data EEPROM is code-protected  
	#pragma config CPD = OFF	// Data EEPROM is not code-protected  

// Write Protection bit:
 	// WRT0 = ON	// Block 0 (000200-0007FFh) write-protected  
	#pragma config	WRT0 = OFF	// Block 0 (000200-0007FFh) not write-protected  

// Write Protection bit:
 	// WRT1 = ON 	// Block 1 (000800-000FFFh) write-protected  
	#pragma config WRT1 = OFF	// Block 1 (000800-000FFFh) not write-protected  

// Write Protection bit:
 	// WRT2 = ON	// Block 2 (001000-0017FFh) write-protected  
	#pragma config WRT2 = OFF	// Block 2 (001000-0017FFh) not write-protected  

// Write Protection bit:
 	// WRT3 = ON	// Block 3 (001800-001FFFh) write-protected  
	#pragma config WRT3 = OFF	// Block 3 (001800-001FFFh) not write-protected  

// Configuration Register Write Protection bit:
 	// WRTC = ON	// Configuration registers (300000-3000FFh) are write-protected  
	#pragma config WRTC = OFF	// Configuration registers (300000-3000FFh) are not write-protected  

// Boot Block Write Protection bit:
 	// WRTB = ON	// Boot block (000000-0001FFh) is write-protected  
	#pragma config WRTB = OFF	// Boot block (000000-0001FFh) is not write-protected  

// Data EEPROM Write Protection bit:
 	// WRTD = ON	// Data EEPROM is write-protected  
	#pragma config WRTD = OFF	// Data EEPROM is not write-protected  

// Table Read Protection bit:
 	// EBTR0 = ON	// Block 0 (000200-0007FFh) protected from table reads executed in other blocks  
	#pragma config EBTR0 = OFF	// Block 0 (000200-0007FFh) not protected from table reads executed in other blocks  

// Table Read Protection bit:
 	// EBTR1 = ON	// Block 1 (000800-000FFFh) protected from table reads executed in other blocks  
	#pragma config EBTR1 = OFF	// Block 1 (000800-000FFFh) not protected from table reads executed in other blocks  

// Table Read Protection bit:
 	// EBTR2 = ON	// Block 2 (001000-0017FFh) protected from table reads executed in other blocks  
	#pragma config EBTR2 = OFF	// Block 2 (001000-0017FFh) not protected from table reads executed in other blocks  

// Table Read Protection bit:
 	// EBTR3 = ON	// Block 3 (001800-001FFFh) protected from table reads executed in other blocks  
	#pragma config EBTR3 = OFF	// Block 3 (001800-001FFFh) not protected from table reads executed in other blocks  

// Boot Block Table Read Protection bit:
 	// EBTRB = ON	// Boot block (000000-0001FFh) is protected from table reads executed in other blocks  
	#pragma config EBTRB = OFF	// Boot block (000000-0001FFh) is not protected from table reads executed in other blocks  

