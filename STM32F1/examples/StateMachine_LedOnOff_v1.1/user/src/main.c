#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "io.h"

void init(void);
void Task_LED(void);

int main(){
  /*Ba�lang�� yap�land�rmalar�*/
  init();  
  
  /*G�rev �evrimi (Task Loop)*/
  /*Co-operative Multitasking (Yard�mla�mal� �oklu g�rev)*/ 
  while(1){
    
    Task_LED();
    
  }
}

void init(void){
  /* System Clock Init*/
  Sys_ClockInit(); 
  
  /* I/O portlar�na Clock verildi ve ba�lat�ld�*/
  Sys_IoInit();    

  /*LED start conditions*/
  IO_Write(IOP_LED, TURN_OFF);
  IO_Init(IOP_LED, IO_MODE_OUTPUT);
}

void Task_LED(void){ 
  static enum{
    S_LED_OFF,
    S_LED_ON,
  }state = S_LED_OFF;
  
  static clock_t t0;        //Duruma ilk ge�i� saati
  clock_t t1;               //G�ncel saat de�eri
  
  t1 = clock();
  
  switch(state) {
    case S_LED_OFF:         // state led OFF da ise ON konumna ge�irilecek
      if(t1 >= t0 + 9 * CLOCKS_PER_SEC / 10 ) {    // 9/10 saniye ge�mi�se (900ms)
        IO_Write(IOP_LED, TURN_ON);
        
        t0 = t1;
        state = S_LED_ON;
      }
      break;
      
    case S_LED_ON:          // state led ON da ise OFF konumna ge�irilecek
      if(t1 >= t0 + 1 * CLOCKS_PER_SEC / 10 ) {    // 1/10 saniye ge�mi�se (100ms)
        IO_Write(IOP_LED, TURN_OFF);
        
        t0 = t1;
        state = S_LED_OFF;
       }
      break;
  }
}
