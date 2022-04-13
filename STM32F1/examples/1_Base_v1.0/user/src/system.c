#include <stdio.h>
#include <stdint.h>

#include "system.h"

#define size(x)  (sizeof(x) / sizeof(x[0]))

static volatile clock_t _tmTick;

static const int _GPIO_Clocks[] ={
  RCC_APB2Periph_GPIOA,
  RCC_APB2Periph_GPIOB,
  RCC_APB2Periph_GPIOC,
  RCC_APB2Periph_GPIOD,
  RCC_APB2Periph_GPIOE,
  RCC_APB2Periph_GPIOF,
  RCC_APB2Periph_GPIOG,
}; 

#define N_PORTS  (sizeof(_GPIO_Clocks)/sizeof(int))

void Sys_IoInit(void){
  int i;
  /*GPIOx Clocklarý Aktif edilmesi*/
  for(i = 0; i < N_PORTS; i++)
    RCC_APB2PeriphClockCmd(_GPIO_Clocks[i], ENABLE);
  
  /*Alternatif Function IO uçlarý aktif edildi*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  /*JTAG özelliðinin kapatýlmasý, SWD kullanýlacak */
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

void Sys_ClockTick(void){
  //elapsed time counter
  ++_tmTick;
}

clock_t clock(void){
  return _tmTick;
}

void Sys_ClockInit(void){
  SysTick_Config(SystemCoreClock / CLOCKS_PER_SEC);
}