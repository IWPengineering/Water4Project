/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMER_H__
#define	TIMER_H__

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>

typedef enum 
{
    TIMER_1 = 1,
    TIMER_2 = 2,
    TIMER_3 = 3,
    TIMER_4 = 4,
    TIMER_5 = 5
} timer_instance_t;

typedef enum 
{
    LPRC_CLOCK = 0b10,
    EXTERNAL_CLOCK = 0b01,
    SOSC_CLOCK = 0b00,
    INTERNAL_CLOCK
} timer_clock_source_t;

typedef enum 
{
    TIMER_PRESCALER_256 = 0b11,
    TIMER_PRESCALER_64 = 0b10,
    TIMER_PRESCALER_8 = 0b01,
    TIMER_PRESCALER_1 = 0b00
} timer_prescaler_t;

typedef struct 
{
    timer_instance_t timer_instance;
    timer_clock_source_t source;
    timer_prescaler_t prescaler;
    bool stop_in_idle_mode;
    bool is_32_bit_enabled; // only valid for 2 and 4
} timer_init_t;

void timer_init(timer_init_t *p_init);
bool timer_start(timer_instance_t timer);
bool timer_stop(timer_instance_t timer);
uint16_t timer_get_16bit(timer_instance_t timer);
uint32_t timer_get_32bit(timer_instance_t timer);

#endif	/* TIMER_H__ */

