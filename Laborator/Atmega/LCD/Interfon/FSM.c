#include "defs.h"

#define FMSG   0
#define WAIT   1
#define RING   2
#define OPEN   3
#define COD1   4
#define COD2   5
#define COD3   6
#define ERR    7

int main(){

   char code_now= NOKEY, code_ante;
   unsigned char kbhit=0;
   unsigned char kbcode = '0';
   unsigned char cmd;
   
   unsigned char loop_cnt=0;
   
   char cod[4]="123";
   char buf[4];
   char stare = FMSG;
      
   sysinit();
   
   while (1){
      //determina daca este tasta apasata
      if(loop_cnt==100){
         code_ante=code_now;
         code_now = kbscan();
         if(code_now!=NOKEY && code_ante==NOKEY){
            kbhit=1;
            kbcode=code_now;
         }
         loop_cnt=0;
      }
      
      //SFSM
      switch (stare){
         case FMSG:
            clrLCD();
            putsLCD("Suna la 1..8,");
            gotoLC(2,1);
            putsLCD("C pentru cod:");
            stare = WAIT;
            break;
         
         case WAIT:
            if(kbhit){
               kbhit=0;
               if ('C'==kbcode){
                  clrLCD();
                  putsLCD("Cod=");
                  stare = COD1;
               }
               if (kbcode>='1' && kbcode <= '8'){
                  clrLCD();
                  putsLCD("Sun la ");
                  putchLCD(kbcode);                  
                  stare = RING;
               }
            }               
            break;
            
         case RING:            
            for(int i = 0; i < 15; ++i){
               clrLCD();
               putsLCD("Sun la ");
               putchLCD(kbcode);
               for(int k = 0; k < i % 4; ++k){
                  putchLCD('.');
               }
               cmd = PINC >> (kbcode - '1') & 1 << 0;
               if(cmd & 1 << 0){
                  stare = OPEN;
                  break;
               }
               wait(1 * S);
            }
            if(stare != OPEN){
               stare = FMSG;
            }
            break;
            
         case COD1:
            if(kbhit){
               kbhit=0;
               if (kbcode>='0' && kbcode <= '9'){
                  buf[0]=kbcode;
                  putchLCD('*');
                  stare = COD2;               
               }
               else{
                  stare = FMSG;
               }
            }
            break;
            
         case COD2: 
            if(kbhit){
               kbhit=0;
               if (kbcode>='0' && kbcode <= '9'){
                  buf[1]=kbcode;
                  putchLCD('*');
                  stare = COD3;
               }
               else{
                  stare = FMSG;
               }
            }
            break;
            
         case COD3: 
            if(kbhit){
               kbhit=0;
               if (kbcode>='0' && kbcode <= '9'){
                  buf[2]=kbcode;
                  putchLCD('*');
                  for(int i = 0; i < 3; ++i){
                     if(buf[i] != cod[i]){
                        stare = ERR;
                     }
                  }
                  if(stare != ERR){
                     stare = OPEN;
                  }
               }
               else{
                  stare = ERR;
               }
            }
            break;
            
         case OPEN: 
            clrLCD();
            putsLCD("Deschis!");
            wait(5 * S);
            stare = FMSG;
            break;
            
         case ERR: 
            //asteapta  5 secunde
            clrLCD();
            putsLCD("Cod invalid");
            wait(5 * S);
            stare = FMSG;
            break;
            
      }//end switch
      loop_cnt++;
   }//end while
}//end main
