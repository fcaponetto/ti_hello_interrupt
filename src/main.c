#include <stdio.h>
#include "init.h"
#include <stdbool.h>

bool blink = false;

//
// cpu_timer0_isr -
//
__interrupt void
cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    printf("Hello World! %lu \r\n", CpuTimer0.InterruptCount);

    if(blink)
    {
        GpioDataRegs.GPBDAT.bit.GPIO34 = 0;
//        GpioDataRegs.GPBDAT.bit.GPIO39 = 1;
    }
    else
    {
        GpioDataRegs.GPBDAT.bit.GPIO34 = 1;
//        GpioDataRegs.GPBDAT.bit.GPIO39 = 0;
    }

    blink = !blink;

    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//
// Main
//
void main(void)
{

    init_system();
    init_cpu_timer(&cpu_timer0_isr);

    //
    // Initialize GPIOs for the LEDs and turn them off
    //
    EALLOW;
    GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO39 = 1;
    GpioDataRegs.GPBDAT.bit.GPIO34 = 1; // red
    GpioDataRegs.GPBDAT.bit.GPIO39 = 1; // blue
    EDIS;

    //
    // Main program loop - IDLE loop. Just sit and loop forever
    //
    for(;;)
    {
    }
}

//
// End of File
//

