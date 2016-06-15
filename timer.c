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
#if USING_TIMER_1 == 1
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
#endif
    
#if USING_TIMER_2 == 1
    if(p_init->timer_instance == TIMER_2)
    {
        T2CONbits.TON   = 0;
        T2CONbiT1CONts.TSIDL = p_init->stop_in_idle_mode;
        T2CONbits.TCKPS = p_init->prescaler;
        T2CONbits.T32 = p_init->is_32_bit_enabled;
        
        if(p_init->source == INTERNAL_CLOCK)
        {
            T2CONbits.TCS   = 0;
        }
        else
        {
        }
    }
#endif
    
#if USING_TIMER_3 == 1
    if(p_init->timer_instance == TIMER_3)
    {
        T3CONbits.TON   = 0;
        T3CONbiT1CONts.TSIDL = p_init->stop_in_idle_mode;
        T3CONbits.TCKPS = p_init->prescaler;
        
        if(p_init->source == INTERNAL_CLOCK)
        {
            T3CONbits.TCS   = 0;
        }
        else
        {
        }
    }
#endif
    
#if USING_TIMER_4 == 1
    if(p_init->timer_instance == TIMER_4)
    {
        T4CONbits.TON           = 0;
        T4CONbiT1CONts.TSIDL    = p_init->stop_in_idle_mode;
        T4CONbits.TCKPS         = p_init->prescaler;
        T4CONbits.T32           = p_init->is_32_bit_enabled;
        
        if(p_init->source == INTERNAL_CLOCK)
        {
            T4CONbits.TCS   = 0;
        }
        else
        {
        }
    }
#endif
    
#if USING_TIMER_5 == 1
    if(p_init->timer_instance == TIMER_5)
    {
        T5CONbits.TON   = 0;
        T5CONbiT1CONts.TSIDL = p_init->stop_in_idle_mode;
        T5CONbits.TCKPS = p_init->prescaler;
        
        if(p_init->source == INTERNAL_CLOCK)
        {
            T5CONbits.TCS   = 0;
        }
        else
        {
        }
    }    
#endif
}

bool timer_start(timer_instance_t timer)
{
#if USING_TIMER_1 == 1
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
#endif
    
#if USING_TIMER_2 == 1
    if(timer == TIMER_2)
    {
        if(T2CONbits.TON)
        {
            return false;
        }
        else
        {
            T2CONbits.TON = 1;
            return true;
        }
    }
#endif
    
#if USING_TIMER_3 == 1
    if(timer == TIMER_3)
    {
        if(T3CONbits.TON)
        {
            return false;
        }
        else
        {
            T3CONbits.TON = 1;
            return true;
        }
    }    
#endif
    
#if USING_TIMER_4 == 1
    if(timer == TIMER_4)
    {
        if(T4CONbits.TON)
        {
            return false;
        }
        else
        {
            T4CONbits.TON = 1;
            return true;
        }
    }
#endif
    
#if USING_TIMER_5 == 1
    if(timer == TIMER_5)
    {
        if(T5CONbits.TON)
        {
            return false;
        }
        else
        {
            T5CONbits.TON = 1;
            return true;
        }
    }    
#endif
    return false;
}

bool timer_stop(timer_instance_t timer)
{
#if USING_TIMER_1 == 1
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
#endif 
    
#if USING_TIMER_2 == 1
    if(timer == TIMER_2)
    {
        if(T2CONbits.TON)
        {
            T2CONbits.TON = 0;
            return true;
        }
        else
        {
            return false;
        }
    }    
#endif
    
#if USING_TIMER_3 == 1
    if(timer == TIMER_3)
    {
        if(T3CONbits.TON)
        {
            T3CONbits.TON = 0;
            return true;
        }
        else
        {
            return false;
        }
    }    
#endif
    
#if USING_TIMER_4 == 1
    if(timer == TIMER_4)
    {
        if(T4CONbits.TON)
        {
            T4CONbits.TON = 0;
            return true;
        }
        else
        {
            return false;
        }
    }
#endif
    
#if USING_TIMER_5 == 1
    if(timer == TIMER_5)
    {
        if(T5CONbits.TON)
        {
            T5CONbits.TON = 0;
            return true;
        }
        else
        {
            return false;
        }
    }    
#endif
    return false;
}

uint16_t timer_get_16bit(timer_instance_t timer)
{
#if USING_TIMER_1 == 1
    if(timer == TIMER_1)
    {
        return TMR1;
    }
#endif
    
#if USING_TIMER_2 == 1
    if(timer == TIMER_2)
    {
        return TMR2;
    }    
#endif
    
#if USING_TIMER_3 == 1
    if(timer == TIMER_3)
    {
        return TMR3;
    }    
#endif
    
#if USING_TIMER_4 == 1
    if(timer == TIMER_4)
    {
        return TMR4;
    }
#endif
    
#if USING_TIMER_5 == 1
    if(timer == TIMER_5)
    {
        return TMR5;
    }    
#endif
    
    return 0;
}

uint32_t timer_get_32bit(timer_instance_t timer)
{
#if USING_TIMER_2 == 1
    
#endif
    
#if USING_TIMER_4 == 1

#endif
    return 0;
}