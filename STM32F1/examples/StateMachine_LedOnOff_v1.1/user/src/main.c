#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "io.h"


/* funtion prototypes */
void Task_LED(void);
void Task_LED_v2(void);



void init(void){
  /* System Clock Init*/
  Sys_ClockInit(); 
    
  /* I/O portlarýna Clock verildi ve baþlatýldý*/
  Sys_IoInit();    

  /*LED start conditions*/
  IO_Write(IOP_LED, TURN_OFF);
  IO_Init(IOP_LED, IO_MODE_OUTPUT);
}


int main(){
  /*Baþlangýç yapýlandýrmalarý*/
  init();  
  
  /*Görev çevrimi (Task Loop)*/
  /*Co-operative Multitasking (Yardýmlaþmalý çoklu görev)*/ 
  while(1){
    
    //Task_LED();           // 1.version TASK_LED
    Task_LED_v2();          // 2.version TASK_LED
    
  }
}


void Task_LED(void){ 
  static enum{
    S_LED_OFF,
    S_LED_ON,
  }state = S_LED_OFF;
  
  static clock_t t0;        //Duruma ilk geçiþ saati
  clock_t t1;               //Güncel saat deðeri
  
  t1 = clock();
  
  switch(state) {
    case S_LED_OFF:         // state led OFF da ise ON konumna geçirilecek
      if(t1 >= t0 + 9 * CLOCKS_PER_SEC / 10 ) {    // 9/10 saniye geçmiþse (900ms)
        //S_LED_ON gecis baslangici
        IO_Write(IOP_LED, TURN_ON);
        
        t0 = t1;
        state = S_LED_ON;
      }
      break;
      
    case S_LED_ON:          // state led ON da ise OFF konumna geçirilecek
      if(t1 >= t0 + 1 * CLOCKS_PER_SEC / 10 ) {    // 1/10 saniye geçmiþse (100ms)
        IO_Write(IOP_LED, TURN_OFF);
        
        t0 = t1;
        state = S_LED_OFF;
       }
      break;
  }
}


void Task_LED_v2(void){ 
  static enum {
    I_LED_OFF,          // instructer of led
    S_LED_OFF,          // state of led
    I_LED_ON,
    S_LED_ON,
  }state = I_LED_OFF;
  
  static clock_t t0;        //Duruma ilk geçiþ saati
  clock_t t1;               //Güncel saat deðeri
  
  t1 = clock();
  
  switch(state) {
    case I_LED_OFF:         // instructer of led
      t0 = t1;
      IO_Write(IOP_LED, TURN_OFF);
      state = S_LED_OFF;        
      break;
      
    case S_LED_OFF:         // state led OFF da ise ON konumna geçirilecek
      if(t1 >= t0 + 9 * CLOCKS_PER_SEC / 10 ) {    // 9/10 saniye geçmiþse (900ms)
        state = I_LED_ON;
      }
      break;
      
    case I_LED_ON:          // instructer of led
      t0 = t1;
      IO_Write(IOP_LED, TURN_ON);
      state = S_LED_ON;  
      break;   
      
    case S_LED_ON:          // state led ON da ise OFF konumna geçirilecek
      if(t1 >= t0 + 1 * CLOCKS_PER_SEC / 10 ) {    // 1/10 saniye geçmiþse (100ms)
        state = I_LED_OFF;
       }
      break;
  }
}
