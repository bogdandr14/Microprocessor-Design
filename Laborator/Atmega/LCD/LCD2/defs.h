
/*
 * defs.h
 *
 * Created: 08/04/2021 10:38:40
 *  Author: bogda
 */ 

#include <avr/io.h>

#define S  250000UL
#define MS 250UL
#define US 25UL/100UL

#define clrbit(var,bit) var &= ~(1<<bit)
#define setbit(var,bit) var |= 1<<bit

void wait(unsigned long int val);
void sysinit();
void putchLCD(char ch);
void putsLCD(char *ch);
void clrLCD();
void gotoLC(unsigned char line, unsigned char col);
