/*
 * File:   utilities.c
 * Author: KSK0419
 *
 * Created on March 22, 2016, 10:45 AM
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "utilities.h"

/*********************************************************************
 * Function: delayMs()
 * Input: milliseconds
 * Output: None
 * Overview: Delays the specified number of milliseconds
 * Note: Clock speed dependent
 * TestDate: 05-20-14
 ********************************************************************/
void delay_ms(volatile int ms) 
{
    volatile int my_index;
    while (ms > 0) 
    {
        my_index = 0;
        while (my_index < 667) 
        {
            my_index++;
        }
        ms--;
    }
}

void reset_check(void)
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