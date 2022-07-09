/*
 * LUT_RAM.c
 *
 * Created: 25/03/2021 10:10:07
 * Author : bogda
 */ 

#include <avr/io.h>

const unsigned char LUR_RAM[]={
   0b0111111, //0
   0b0000110, //1
   0b1011011, //2
   0b1001111, //3
   //------------
   0b1100110, //4
   0b1101101, //5
   0b1111101, //6
   0b0000111 //7
};

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
      //se sciu la iesire valoarea data de vectorul LUT_RAM
      PORTA = LUR_RAM[inputs];
   }
   return 0;
}

