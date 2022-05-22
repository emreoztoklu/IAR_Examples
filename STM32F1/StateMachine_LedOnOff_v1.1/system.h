#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "stm32f10x.h"

/*DEFİNES*/
#define CLOCKS_PER_SEC 1000


/*TYPEDEFS*/
typedef uint32_t clock_t;


/*function declerations */
clock_t clock(void);
void Sys_ClockInit(void);
void Sys_ClockTick(void);

void Sys_IoInit(void);

void __delay(uint32_t ncy);

#define CLOCK_FREQ   72000000
#define INSTR_FREQ   ((CLOCK_FREQ) *8 / 9)
#define US_CYCLES    ((INSTR_FREQ) / 1000000)
#define MS_CYCLES    ((INSTR_FREQ) / 1000)

/*function like makro*/
#define DelayUs(us)   __delay((us) * US_CYCLES)
#define DelayMs(ms)   __delay((ms) * MS_CYCLES)

#endif 