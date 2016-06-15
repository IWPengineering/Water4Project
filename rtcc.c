/*
 * File:   rtcc.c
 * Author: KSK0419
 *
 * Created on June 15, 2016, 1:42 PM
 */


#include "xc.h"
#include "rtcc.h"

void __attribute__ ((interrupt, auto_psv)) _RTCCInterrupt(void)
{
    _RTCIF = 0; // Clear the interrupt flag
    // Go back to wherever we were executing from - the goal here is
    //  just to sleep for some time
}


void init_sleep(void)
{
    // Follows example for how to set RTCWREN bit.
    //  ref pg 186.
    asm volatile ("push w7");
    asm volatile ("push w8");
    asm volatile ("disi #5");
    asm volatile ("mov #0x55, w7");
    asm volatile ("mov w7, _NVMKEY");
    asm volatile ("mov #0xAA, w8");
    asm volatile ("mov w8, _NVMKEY");
    asm volatile ("bset _RCFGCAL, #13"); //set the RTCWREN bit
    asm volatile ("pop w8");
    asm volatile ("pop w7");

    // Configure the RTCC alarm to wake us up @ specified intervals
    RCFGCAL  = 0x2200;
    RTCPWCbits.RTCCLK = 0b01;
    ALCFGRPTbits.CHIME = 0;
    ALCFGRPTbits.ALRMEN = 0;
    RCFGCALbits.RTCEN = 1;
    RCFGCALbits.RTCWREN = 0;
    
    /* Set interrupt priority to lowest available while still being enabled
     Note: This is important! If the interrupt is of a higher or equal
     priority to a CPU interrupt, then the device will generate either
     a hard fault or address fault on wakeup from sleep.
     */
    _RTCIP = 0b001;
    IPC15bits.RTCIP = 1;
    IFS3bits.RTCIF = 0;
    IEC3bits.RTCIE = 1;
}

void sleep_for_period(sleepLength_t length)
{
    // Follows example for how to set RTCWREN bit.
    //  ref pg 186.
    asm volatile ("push w7");
    asm volatile ("push w8");
    asm volatile ("disi #5");
    asm volatile ("mov #0x55, w7");
    asm volatile ("mov w7, _NVMKEY");
    asm volatile ("mov #0xAA, w8");
    asm volatile ("mov w8, _NVMKEY");
    asm volatile ("bset _RCFGCAL, #13"); //set the RTCWREN bit
    asm volatile ("pop w8");
    asm volatile ("pop w7");
    
    // Enable RTCC and Alarm interval to desired len
    _RTCEN = 1;
    ALCFGRPTbits.AMASK = length;
    
    ALCFGRPTbits.ALRMPTR = 0b0010;
    ALRMVAL = 0x0000;
    ALRMVAL = 0x0000;
    ALRMVAL = 0x0000;
    ALCFGRPTbits.ARPT = 0;  
    ALCFGRPTbits.ALRMEN = 1;
    _RTCWREN = 0;
    
    // Go to sleep
    Sleep();
    asm volatile("NOP"); // Must be the next instruction after sleep
    
    // Disable RTCC on wakeup, must be done so we don't keep getting
    //  RTCC interrupts. If this isn't done in 'length' time, 
    //  Application may address fault.
    asm volatile ("push w7");
    asm volatile ("push w8");
    asm volatile ("disi #5");
    asm volatile ("mov #0x55, w7");
    asm volatile ("mov w7, _NVMKEY");
    asm volatile ("mov #0xAA, w8");
    asm volatile ("mov w8, _NVMKEY");
    asm volatile ("bset _RCFGCAL, #13"); //set the RTCWREN bit
    asm volatile ("pop w8");
    asm volatile ("pop w7");
    
    _RTCEN = 0;
    _RTCPTR = 0b00; // get ready to set seconds/minutes
    RTCVAL = 0x0000; // set minutes and secs back to 0.
    
    _RTCWREN = 0;
}
