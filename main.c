/* 
 * File:   Water4Project.c
 * Author: Jacqui
 *
 * Created on January 20, 2016, 6:35 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <xc.h>
#include <string.h>
#include "display.h"
#include "utilities.h"





// ****************************************************************************
// *** PIC24F32KA302 Configuration Bit Settings *******************************
// ****************************************************************************
// FBS
#pragma config BWRP = OFF // Boot Segment Write Protect (Disabled)
#pragma config BSS = OFF // Boot segment Protect (No boot program flash segment)
// FGS
#pragma config GWRP = OFF // General Segment Write Protect (General segment may be written)
#pragma config GSS0 = OFF // General Segment Code Protect (No Protection)
// FOSCSEL
#pragma config FNOSC = FRC // Oscillator Select (Fast RC Oscillator (FRC))
#pragma config SOSCSRC = ANA // SOSC Source Type (Analog Mode for use with crystal)
#pragma config LPRCSEL = LP // LPRC Oscillator Power and Accuracy (High Power, High Accuracy Mode)
#pragma config IESO = OFF // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-speed Start-up enabled))
// FOSC
#pragma config POSCMOD = NONE // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF // CLKO Enable Configuration bit (CLKO output disabled)
#pragma config POSCFREQ = HS // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8MHz)
#pragma config SOSCSEL = SOSCHP // SOSC Power Selection Configuration bits (Secondary Oscillator configured for high-power operation)
#pragma config FCKSM = CSDCMD // Clock Switching and Monitor Selection (Both Clock Switching and Fail-safe Clock Monitor are disabled)
// FWDT
#pragma config WDTPS = PS32768 // Watchdog Timer Postscale Select bits (1:32768)
#pragma config FWPSA = PR128 // WDT Prescaler bit (WDT prescaler ratio of 1:128)
#pragma config FWDTEN = OFF // Watchdog Timer Enable bits (WDT disabled in hardware; SWDTEN bit disabled)
#pragma config WINDIS = ON // Windowed Watchdog Timer Disable bit (Windowed WDT enabled)
// FPOR
#pragma config BOREN = BOR3 // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware, SBOREN bit disabled)
#pragma config LVRCFG = OFF // (Low Voltage regulator is not available)
#pragma config PWRTEN = ON // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI // Alternate I2C1 Pin Mapping bit (Use Default SCL1/SDA1 Pins For I2C1)
//#pragma config BORV = V20 // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (2.0V))
#pragma config MCLRE = ON // MCLR Pin Enable bit (RA5 input pin disabled,MCLR pin enabled)
// FICD
#pragma config ICS = PGx2 // ICD Pin Placement Select bits (EMUC/EMUD share PGC1/PGD1)
// FDS
#pragma config DSWDTPS = DSWDTPSF // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC // DSWDT Reference Clock Select bit (DSWDT uses Low Power RC Oscillator (LPRC))
#pragma config DSBOREN = ON // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

const int pulseWidthThreshold = 20; // The value to check the pulse width against (2048)
static volatile int buttonFlag = 0; // alerts us that the button has been pushed and entered the inerrupt subroutine
static bool isButtonTicking = false;
static volatile int buttonTicks = 0;

void initAdc(void); // forward declaration of init adc

/*********************************************************************
 * Function: initialization()
 * Input: None
 * Output: None
 * Overview: configures chip to work in our system (when power is turned on, these are set once)
 * Note: Pic Dependent
 * TestDate: 06-03-14
 ********************************************************************/
void initialization(void) {
    //IO port control
    ANSA = 0; // Make PORTA digital I/O
    TRISA = 0xFFFF; // Make PORTA all inputs
    ANSB = 0; // All port B pins are digital. Individual ADC are set in the readADC function
    TRISB = 0x0DC0; //0xCEE0; // Set LCD outputs as outputs
    TRISBbits.TRISB4 = 1;
    // Timer control (for WPS)
    T1CONbits.TCS = 0; // Source is Internal Clock (8MHz)
    T1CONbits.TCKPS = 0b11; // Prescalar to 1:256
    T1CONbits.TON = 1; // Enable the timer (timer 1 is used for the water sensor)

    //H2O sensor config
    // WPS_ON/OFF pin 2
    TRISAbits.TRISA0 = 0; //makes water presence sensor pin an output.
    PORTAbits.RA0 = 1; //turns on the water presence sensor.

    CNEN2bits.CN24IE = 1; // enable interrupt for pin 15
    IEC1bits.CNIE = 1; // enable change notification interrupt

    initAdc();
}

/*********************************************************************
 * Function: readWaterSensor
 * Input: None
 * Output: pulseWidth
 * Overview: RB5 is one water sensor, start at beginning of positive pulse
 * Note: Pic Dependent
 * TestDate: Not tested as of 03-05-2015
 ********************************************************************/
int readWaterSensor(void) // RB5 is one water sensor
{
    // WPS_OUT - pin14
    if (PORTBbits.RB8) 
    {
        while (PORTBbits.RB8) 
        {
        }; //make sure you start at the beginning of the positive pulse
    }
    
    while (!PORTBbits.RB8) 
    {
    }; //wait for rising edge
    
    uint32_t prevICTime = TMR1; //get time at start of positive pulse
    while (PORTBbits.RB8) 
    {
    };
    uint32_t currentICTime = TMR1; //get time at end of positive pulse
    uint32_t pulseWidth = 0;
    if (currentICTime >= prevICTime) 
    {
        pulseWidth = (currentICTime - prevICTime);
    } 
    else 
    {
        pulseWidth = (currentICTime - prevICTime + 0x100000000);
    }
    //Check if this value is right
    return (pulseWidth <= pulseWidthThreshold);
}

/*********************************************************************
 * Function: initAdc()
 * Input: None
 * Output: None
 * Overview: Initializes Analog to Digital Converter
 * Note: Pic Dependent
 * TestDate: 06-02-2014
 ********************************************************************/
void initAdc(void) 
{
    // 10bit conversion
    AD1CON1 = 0; // Default to all 0s
    AD1CON1bits.ADON = 0; // Ensure the ADC is turned off before configuration
    AD1CON1bits.FORM = 0; // absolute decimal result, unsigned, right-justified
    AD1CON1bits.SSRC = 0; // The SAMP bit must be cleared by software
    AD1CON1bits.SSRC = 0x7; // The SAMP bit is cleared after SAMC number (see
    // AD3CON) of TAD clocks after SAMP bit being set
    AD1CON1bits.ASAM = 0; // Sampling begins when the SAMP bit is manually set
    AD1CON1bits.SAMP = 0; // Don't Sample yet
    // Leave AD1CON2 at defaults
    // Vref High = Vcc Vref Low = Vss
    // Use AD1CHS (see below) to select which channel to convert, don't
    // scan based upon AD1CSSL
    AD1CON2 = 0;
    // AD3CON
    // This device needs a minimum of Tad = 600ns.
    // If Tcy is actually 1/8Mhz = 125ns, so we are using 3Tcy
    //AD1CON3 = 0x1F02; // Sample time = 31 Tad, Tad = 3Tcy
    AD1CON3bits.SAMC = 0x1F; // Sample time = 31 Tad (11.6us charge time)
    AD1CON3bits.ADCS = 0x2; // Tad = 3Tcy
    // Conversions are routed through MuxA by default in AD1CON2
    AD1CHSbits.CH0NA = 0; // Use Vss as the conversion reference
    AD1CSSL = 0; // No inputs specified since we are not in SCAN mode
    // AD1CON2
}

/*********************************************************************
 * Function: readAdc()
 * Input: channel
 * Output: adcValue
 * Overview: check with accelerometer
 * Note: Pic Dependent
 * TestDate:
 ********************************************************************/
int readAdc(int channel) //check with accelerometer
{
    switch (channel) 
    {
        case 4:
            ANSBbits.ANSB2 = 1; // AN4 is analog
            TRISBbits.TRISB2 = 1; // AN4 is an input
            AD1CHSbits.CH0SA = 4; // Connect AN4 as the S/H input
            break;
    }
    AD1CON1bits.ADON = 1; // Turn on ADC
    AD1CON1bits.SAMP = 1;
    while (!AD1CON1bits.DONE) 
    {
    }
    // Turn off the ADC, to conserve power
    AD1CON1bits.ADON = 0;
    return ADC1BUF0;
}

void __attribute__((__interrupt__, __auto_psv__)) _DefaultInterrupt() 
{ 
    // We should never be here
}

void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) 
{
    if (IFS1bits.CNIF && PORTBbits.RB6) 
    { // If the button is pushed and we're in the right ISR
        buttonFlag = 1;
    }

    // Always reset the interrupt flag
    IFS1bits.CNIF = 0;
}

void hoursToAsciiDisplay(int hours, int decimalHour) 
{
    int startLcdView = 0;
    DisplayTurnOff();
    unsigned char aryPtr[] = "H: ";
    DisplayDataAddString(aryPtr, sizeof ("H: "));
    //    DisplayDataAddCharacter(49); // can we cycle power, or ones mixed up.

    if (hours == 0) 
    {
        DisplayDataAddCharacter(48);
    } 
    else 
    {
        if (startLcdView || (hours / 10000 != 0)) 
        {
            DisplayDataAddCharacter(hours / 10000 + 48);
            startLcdView = 1;
            hours = hours - ((hours / 10000) * 10000); // moving the decimal point - taking advantage of int rounding
        }
        
        if (startLcdView || hours / 1000 != 0) 
        {
            DisplayDataAddCharacter(hours / 1000 + 48);
            startLcdView = 1;
            hours = hours - ((hours / 1000) * 1000);
        }
        
        if (startLcdView || hours / 100 != 0) 
        {
            DisplayDataAddCharacter(hours / 100 + 48);
            startLcdView = 1;
            hours = hours - ((hours / 100) * 100);
        }
        
        if (startLcdView || hours / 10 != 0) 
        {
            DisplayDataAddCharacter(hours / 10 + 48);
            startLcdView = 1;
            hours = hours - ((hours / 10) * 10);
        }
        
        DisplayDataAddCharacter(hours + 48);
    }
    
    DisplayDataAddCharacter('.');
    startLcdView = 0;
    
    if (decimalHour == 0) 
    {
        DisplayDataAddCharacter(48);
    } 
    else 
    {   
        if (startLcdView || decimalHour / 100 != 0) 
        {
            DisplayDataAddCharacter(decimalHour / 100 + 48);
            startLcdView = 1;
            decimalHour = decimalHour - ((decimalHour / 100) * 100);
        }
        else
        {
            DisplayDataAddCharacter(48);
        }
        
        if (startLcdView || decimalHour / 10 != 0) 
        {
            DisplayDataAddCharacter(decimalHour / 10 + 48);
            startLcdView = 1;
            decimalHour = decimalHour - ((decimalHour / 10) * 10);
        }
        else
        {
            DisplayDataAddCharacter(48);
        }
        
        DisplayDataAddCharacter(decimalHour + 48);
    }

    DisplayLoop(15, true);
}

static int countdownPos = 0;
const unsigned char countdownArray[] = { '5', '5', '4', '4', '3', '3', '2', '2', '1', '1', '0', '0' };
const unsigned char countdownResetArray[] = "Reset In ";
static void DisplayCountdown(void)
{
DisplayTurnOff();
DisplayDataAddString((unsigned char *)&countdownResetArray, sizeof(countdownResetArray));    
DisplayDataAddCharacter(countdownArray[countdownPos++]);
    DisplayLoop(15, true);
}

static void ResetDisplayCountdown(void)
{
    countdownPos = 0;
    buttonTicks = 0;
}

#define delayTime                   500
#define msHr                        (uint32_t)3600000
#define hourTicks                   msHr / delayTime
#define BUTTON_TICK_COUNTDOWN_THRESHOLD          10
#define BUTTON_TICK_RESET_THRESHOLD              20

int main(void)
{   
    resetCheckRemedy();
    
    initialization();

    DisplayInit();
    
    uint16_t tickCounter = 0;
    uint16_t hourCounter = 0;
 
    while (1) 
    {
        //sleepForPeriod(HALF_SECOND);
delayMs(delayTime);

        if (readWaterSensor())
        {
            tickCounter++;

            if (tickCounter >= hourTicks) 
            {
                hourCounter++;
                tickCounter = 0;
            }
        }
        
        if(isButtonTicking)
        {
            if(PORTBbits.RB6)
            {
               buttonTicks++; 
               if(buttonTicks > BUTTON_TICK_COUNTDOWN_THRESHOLD)
               {
                   DisplayCountdown();
               }
               if(buttonTicks > BUTTON_TICK_RESET_THRESHOLD)
               {
                   tickCounter = 0;
                   hourCounter = 0;
                   isButtonTicking = false;
                   ResetDisplayCountdown();
                   DisplayTurnOff();
               }
            }
            else
            {
                
                isButtonTicking = false;
                ResetDisplayCountdown();
                DisplayTurnOff();
                DisplayLoop(1, true);
            }
            
        }

        if (buttonFlag) 
        { // If someone pushed the button
            buttonFlag = 0;
            
            hoursToAsciiDisplay(hourCounter, // hour part
                    (tickCounter / (hourTicks / 1000))); // decimal hour part
            
            // We should clear/turn off display here
            isButtonTicking = true;
        }
    }

    return -1;
}
