/*
**Hello Embedded World!
*/
#include <p32xxxx.h>

main() {
  DDPCONbits.JTAGEN = 0;
  TRISA = 0x00;
  PORTA = 0xe1;   // 1110 0001
}