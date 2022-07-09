/*
 * LCD2.c
 *
 * Created: 08/04/2021 10:37:00
 * Author : bogda
 */ 

#include "defs.h"
#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    sysinit();
    while(1){
       clrLCD();                    // Ce se vede pe LCD
       wait(S * 2);
       putsLCD("0123456789abcdef"); //0123456789abcdef
       wait(S * 2);                 //_
       putchLCD('g');               //0123456789abcdef
       wait(S * 2);                 //g_
       clrLCD();                    //_
       wait(S * 2);                 //
       putsLCD("0123456789abcdefg");//0123456789abcdef
       wait(S * 2);                 //g_
       putsLCD("hijklmnopqrstuv");  //0123456789abcdef
       wait(S * 2);                 //ghijklmnopqrstuv
       putchLCD('w');               //w123456789abcdef
       wait(S * 2);                 //ghijklmnopqrstuv
    }       
}

