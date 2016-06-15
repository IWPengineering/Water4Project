/*
 * File:   timer.c
 * Author: KSK0419
 *
 * Created on June 15, 2016, 1:44 PM
 */


#include "xc.h"
#include "timer.h"
#include <stdint.h>
#include <stdbool.h>

void timer_init(timer_init_t *p_init)
{
    if(p_init->timer_instance == TIMER_1)
    {
        T1CONbits.TON   = 0;
        T1CONbits.TSIDL = p_init->stop_in_idle_mode;
        T1CONbits.TCKPS = p_init->prescaler;
        
        if(p_init->source == INTERNAL_CLOCK)
        {
            T1CONbits.TCS   = 0;
        }
        else
        {
            T1CONbits.TCS   = 1;
            T1CONbits.T1ECS = p_init->source;
        }
    }
}

bool timer_start(timer_instance_t timer)
{
    if(timer == TIMER_1)
    {
        if(T1CONbits.TON)
        {
            return false;
        }
        else
        {
            T1CONbits.TON = 1;
            return true;
        }
    }
    
    return false;
}

bool timer_stop(timer_instance_t timer)
{
    if(timer == TIMER_1)
    {
        if(T1CONbits.TON)
        {
            T1CONbits.TON = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
    
    return false;
}

uint16_t timer_get_16bit(timer_instance_t timer)
{
    if(timer == TIMER_1)
    {
        return TMR1;
    }
    
    return 0;
}

uint32_t timer_get_32bit(timer_instance_t timer)
{
    return 0;
}