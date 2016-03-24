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
#ifndef DISPLAY_H__
#define	DISPLAY_H__

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdbool.h>
#include "utilities.h"

#define DISPLAY_DATA_POSITION_MAX 75

#define DSP0_PORT           LATBbits.LATB2 //pin25
#define DSP1_PORT           LATBbits.LATB14 //LATAbits.LATA7     // pin 19 is on port a
#define DSP2_PORT           LATBbits.LATB3 // LATAbits.LATA7 // pin 19 //
#define DSP3_PORT           LATBbits.LATB13
#define DSP4_PORT           LATBbits.LATB4
#define DSP5_PORT           LATBbits.LATB12 // LATAbits.LATA7 //LATBbits.LATB7
#define DSP6_PORT           LATBbits.LATB5
#define DSP7_PORT           LATBbits.LATB9

// That may be fine, but i don't think so , for kicks and giggles lets try pin 19?

#define DSP0_PORT_DIR       TRISBbits.TRISB2 //  LATBbits.RB14
#define DSP1_PORT_DIR       TRISBbits.TRISB14 // TRISAbits.TRISA7 //    LATBbits.RB2
#define DSP2_PORT_DIR       TRISBbits.TRISB3 // TRISAbits.TRISA7 // pin 19 // TRISBbits.TRISB9 //    LATBbits.RB13
#define DSP3_PORT_DIR       TRISBbits.TRISB13 //    LATBbits.RB3
#define DSP4_PORT_DIR       TRISBbits.TRISB4 //    LATBbits.RB12
#define DSP5_PORT_DIR       TRISBbits.TRISB12 // TRISAbits.TRISA7 //TRISBbits.TRISB7 //   LATBbits.RB4
#define DSP6_PORT_DIR       TRISBbits.TRISB5 //    LATBbits.RB9
#define DSP7_PORT_DIR       TRISBbits.TRISB9 //    LATBbits.RB5

#define DISPLAY_COMMAND_FUNCTION_SET    0x34//0b00111001
#define DISPLAY_COMMAND_OFF             0b00001000
#define DISPLAY_COMMAND_CLEAR           0b00000001
#define DISPLAY_COMMAND_ENTRY           0b00000110
#define DISPLAY_COMMAND_HOME            0b00000010
#define DISPLAY_COMMAND_ON              0b00001100

/****  DISPLAY SPECIFIC  ******/
#define DISPLAY_BIT_REGISTER_SELECT__COMMAND_DATA  LATBbits.LATB0 // LATAbits.LATA6
#define DISPLAY_BIT_SELECT_DIR__READ_WRITE LATBbits.LATB1
#define DISPLAY_BIT_ENABLE__HIGH_LO LATBbits.LATB15 // LATAbits.LATA4
//#define DISPLAY_BIT_BUSY_FLAG PORTBbits.RB7

#define DISPLAY_BIT_REGISTER_SELECT__COMMAND_DATA_DIR TRISBbits.TRISB0 // TRISAbits.TRISA6 // LATAbits.LATA6
#define DISPLAY_BIT_SELECT_DIR__READ_WRITE_DIR TRISBbits.TRISB1 // LATAbits.LATA7
#define DISPLAY_BIT_ENABLE__HIGH_LO_DIR TRISBbits.TRISB15 // TRISAbits.TRISA4 // LATAbits.LATA4

typedef enum _DisplayCommand {
    sendCommand = 1,
    sendData = 2,
    setPosition = 3
} DisplayCommand_t;

typedef struct _DisplayData {
    enum _DisplayCommand command;
    unsigned char data;
} DisplayData_t;

void DisplayInit(void);
void DisplayLoop(int count, bool resetCursor);
void DisplayDataAddString(unsigned char *string, int size);
void DisplayDataAddCharacter(unsigned char ch);

#endif	/* DISPLAY_H__ */

