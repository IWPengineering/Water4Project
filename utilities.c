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
    
    IFS3bits.RTCIF = 0;
    IEC3bits.RTCIE = 1;
}

void sleepms(sleepLength_t length)
{
    ALCFGRPTbits.AMASK = length;
    ALCFGRPTbits.ALRMEN = 1;
    
    // Go to sleep
    Sleep();
}
