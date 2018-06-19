/////////////////////////////////////
//  Generated Initialization File  //
/////////////////////////////////////
#include "compiler_defs.h"
#include "C8051F300_defs.h"

// Peripheral specific initialization functions,
// Called from the Init_Device() function
void PCA_Init()
{
    PCA0MD    &= ~0x40;
    PCA0MD    = 0x00;
}

void Timer_Init()
{
	  TCON      = 0x40;
    TMOD      = 0x20;
    CKCON     = 0x10;
    TH1       = 0xCB;
}

void UART_Init()
{
    SCON0     = 0x30;
}
//sfr  P0SKIP       =  0xD4;
void Port_IO_Init()
{
    // P0.0  -  Skipped,     Open-Drain, Digital
    // P0.1  -  Unassigned,  Push-Pull,  Digital
    // P0.2  -  Unassigned,  Push-Pull,  Digital
    // P0.3  -  Unassigned,  Push-Pull,  Digital
    // P0.4  -  TX0 (UART0), Open-Drain, Digital
    // P0.5  -  RX0 (UART0), Open-Drain, Digital
    // P0.6  -  Skipped,     Open-Drain, Digital
    // P0.7  -  Unassigned,  Open-Drain, Digital
	  P0MDOUT   = 0x0E;
    XBR0      = 0x41;
    XBR1      = 0x03;
    XBR2      = 0x40;

}

void Oscillator_Init()
{
	OSCICN    = 0x06; //12.25m
}

void Interrupts_Init()
{
    IP        = 0x10;
    IE        = 0x90;
}
// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
    PCA_Init();
    Timer_Init();
    UART_Init();
    Port_IO_Init();
    Oscillator_Init();
    Interrupts_Init();  
}
