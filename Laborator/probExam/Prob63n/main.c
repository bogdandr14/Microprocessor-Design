/*
 * Prob63n.c
 *
 * Created: 16/06/2021 20:51:24
 * Author : bogda
 */ 

#include <avr/io.h>
#define UP 5
#define DOWN 6

#define DELAY 30
#define REF_NOW 0
#define WAIT_REF 1

unsigned int pin = 2;
unsigned char cycles = 0, sec05 = 0;
unsigned char output = 0b00100, state = REF_NOW;

int main(void)
{
   unsigned char sample_now = 0xf0, sample_ante = 0xf0;
   unsigned char loop_cnt = 0, k = 50;
   
   DDRB = 0b00011111;
   DDRD = 0xff;
   PORTB = 0b00100;
   PORTD = 0x00;
   TCCR2 = 0b01101101;
   OCR2 = 255 * 3 / 5;

    while (1)
    {
       if(loop_cnt == DELAY){
          loop_cnt = 0;
          sample_ante = sample_now;
          sample_now = PINB;
          if((sample_now & 1 << UP) && (sample_ante & 1 << UP) == 0){
             pin = (pin + 1) % 5;
             startCount();
          }          
          if((sample_now & 1 << DOWN) && (sample_ante & 1 << DOWN) == 0){
             pin = (pin + 4) % 5;
             startCount();
          }
       }
       if(state == WAIT_REF && TIFR & 1 << TOV2){
          TIFR |= 1 << TOV2;
          ++cycles;
          if(k == cycles){
             cycles = 0;
             ++sec05;
             PORTB ^= output;
             if(sec05 == 8){
                state = REF_NOW;
                OCR2 = 254 * (pin + 1) / 5;
             }
          }
       } 
       ++loop_cnt;
    }
}

void startCount(){
   state = WAIT_REF;
   output = 1 << pin;
   PORTB = output;
   cycles = 0;
   sec05 = 0;
   TCNT2 = 0;
}
unsigned char numbers[]={0b011111, //0
   0b000011, //1
   0b101101, //2
   0b100111, //3
   0b100011, //4
   0b110110, //5
   0b111110, //6
   0b010011, //7
   0b111111, //8
   0b110111, //9
   0b100000, //-
};

void display_2seg6(int n){
   if(n > 99){
      PORTA = numbers[10];
      PORTB = numbers[10];
   }
   else{
      PORTA = numbers[n / 10];
      PORTB = numbers[n % 10];
   }
}

#include <avr/io.h>

#define WAIT 0
#define COUNT 1
#define PS 0
#define PV 1
#define DELAY //se va calcula experimental
unsigned char sec = 0, state = WAIT;

int main(){
   unsigned char sn = 0x0f, sa = 0x0f, na = 0;
   
   DDRB = 0xff;
   DDRA = 0xff;
   DDRD = 0x00;
   PORTA = 0x00;
   PORTB = 0x00;
   TCCR1A=0b00000000;
   TCCR1B=0b00001100;
   OCR1A=46875 -1;
   
   while (1){
      sa = sn;
      sn = PIND;
      switch(state){
         case WAIT:
         {
            if(!(sn & 1<<PS) && (sa & 1<<PS)){
               state = COUNT;
               TCNT1 = 0;
               TIFR |= 1<<OCF1A;
               na = 0;
            }
            break;
         }
         case COUNT:{
            if((TIFR & 1<<OCF1A)){
               TIFR |= 1<<OCF1A;
               state = WAIT;
               display_2seg6(na);
            }
            else if(!(sn & 1<<PV) && (sa & 1<<PV)){
               na++;
            }
         }
      }
   }
}
