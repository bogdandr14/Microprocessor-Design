/*
 * probExam.c
 *
 * Created: 15/06/2021 19:54:55
 * Author : bogda
 */ 

#include <avr/io.h>
#include<avr/interrupt.h>
#define DELAY 30
volatile unsigned char cfg_no = 0, t_sec = 0;
volatile unsigned char k = 0;

void config(unsigned char cfg_no){
   switch(cfg_no){
      case 0:
         PORTA = 0b11101110;
         PORTB = 0b11111011;
         break;
      case 1:
         PORTA = 0b11101101;
         PORTB = 0b11111101;
         break;
      case 2:
         PORTA = 0b11101011;
         PORTB = 0b11111110;
         break;
      case 3:
         PORTA = 0b11000111;
         PORTB = 0b11111111;
         break;
   }
}

int main(void)
{
   unsigned char btn_now = 0, btn_ante = 0;
   unsigned char loop_cnt = 0;
   PORTB = 0xFF;
   PORTA = 0x7F;
   DDRB = 0xFF;
   DDRA = 0x7F;
   TCCR0 = 0b00001101;
   OCR0 = 250 - 1;
   sei();
   TIMSK |= 1 << OCIE0;
   config(cfg_no);

   while (1) {
      if(loop_cnt == DELAY){
         loop_cnt = 0;
         btn_ante = btn_now;
         btn_now = PINA;
         if((btn_now & 1<<7) != 0 && (btn_ante & 1<<7) == 0){
            cli();
            config(0);
            cfg_no = 0;
            k = 0;
            t_sec = 0;
            TCNT0 = 0;
            sei();
         }
      }
      loop_cnt++;
   }
   return 0;
}

ISR(TIMER0_COMP_vect){
   /*static unsigned char btn_now = 0, btn_ante = 0;
   btn_ante = btn_now;
   btn_now = PINA;
   if((btn_now & 1<<7) != 0 && (btn_ante & 1<<7) == 0){
      config(0);
      cfg_no = 0;
      k = 0;
      t_sec = 0;
      TCNT0 = 0;
   }
   else{*/
      k++;
      if(k == 24){
         k = 0;
         t_sec++;
         if(t_sec == 30){
            t_sec = 0;
            cfg_no++;
            if(cfg_no == 4){
               cfg_no = 0;
            }
            config(cfg_no);
         }
      }
   //}
}