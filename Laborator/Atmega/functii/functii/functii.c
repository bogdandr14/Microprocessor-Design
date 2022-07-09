/*
 * functii.c
 *
 * Created: 15/03/2021 23:16:17
 * Author : bogda
 */ 

#include <avr/io.h>
int main(){
   // memoreaza valorile semnalelor de intrare
   unsigned char inputs;
   
   // Variabila vxi, (i=0,1,2) memoreaza in bitul i valoarea intrarii xi.
   // Ceilalti biti sunt 0.
   unsigned char vx3, vx2, vx1, vx0;
   // in variabila outs se asambleaza iesirile astfel:
   // bit 7 6 5 4 3 2 1 0
   // outs= - - - - - f2 f1 f0
   unsigned char outs;
   unsigned char temp;
   
   DDRA = 0b11111111;//Pinii din portul A ca iesiri
   DDRB = 0b00000000;//Pinii din portul B ca intrari
   
   while(1){
      inputs = PINB; //X se înlocuieste cu A, B, C sau D;
      inputs = inputs & 0b00001111;//se forteaza bitii 7:3 la 0, pastrandu-se doar valorile bitilor 2:0
   
      vx0 = inputs & 1 << 0; // vx0 = 0000 000x0; Variabila vx0 contine numai intrarea x0 10
      vx1 = inputs >> 1 & 1 << 0; // vx1 = 0000 000x1
      vx2 = inputs >> 2 & 1 << 0; // vx2 = 0000 000x2
      vx3 = inputs >> 3 & 1 << 0; // vx2 = 0000 000x3
      
      outs = 0;
   
      temp = vx3 ^ vx2 ^ vx1 ^ vx0; // f0 se calculeaza in temp
      
      if(temp & 1 << 0) {
         outs |= 1 << 0; //seteaza bitul 0 din outs la 1 cand valoarea bitului de pe pozitia 0 din temp e 1
      }
      
      if(inputs < 3){
         outs |= 1 << 1; //seteaza bitul 1 din outs la 1 cand valoarea input este mai mic ca 3 (doar x0 si x1 pot fi 1)
      }   
      
      //temp = ((~vx2) & vx1) | (vx2 & vx0); // f2 se calculeaza in temp
      temp = (~vx3 & ~vx2 & vx1) | (((~vx2 & vx1) | (~vx3 & vx2) | (vx2 & ~vx1)) & vx0);
      if(temp & 1 << 0){
      //if((vx0 & (vx2 | vx1) << 0) || (inputs == 2)){
      //if((inputs & 1 << 0) && inputs > 1 || inputs == 2){
         outs |= 1 << 2; // seteaza bitul 2 din outs la 1 cand valoarea din input este numar prim
      }      
      PORTA = outs;
   }   
   return 0;
}