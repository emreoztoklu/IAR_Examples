/*
  #define SET_BIT(REG, BIT)     ((REG) |= (BIT))
  #define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
  #define READ_BIT(REG, BIT)    ((REG) & (BIT))
  #define CLEAR_REG(REG)        ((REG) = (0x0))
  #define WRITE_REG(REG, VAL)   ((REG) = (VAL))
  #define READ_REG(REG)         ((REG))
  #define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
*/
#include <stdio.h>
#include <stdint.h>

#include "stm32f10x.h"

#define _IOS_           // IO_PIN _ios dizi koþullu compiler ile sadece bu .c dosysý içinde tanýmlý olcak
#include "io.h"


void IO_Init(uint8_t idx, uint8_t mode){
  GPIO_InitTypeDef ioInit;
  uint32_t port;
  
  ioInit.GPIO_Mode  = (GPIOMode_TypeDef)mode;
  ioInit.GPIO_Speed = GPIO_Speed_50MHz;
  ioInit.GPIO_Pin   = (1 << _ios[idx].pin);
  
  port = _ios[idx].port;
  GPIO_Init(_GPIO_Ports[port], &ioInit);
}


void IO_Write(uint8_t idx, uint8_t val){
  uint32_t port;
  port = _ios[idx].port;
  /*
  if(val)
    _GPIO_Ports[port]->BSRR = (1 << _ios[idx].pin);
  else
   _GPIO_Ports[port]->BRR = (1 << _ios[idx].pin);
  */
  switch(val){
    case TURN_ON:
      _GPIO_Ports[port]->BRR = (1 << _ios[idx].pin);
      break;
    case TURN_OFF:
      _GPIO_Ports[port]->BSRR = (1 << _ios[idx].pin);
      break;
  };
}

uint8_t  IO_Read(uint8_t idx){
  uint32_t port;
  port = _ios[idx].port;
  
  return (_GPIO_Ports[port]->IDR & (1 << _ios[idx].pin)) != 0;
}





