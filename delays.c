#include <xc.h>

void DelayFor18TCY(void) {
  //WH0423
  char b;
  b = GIEH;
  GIEH = 0;
  Nop(); // 1
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop();
  Nop(); // 18 ... more than enough
  GIEH = b;
}

void DelayPORXLCD(void) {
  Delay10KTCYx(3);
}

void DelayXLCD(void) {
  Delay10KTCYx(1);
}
