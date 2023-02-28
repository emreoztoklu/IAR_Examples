/*
  #define SET_BIT(REG, BIT)     ((REG) |= (BIT))
  #define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
  #define READ_BIT(REG, BIT)    ((REG) & (BIT))
  #define CLEAR_REG(REG)        ((REG) = (0x0))
  #define WRITE_REG(REG, VAL)   ((REG) = (VAL))
  #define READ_REG(REG)         ((REG))
  #define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
*/

#define _IOS_           // IO_PIN _ios dizi koþullu compiler ile sadece bu .c dosysý içinde tanýmlý olcak
#include "io.h"


void IO_Init(IO_IDX idx, IO_MODE mode){
  GPIO_InitTypeDef ioInit;
  uint32_t port;
  
  ioInit.GPIO_Pin   = (1 << _ios[idx].pin);
  ioInit.GPIO_Mode  = (GPIOMode_TypeDef)mode;
  ioInit.GPIO_Speed = GPIO_Speed_50MHz;
 
  
  port = _ios[idx].port;
  GPIO_Init(_GPIO_Ports[port], &ioInit);
}


void IO_Write(IO_IDX idx, uint8_t val){
  uint32_t port;
  port = _ios[idx].port;
 
  if(val)
    _GPIO_Ports[port]->BSRR = (1 << _ios[idx].pin);
  else
   _GPIO_Ports[port]->BRR = (1 << _ios[idx].pin);

}

uint8_t  IO_Read(IO_IDX idx){
  uint32_t port;
  port = _ios[idx].port;
  
  return (_GPIO_Ports[port]->IDR & (1 << _ios[idx].pin)) != 0;
}





