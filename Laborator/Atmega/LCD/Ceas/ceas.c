/*
* kbd.c
*
* Created: 11/04/2021 20:26:18
* Author : bogda
*/
#include "defs.h"
#include <avr/io.h>
#include <stdio.h>
#define DELAY 200
//include ?i prototipuri
int main(){
   //declara?ii variabile pentru tastatur? – deja exist?
   unsigned int cycles = 0;
   // alte declara?ii variabile pentru ceas
   unsigned int second = 00, minute = 07, hour = 11;
   //ini?ializ?ri pentru ceas
   sysinit();
   unsigned char temp;
   //char buf[17];
   //Foarte important: dupa sysinit() si înainte de while(1):7
   // configureaza Timer 2 (modul, CTC, p, N) prin intermediul lui TCCR2
   // si OCR2;
   unsigned int N = 250, k = 125;
   TCCR2 = 0b00001110;
   OCR2 = N - 1;
   while(1){
      //Codul pentru tastatura. NU modifica!
      temp = kbscan();
      if(NOKEY != temp)
      {
         gotoLC(1, 1);
         putchLCD(temp);
      }
      if(TIFR & 1<<OCF2){//
         //TIFR |= 1 << OCF2; //Reseteaza TF;
         setbit(TIFR, OCF2);
         ++cycles;
         if(cycles == k){
            cycles = 0;
            // inverseaza starea separatorului
            ++second;
            if(second > 59){
               second = 0;
               ++minute;
               if(minute > 59){
                  minute = 0;
                  ++hour;
                  if(hour > 23){
                     hour = 0;
                  }
               }
            }
            //Afiseaza timpul pe linia 2, începând cu coloana 1;
            gotoLC(2,1); //seteaza cursorul pe linia 2, coloana 1
            putchLCD((hour / 10) ? hour / 10 + '0' : ' ');
            putchLCD(hour % 10 + '0');
            putchLCD(':');
            putchLCD(minute / 10 + '0');
            putchLCD(minute % 10 + '0');
            putchLCD(':');
            putchLCD(second / 10 + '0');
            putchLCD(second % 10 + '0');
            //sprintf(buf, "%2d:%02d:%02d", hour, minute, second);
            //putsLCD(buf);
            //Verifica încadrarea in timp:
            unsigned char TF = TIFR & 1 << OCF2;
            if(TF){
               // P9. Afiseaza mesaj de eroare si intra în bucla infinita.
               clrLCD();
               putsLCD("Eroare ceas!");
               while(1);
            }
         }
      }
   }//end while
}//end main