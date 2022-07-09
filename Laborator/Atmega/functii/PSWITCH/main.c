/*
 * PSWITCH.c
 *
 * Created: 25/03/2021 10:35:54
 * Author : bogda
 */ 

#include <avr/io.h>

int main(void)
{
   // memoreaza valorile semnalelor de intrare
   unsigned char inputs;

   // se asambleaza iesirile astfel:
   // bit : 7 6 5 4 3 2 1 0
   // outs= - g f e d c b a
   
   DDRA = 0b11111111;//Pinii din portul A ca iesiri
   DDRB = 0b00000000;//Pinii din portul B ca intrari
   
   while(1){
       //se forteaza bitii 7:3 la 0, pastrandu-se doar valorile bitilor 2:0
      inputs = PINB & 0b00000111;
      switch(inputs){
         case 0:
            PORTA = 0b0111111;
            break;
         case 1:
            PORTA = 0b0000110;
            break;
         case 2:
            PORTA = 0b1011011;
            break;
         case 3:
            PORTA = 0b1001111;
            break;
         case 4:
            PORTA = 0b1100110;
            break;
         case 5:
            PORTA = 0b1101101;
            break;
         case 6:
            PORTA = 0b1111101;
            break;
         case 7:
            PORTA = 0b0000111;
            break;
      }
   }
   return 0;
}

