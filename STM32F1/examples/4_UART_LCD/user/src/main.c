#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "io.h"
#include "lcdm.h"
#include "uart.h"

/* funtion prototypes */
void Task_LED(void);
void Task_printf(void);

void init(void){
  /* System Clock Init*/
  Sys_ClockInit(); 
  
  /* I/O portlarýna Clock verildi ve baþlatýldý*/
  Sys_IoInit();
  
  /*Console yazma*/
  Sys_ConsoleInit();
  
  /*UART1 init*/
  UART_Init(UART_1, 115200);

  /*LED start conditions*/
  IO_Write(IOP_LED, TURN_OFF);
  IO_Init(IOP_LED, IO_MODE_OUTPUT);
}



int main(){
  int ch;
  
  /*Baþlangýç yapýlandýrmalarý*/
  init();
  
  printf("Merhaba\n");      // sending the text LCD
  
  
  /*Görev çevrimi (Task Loop)*/
  /*Co-operative Multitasking (Yardýmlaþmalý çoklu görev)*/ 
  while(1){
    
    Task_LED();
    Task_printf();
    
    if(UART_DataReady(UART_1)){
      ch = UART_Recv(UART_1);
    
      
      if (ch < 0)
        printf("\rHATA:%d\n", ch);
      else
        printf("\r%c\n", ch);

    }
  }/*while loop*/
}


void Task_LED(void){ 
  static enum{
    S_LED_OFF,
    S_LED_ON,
  }state = S_LED_OFF;
  
  static clock_t t0;        //Duruma ilk geçis saati
  clock_t t1;               //Güncel saat degeri
  
  t1 = clock();
  
  switch(state) {
    case S_LED_OFF:         // state led OFF da ise ON konumna geçirilecek
      if(t1 >= t0 + 9 * CLOCKS_PER_SEC / 10 ) {    // 9/10 saniye geçmisse (900ms)
        //S_LED_ON gecis baslangici
        IO_Write(IOP_LED, TURN_ON);
        
        t0 = t1;
        state = S_LED_ON;
      }
      break;
      
    case S_LED_ON:          // state led ON da ise OFF konumna geçirilecek
      if(t1 >= t0 + 1 * CLOCKS_PER_SEC / 10 ) {    // 1/10 saniye geçmisse (100ms)
        IO_Write(IOP_LED, TURN_OFF);
        
        t0 = t1;
        state = S_LED_OFF;
       }
      break;
  }
}
       
void Task_printf(void){
  static int count;
  
  printf("Sayi=%10lu\n",(int)(++count/1000));
  UART_printf(UART_1, "Count=%10lu\n",(int)(++count/1000));
}