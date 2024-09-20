/*
 * Loops (Project 2)
 *
 *   Configuration Bits set for:  72 MHz System Clock
 *                                36 MHz Peripheral Bus Clock
 *       36 MHz PBC = (1/36000000=0.00000002777) = 27.7ns
 *       Each tick (wth 256 prescaler) takes = 27.2 * 256 = 7.11us
 * 
 *       We want to blink the LED twice per second, on=256ms, off=256ms
 *       How many times does 7.11us go into 256 ms?
 *            256000 / 7.11 = 36,006 
 */

#include <p32xxxx.h>

//#define DELAY 36006   // 256 ms
#define DELAY 36   // 256 ms
main() {

    // initialization
    DDPCONbits.JTAGEN = 0;   // disable JTAG port, free up PORT A for I/O
    TRISA = 0xff00;          // set all PORT A pins to OUTPUT

    T1CON = 0x8030;          // TMR1 on, Prescaler 1:256 = 3
                             //          Prescaler 1:1   = 0
    PR1 = 0xFFFF;            // set period register to max

    // main loop
    while(1) {
    
        // turn on some LEDs
        PORTA = 0xff;  // turn pins 0-7 on, Port A
        TMR1 = 0;      // reset 16-bit timer, Timer1
        while (TMR1 < DELAY) ; // do nothing

        // turn off all LEDs
        PORTA = 0x00;  // turn all pins off
        TMR1 = 0;      // reset 16-bit timer, Timer1
        while (TMR1 < DELAY) ; // do nothing
    }
}