#include <avr/io.h>

#define F_CPU 8000000UL
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

//period
#define T 10UL
//duty factor
#define DF 50UL
#define THIGH (T*DF/100)
#define TLOW (T*(100-DF)/100)

#define DFHIGH 65UL
#define DFLOW 25UL
#define THIGH1 (T*DFHIGH/100)
#define TLOW1 (T*(100-DFHIGH)/100)
#define THIGH2 (T*DFLOW/100)
#define TLOW2 (T*(100-DFLOW)/100)

int main(){
   DDRA=0xff;
   
   while(1){
      for(int i = 100; i > 0; --i){
         PORTA=0b00000001; //turn on LED
         _delay_ms(THIGH1);
         PORTA=0; //turn off LED
         _delay_ms(TLOW1);
      }
      
      for(int i = 100; i > 0; --i){
         PORTA=0b00000001; //turn on LED
         _delay_ms(THIGH2);
         PORTA=0; //turn off LED
         _delay_ms(TLOW2);
      }

   }
}