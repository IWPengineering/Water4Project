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
 * Note: Depends on Clock speed. Pic Dependent
 * TestDate: 05-20-14
 ********************************************************************/
void delayMs(int ms) {
    int myIndex;
    while (ms > 0) {
        myIndex = 0;
        while (myIndex < 667) {
            myIndex++;
        }
        ms--;
    }
}

void initSleep(void)
{
    
}

void sleepms(int ms)
{
    
}

void resetCheckRemedy(void)
{
    if(_TRAPR)
    {
        
    }
    
    if(_IOPUWR)
    {
        
    }

    if(_DPSLP)
    {
        
    }
    
    if(RCONbits.CM)
    {
        
    }
    
    if(_EXTR)
    {
        
    }
    
    if(_SWR)
    {
        
    }
    
    if(_WDTO)
    {
        
    }
    
    if(_SLEEP)
    {
        
    }
    
    if(_IDLE)
    {
        
    }
    
    if(_BOR)
    {
        
    }
    
    if(_POR)
    {
        
    }
}