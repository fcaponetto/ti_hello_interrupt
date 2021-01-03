/*
 * init.c
 *
 *  Created on: Jun 14, 2020
 *      Author: nando
 */
// Included Files
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <file.h>

#include "sci_io.h"
#include "init.h"



//
// scia_init - SCIA  8-bit word, baud rate 0x000F, default, 1 STOP bit,
// no parity
//
void scia_init()
{
    //
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    //

    //
    // 1 stop bit,  No loopback, No parity,8 char bits, async mode,
    // idle-line protocol
    //
    SciaRegs.SCICCR.all =0x0007;

    //
    // enable TX, RX, internal SCICLK, Disable RX ERR, SLEEP, TXWAKE
    //
    SciaRegs.SCICTL1.all =0x0003;

    SciaRegs.SCICTL2.bit.TXINTENA =1;
    SciaRegs.SCICTL2.bit.RXBKINTENA =1;

    //
    // 115200 baud @LSPCLK = 22.5MHz (90 MHz SYSCLK).
    //
    SciaRegs.SCIHBAUD    =0x0000;

    SciaRegs.SCILBAUD    =0x0017;

    SciaRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset

    return;
}

void redirect_output()
{
    volatile FILE *fid;

    //
    // Redirect STDOUT to SCI
    //
    add_device("scia", _SSA, SCI_open, SCI_close, SCI_read, SCI_write,
                        SCI_lseek, SCI_unlink, SCI_rename);
    fid = fopen("scia","w");
    freopen("scia:", "w", stdout);
    setvbuf(stdout, NULL, _IONBF, 0);

}


void init_system()
{
    //
    // If running from flash copy RAM only functions to RAM
    //
#ifdef _FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
#endif

    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2806x_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Initalize GPIO:
    // This example function is found in the F2806x_Gpio.c file and
    // illustrates how to set the GPIO to its default state.
    //
    // InitGpio(); Skipped for this example

    //
    // For this example, only init the pins for the SCI-A port.
    // This function is found in the F2806x_Sci.c file.
    //
    InitSciaGpio();

    //
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the F2806x_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2806x_DefaultIsr.c.
    // This function is found in F2806x_PieVect.c.
    //
    InitPieVectTable();

    //
    // Initialize SCIA
    //
    scia_init();

    //
    // Redirect STDOUT to SCI
    //
    redirect_output();
}

void init_cpu_timer(void(*isr_func)(void))
{
    //
        // Interrupts that are used in this example are re-mapped to
        // ISR functions found within this file.
        //
        EALLOW;  // This is needed to write to EALLOW protected registers
//        PieVectTable.TINT0 = &cpu_timer0_isr;
        PieVectTable.TINT0 = isr_func;
        EDIS;    // This is needed to disable write to EALLOW protected registers

        //
        // Step 4. Initialize the Device Peripheral. This function can be
        //         found in F2806x_CpuTimers.c
        //
        InitCpuTimers();   // For this example, only initialize the Cpu Timers

        //
        // Configure CPU-Timer 0 to interrupt every second:
        // 80MHz CPU Freq, 1 second Period (in uSeconds)
        //
        ConfigCpuTimer(&CpuTimer0, 90, 1000000);

        //
        // To ensure precise timing, use write-only instructions to write to the
        // entire register. Therefore, if any of the configuration bits are changed
        // in ConfigCpuTimer and InitCpuTimers (in F2806x_CpuTimers.h), the below
        // settings must also be updated.
        //

        //
        // Use write-only instruction to set TSS bit = 0
        //
        CpuTimer0Regs.TCR.all = 0x4000;

        //
        // Step 5. User specific code, enable interrupts
        //

        //
        // Enable CPU int1 which is connected to CPU-Timer 0
        //
        IER |= M_INT1;

        //
        // Enable TINT0 in the PIE: Group 1 interrupt 7
        //
        PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

        //
        // Enable global Interrupts and higher priority real-time debug events:
        //
        EINT;   // Enable Global interrupt INTM
        ERTM;   // Enable Global realtime interrupt DBGM
}


