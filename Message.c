#include <p32xxxx.h>

// DELAY's are driven by 36 MHz external clock, 27 ns period
// Each tick is scaled 1:256, so tick now = 6912 ns
#define SHORT_DELAY  22  
#define LONG_DELAY  177 
//#define SHORT_DELAY  2230  
//#define LONG_DELAY  65000

char bitmap[30] = {
	0xff,      // H      11111111
	0x08,      //        00001000
	0x08,      //        00001000
	0xff,      //        11111111
	0,
	0,
	0xff,      // E      11111111
	0x89,      //        10001001
	0x89,      //        10001001
	0x81,      //        10000001
	0,
	0,
	0xff,      // L      11111111
	0x80,      //        10000000
	0x80,      //        10000000
	0x80,      //        10000000
	0,
	0,
	0,
	0,
	0xff,      // L      11111111
	0x80,      //        10000000
	0x80,      //        10000000
	0x80,      //        10000000
	0,
	0,
	0,
	0,
	0x7e,      // O      01111110
	0x81,      //        10000001
	0x81,      //        10000001
	0x7e,      //        01111110
	0,
	0
};

main() {

    DDPCONbits.JTAGEN = 0;  // disable JTAGport, free up PORTA
  
    int i;

    TRISA = 0xff00;  // PORTA pins set to OUTPUT
    T1CON = 0x8030;  // TMR1 on, prescale 1:256
    PR1   = 0xffff;  // max period (not used)

    while(1) {
        
        // hand moving to the right
        for(i=0; i<30; i++) {
            PORTA = bitmap[i];
            TMR1 = 0;
            while(TMR1 < SHORT_DELAY) ;
        }   
     
        // hand moving to the left
        PORTA = 0;
        TMR1 = 0;
        while(TMR1 < LONG_DELAY) ;

        // hand moving to the left (again)
        PORTA = 0;
        TMR1 = 0;
        while(TMR1 < LONG_DELAY) ;

    }
}

    