#include <stdio.h>
#include <stdint.h>

#include "system.h"
#include "io.h"

void init(void){
  /* System Clock Init*/
  Sys_ClockInit(); 
  
  /* I/O portlarýna Clock verildi ve baþlatýldý*/
  Sys_IoInit();    

  /*LED start conditions*/
  IO_Write(IOP_LED, TURN_OFF);
  IO_Init(IOP_LED, IO_MODE_OUTPUT);
}

int read_pin;

int main(){
  
  /*Baþlangýç yapýlandýrmalarý*/
  init();
  
  IO_Write(IOP_LED, TURN_ON);
  read_pin = IO_Read(IOP_LED);
  
  IO_Write(IOP_LED, TURN_OFF);
  read_pin = IO_Read(IOP_LED);
  
  IO_Write(IOP_LED, TURN_ON);
  IO_Write(IOP_LED, TURN_OFF);
  
  // GPIOB<5> Çýkýþ-1 yapmak istendiðinde
  IO_Init(IOP_TEST, IO_MODE_OUTPUT);
  IO_Write(IOP_TEST, TURN_OFF);
 
  // GPIOA<3> input yani giriþ yapýlsýn sonra oku
  IO_Init(IOP_INPUT, IO_MODE_INPUT);
  read_pin = IO_Read(IOP_INPUT);
  
  
  /*Görev çevrimi (Task Loop)*/
  /*Co-operative Multitasking (Yardýmlaþmalý çoklu görev)*/ 
  while(1){
    
  }
  
}

