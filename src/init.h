#ifndef _INIT_H_
#define _INIT_H_

#include "DSP28x_Project.h"     // DSP28x Headerfile

//////
////// Globals
//////
extern void DSP28x_usDelay(Uint32 Count);

//
// scia_init - SCIA  8-bit word, baud rate 0x000F, default, 1 STOP bit,
// no parity
//
void scia_init();

void redirect_output();

void init_system();

void init_cpu_timer(void(*isr_func)(void));

#endif
