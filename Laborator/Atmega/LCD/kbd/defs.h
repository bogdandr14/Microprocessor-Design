
/*
 * defs.h
 *
 * Created: 11/04/2021 20:26:36
 *  Author: bogda
 */ 

#include <avr/io.h>

#define NOKEY 0x7f
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
char kbscan();