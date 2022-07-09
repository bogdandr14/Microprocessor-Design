/*
 * probExam.c
 *
 * Created: 15/06/2021 19:54:55
 * Author : bogda
 */ 

#include <avr/io.h>
#include<stdio.h>
#include<avr/interrupt.h>
#define MIN10 0 
#define MIN 1
#define SEC10 2
#define START 3
#define CANCEL 4

#define TSELECT 0
#define RUN 1
#define DELAY 130 //se va calcula experimental

unsigned char buf[5];
volatile unsigned char state = TSELECT, k = 0;
volatile int t_sec = 0;

int main(void)
{
   unsigned char min10 = 0, min = 0, sec10 = 0;
   unsigned char loop_cnt = 0;
   unsigned char sample_ante = 0, sample_now = 0;
   sysinit();
   TCCR0 = 0b00001101;
   OCR0 = 250 - 1;
   sei();
   gotoLC(1,1);
   putsLCD("00:00");
   while (1) {
      if(loop_cnt == DELAY){
         loop_cnt = 0;
         sample_ante = sample_now;
         sample_now = PINA;
         switch(state){
            case TSELECT:
               if((sample_now >> MIN10 & 1 << 0) != 0 && (sample_ante >> MIN10 & 1 << 0) == 0){
                  min10 = (min10 + 1) % 10;
               }
               if((sample_now >> MIN & 1 << 0) != 0 && (sample_ante >> MIN & 1 << 0) == 0){
                  min = (min + 1) % 10;
               }
               if((sample_now >> SEC10 & 1 << 0) != 0 && (sample_ante >> SEC10 & 1 << 0) == 0){
                  sec10 = (sec10 + 1) % 6;
               }
               if((sample_now >>START & 1<< 0) != 0 && (sample_ante >> START & 1 << 0) == 0){
                  state = RUN;
                  t_sec = min10 * 600 + min * 60 + sec10 * 10;
                  min10 = min = sec10 = 0;
                  PORTB |= 1 << 0;
                  TIFR |= 1 << OCF0;
                  TIMSK |= 1<< OCIE0;
                  k = 0;
               }
               sprintf(buf, "%d%d:%d0", min10,min, sec10);
               gotoLC(1,1);
               putsLCD(buf);
               break;
            
            case RUN:
               if((sample_now >> CANCEL & 1<< 0) != 0 && (sample_ante >> CANCEL & 1 << 0) == 0){
                  state = TSELECT;
                  PORTB &= ~(1 << 0);
                  TIMSK &= ~(1<< OCIE0);
                  t_sec = 0;
               }
               break;
         }
      }
      loop_cnt++;
   }
   return 0;
}

ISR(TIMER0_COMP_vect){
   k++;
   if(k == 18){
      k = 0;
      t_sec--;
      if(t_sec == 0){
         t_sec = 0;
         state = TSELECT;
         PORTB &= ~(1 << 0);
         TIMSK &= ~(1<< OCIE0);
      }
       sprintf(buf, "%1d%1d:%1d%1d", t_sec / 600, t_sec / 60 % 10, t_sec / 10 % 6, t_sec % 10);
       gotoLC(1,1);
       putsLCD(buf);
   }
}