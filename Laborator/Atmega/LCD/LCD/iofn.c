#include "defs.h"

#define E  7
#define RS 5

#define IReg 0
#define DReg 1

volatile unsigned long int delay;
void wait(unsigned long int val){
   for(delay = 0; delay <= val; ++delay);
}

void wr_LCDreg(unsigned char vRS, unsigned char data){
#define wait125   cnt8++
   static volatile unsigned char cnt8;

   if(vRS == 0){
      clrbit(PORTB, RS); //RS = 0
   }	
   else{
      setbit(PORTB, RS); //RS = 1
   }

   PORTD = data; //Write data in PORTD
   
   wait125; //Wait tAS
   
   setbit(PORTB, E); //E = 1
   
   wait125;
   wait125; //Wait tPWeh
    
   clrbit(PORTB, E); //E = 0
   
   wait125; //Wait tH
}

void initLCD(){
   // Function set
	//bit 7 6 5 4   3 2 1 0
	//    0 0 0 DL  N F - - 
	//    0 0 0  1  1 1 0 0  0x38
	wr_LCDreg(IReg, 0x38);
   wait(50 * US);
   
   // Display on/off control
	//bit 7 6 5 4  3 2 1 0
	//    0 0 0 0  1 D C B     
	//    0 0 0 0  1 1 1 1 0x0f
	wr_LCDreg(IReg, 0x0e);
   wait(50 * US);
   
   // Entry mode set
   //bit 7 6 5 4  3 2 1 0
   //    0 0 0 0  0 1 D S
   //    0 0 0 0  0 1 1 0 0x06
	wr_LCDreg(IReg, 0x06);
   wait(50 * US);

   // Clear display
   //bit 7 6 5 4  3 2 1 0
   //    0 0 0 0  0 0 0 1
   //    0 0 0 0  0 0 0 1 0x01
	wr_LCDreg(IReg, 0x01);
   wait(50 * US);
}

void putchLCD(char ch) {
   wr_LCDreg(DReg, ch);
   wait(50 * US);
}

void putsLCD(char *ch){
   while(*ch != '\0'){
      putchLCD(*ch);
      ++ch;
   }
}

void clrLCD(){
   wait(1 * S);
   wr_LCDreg(IReg, 0x01);
   wait(1 * S);
}

void gotoLC(unsigned char line, unsigned char col){
   unsigned char setDDRAM = 0x80;
   setDDRAM += col - 1;
   if(line == 2 || line == 1){
      if(line == 2){
         setDDRAM += 0x40;
      }
      wr_LCDreg(IReg, setDDRAM);
   }
   wait(50 * US);
}

void sysinit(){
   DDRD  = 0b11111111;
   PORTB = 0b00000000;
   DDRB  = 0b10100000;
   
   wait(1 * S);
   initLCD();
}

 