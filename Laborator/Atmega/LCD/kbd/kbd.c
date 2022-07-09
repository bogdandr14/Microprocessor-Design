/*
 * kbd.c
 *
 * Created: 11/04/2021 20:26:18
 * Author : bogda
 */ 
#include "defs.h"
#include <avr/io.h>
#define DELAY 200
int main(){
   char code_ante = NOKEY;
   char code_now = NOKEY;
   unsigned char kbhit = 0;
   char kbcode;
   unsigned char loop_cnt=0;
   sysinit(); //nu este in curs, dar este necesar
   while(1){ //bucla principala
      //…
      if(loop_cnt==DELAY){ //citirile se fac din 15 ms in 15 ms
         loop_cnt=0;
         code_ante = code_now;
         code_now = kbscan();
         if( code_ante == NOKEY && code_now != NOKEY){
            kbhit=1;
            kbcode=code_now;
         }
      }
      //……
      //consuma codul
      if(kbhit){
         kbhit=0;
         putchLCD(kbcode);
      }
      //……
      loop_cnt++;
   } //end while
} //end main