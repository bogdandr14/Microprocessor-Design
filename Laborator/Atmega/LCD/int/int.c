#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define C1 1
#define C2 2
#define C3 3
#define IFLAG 7

//Pentru bonus
volatile unsigned char finished = 1; //Folosit pentru excluderea mutuala dintre afisarea 
//procentului intensitatii ledului si afisarea pasului curent a variabilei a

char buf_a[4]; //Folosit pentru stocarea si afisarea valorii curente a variabilei a sub
//forma de char array 

volatile unsigned char kbhit=0;
volatile char kbcode = 0;
int lumi=50;  //luminozitatea
char buf_lumi[4];

int main(){
   unsigned char stare = C1;
   char buf_pit[4];
   unsigned long int a, b, c;
   unsigned char isPyth = 0;

   sysinit();
   
   // Fast PWM => WGM20 = 1, WGM21 = 1
   // p = 256 avem f_cyle ~= 122Hz => CS22 = 1, CS21 = 0, CS20 = 0
   // COM21 = 1, COM20 = 0
   //   7    6     5     4     3     2    1    0
   // FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
   //   0    1     1     0     1     1    0    0
   TCCR0 = 0b01101100;
   
   //Se seteaza bitul de TOIE pentru Timer/Counter0 Overflow Interrupt Enable
   TIMSK |= (1 << TOIE0);
   
   //Se seteaza bitul de intrerupere mascabila I
   setbit(SREG, IFLAG);
   
   OCR0 = lumi;
   
   DDRB |= 1 << 3;
   
   while(1)
   {
      //c pitagoreic start
      if( kbhit && kbcode >= '0' && kbcode <= '9')
      {
         kbhit = 0;

         switch(stare){
            case C1:   //cifra 0
            gotoLC(1, 1);
            putsLCD("                ");
            gotoLC(1, 1);
            buf_pit[0] = kbcode;
            putchLCD(kbcode);
            stare = C2;
            break;

            case C2:
            gotoLC(1, 2);
            buf_pit[1] = kbcode;
            putchLCD(kbcode);
            stare = C3;
            break;
            
            case C3:
            gotoLC(1, 3);
            buf_pit[2] = kbcode;
            buf_pit[3] = '\0';
            putchLCD(kbcode);
            gotoLC(1, 8);
            putsLCD("Busy...");
            
            //calculeaza c
            c = ((buf_pit[0] - '0') * 100 + (buf_pit[1] - '0') * 10 + (buf_pit[2] - '0'));
            isPyth = 0;
            for(a = 1; a < c; ++a)
            {
               
               //Daca nu folosim excluderea mutuala, sunt sanse ca dupa ce ne mutam
               //cu cursorul la pozitia 1 randul 5, sa apuce sa se scrie valoarea lui L, 
               //caz in care cursorul se afla dupa valoarea intensitatii led-ului, 
               //unde se va scrie valoarea lui a 
               
               while(!finished); //Cat timp se afiseaza L=..., se va astepta
               finished = 0 ;//Se notifica faptul ca se va scrie a
               gotoLC(1, 5);
               itoa(a, buf_a, 10); //Facem conversie de la int la char
               putsLCD(buf_a); //Scriem valoarea lui a
               finished = 1; //Se notifica faptul ca s-a terminat scrierea valorii a

               for(b = 1; b <= a; ++b)
               {
                  if(c * c <= a * a + b * b)
                  {
                     if(c * c == a * a + b * b)
                     {
                        isPyth = 1;
                        goto show;
                     }else
                     {
                        break;
                     }
                  }
                  
               }
            }
            show:
            gotoLC(1,8);
            if(isPyth)
            {
               putsLCD(" PYTH   ");
            }
            else
            {
               putsLCD(" nonPYTH");
            }
            stare=C1;
            break;
         }//end switch
      }// end c pitagoreic
   }//end while
}//end main

ISR(TIMER0_OVF_vect)
{
   static unsigned char code_now= NOKEY, code_ante;
   static unsigned char loop_cnt = 0;
   
   if(loop_cnt == 4){
      loop_cnt = 0;
      code_ante = code_now;
      code_now = kbscan();
      if( code_ante == NOKEY && code_now != NOKEY){
         kbhit = 1;
         kbcode = code_now;
      }
   }
   
   //PWM start
   if(kbhit  && (kbcode == 'C' || kbcode == 'D'))
   {
      kbhit = 0;
      if(kbcode == 'C')
      {
         lumi = lumi + 5 > 100 ? 100 : lumi + 5;//...creste luminozitatea
      }
      if(kbcode == 'D')
      {
         lumi = lumi - 5 < 0 ? 0 : lumi - 5;//...scade luminozitatea
      }
      OCR0 = lumi == 100 ? 254 : (255 * lumi) / 100;
      
      while(!finished); //Cat timp se scrie valoare lui a, se asteapta
      finished = 0; //Se notifica faptul ca se scrie valoarea intensitatii led-ului
      gotoLC(2, 1);
      putsLCD("L=");
      itoa(lumi, buf_lumi, 10);
      putsLCD(buf_lumi);
      putsLCD("%  " );
      finished = 1; //Se notifica faptul ca s-a terminat scrierea valorii intensitatii led-ului
   }
   //PWM end
   
   loop_cnt++;
}