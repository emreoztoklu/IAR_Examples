#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "io.h"
#include "lcdm.h"

/* funtion prototypes */
void Task_LED(void);
void Task_printf(void);

void init(void){
  /* System Clock Init*/
  Sys_ClockInit(); 
  
  /* I/O portlar�na Clock verildi ve ba�lat�ld�*/
  Sys_IoInit();
  
  /*Console yazma*/
  Sys_ConsoleInit();

  /*LED start conditions*/
  IO_Write(IOP_LED, TURN_OFF);
  IO_Init(IOP_LED, IO_MODE_OUTPUT);
}



int main(){
  
  /*Ba�lang�� yap�land�rmalar�*/
  init();
  
  LCD_DisplayOn(LCD_MODE_ON | LCD_MODE_CURSOR );
  /*
  LCD_SetCursor(0x00);      //0x04 1.satir 4. sut�n adresi 0x04
  LCD_PutChar('T');
  LCD_PutChar('E');
  LCD_PutChar('S');
  LCD_PutChar('T');
  LCD_SetCursor(0x40);      // 2.satir basi adresi 0x40
  LCD_PutChar('T');
  LCD_PutChar('E');
  LCD_PutChar('S');
  LCD_PutChar('T');
  LCD_PutChar('2');   
 */
  
  printf("Merhaba\n");
  
  
  /*G�rev �evrimi (Task Loop)*/
  /*Co-operative Multitasking (Yard�mla�mal� �oklu g�rev)*/ 
  while(1){
    
    Task_LED();
    Task_printf();
    
  }
  
}


void Task_LED(void){ 
  static enum{
    S_LED_OFF,
    S_LED_ON,
  }state = S_LED_OFF;
  
  static clock_t t0;        //Duruma ilk ge�is saati
  clock_t t1;               //G�ncel saat degeri
  
  t1 = clock();
  
  switch(state) {
    case S_LED_OFF:         // state led OFF da ise ON konumna ge�irilecek
      if(t1 >= t0 + 9 * CLOCKS_PER_SEC / 10 ) {    // 9/10 saniye ge�misse (900ms)
        //S_LED_ON gecis baslangici
        IO_Write(IOP_LED, TURN_ON);
        
        t0 = t1;
        state = S_LED_ON;
      }
      break;
      
    case S_LED_ON:          // state led ON da ise OFF konumna ge�irilecek
      if(t1 >= t0 + 1 * CLOCKS_PER_SEC / 10 ) {    // 1/10 saniye ge�misse (100ms)
        IO_Write(IOP_LED, TURN_OFF);
        
        t0 = t1;
        state = S_LED_OFF;
       }
      break;
  }
}


void Task_printf(void){
  static int count;
  
  printf("Sayi=%d\r\n",(int)(++count/1000));
}