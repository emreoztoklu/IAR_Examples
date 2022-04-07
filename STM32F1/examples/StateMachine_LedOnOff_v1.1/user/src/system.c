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
  /*GPIOx Clocklar� Aktif edilmesi*/
  for(i = 0; i < N_PORTS; i++)
    RCC_APB2PeriphClockCmd(_GPIO_Clocks[i], ENABLE);
  
  /*Alternatif Function IO u�lar� aktif edildi*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  
  /*JTAG �zelli�inin kapat�lmas�, SWD kullan�lacak */
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

void Sys_ClockTick(void){    // Bu fonksiyon "stm32f10x_it.c" i�erisinde �agirilacak
  //elapsed time counter     // Sistem "SysTick_Handler" her tick kesmesinde tetiklenir
  ++_tmTick;                 // oda tm_tick counterini bir artirir.
}

clock_t clock(void){
  return _tmTick;
}

void Sys_ClockInit(void){
  SysTick_Config(SystemCoreClock / CLOCKS_PER_SEC);
}
