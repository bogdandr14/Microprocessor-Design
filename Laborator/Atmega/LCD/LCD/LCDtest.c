#include "defs.h"
#include <avr/io.h>

int main(){
   unsigned char k;
   sysinit();
   while(1){
      putsLCD("0123456789abcdef");
      gotoLC(2, 1);
      putsLCD("ghijklmnopqrstuv");
      for(k = 16; k > 0; --k){
         gotoLC(2, k);
         wait(500 * MS);
      }
      for(k = 16; k > 0; --k){
         gotoLC(1, k);
         wait(500 * MS);
      }
      clrLCD();
      wait(500 * MS);
   }
}
