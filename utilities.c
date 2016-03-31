/*
 * File:   utilities.c
 * Author: KSK0419
 *
 * Created on March 22, 2016, 10:45 AM
 */


#include "xc.h"
#include "utilities.h"

/*********************************************************************
 * Function: delayMs()
 * Input: milliseconds
 * Output: None
 * Overview: Delays the specified number of milliseconds
 * Note: Clock speed dependent
 * TestDate: 05-20-14
 ********************************************************************/
void delayMs(volatile int ms) 
{
    volatile int myIndex;
    while (ms > 0) 
    {
        myIndex = 0;
        while (myIndex < 667) 
        {
            myIndex++;
        }
        ms--;
    }
}

void __attribute__ ((interrupt, auto_psv)) _RTCCInterrupt(void)
{
    _RTCIF = 0; // Clear the interrupt flag
    // Go back to wherever we were executing from - the goal here is
    //  just to sleep for some time
}

void initSleep(void)
{
    RCFGCALbits.RTCWREN = 1; // enable writing to the RTCC control registers
    RCFGCAL  = 0x2200;
    RTCPWC   = 0x0400;
    ALCFGRPTbits.CHIME = 0;
    RCFGCALbits.RTCEN = 1;
    
    /* Set interrupt priority to lowest available while still being enabled
     Note: This is important! If the interrupt is of a higher or equal
     priority to a CPU interrupt, then the device will generate either
     a hard fault or address fault on wakeup from sleep.
     */
    _RTCIP = 0b001;
    IFS3bits.RTCIF = 0;
    IEC3bits.RTCIE = 1;
}

void sleepForPeriod(sleepLength_t length)
{
    ALCFGRPTbits.AMASK = length;
    ALCFGRPTbits.ALRMEN = 1;
    
    // Go to sleep
    Sleep();
}

void resetCheckRemedy(void)
{
    if(_TRAPR)
    {
        // Trap Reset Conflict has occured
        _TRAPR = 0;
    }
    
    if(_IOPUWR)
    {
        // Illegal Opcode, illegal address mode, or
        //  W reg used as address pointer caused a reset
        _IOPUWR = 0;
    }

    if(_DPSLP)
    {
        // Woke up from Deep Sleep Mode
        _DPSLP = 0;
    }
    
    if(RCONbits.CM)
    {
        // Configuration word mismatch occurred
        RCONbits.CM = 0;
    }
    
    if(_EXTR)
    {
        // External Reset (MCLR Pin) has occurred
        _EXTR = 0;
    }
    
    if(_SWR)
    {
        // Software Reset has occurred
        _SWR = 0;
    }
    
    if(_WDTO)
    {
        // Watchdog timeout reset has occurred
        _WDTO = 0;
    }
    
    if(_SLEEP)
    {
        // Woke up from sleep mode
        _SLEEP = 0;
    }
    
    if(_IDLE)
    {
        // Woke up from idle mode
        _IDLE = 0;
    }
    
    if(_BOR)
    {
        // Brownout caused reset
        _BOR = 0;
    }
    
    if(_POR)
    {
        // Power-up reset has occurred
        _POR = 0;
    }
}