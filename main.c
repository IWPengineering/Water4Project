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





/**********************/
/**********************/
/**********************/
/**********************/
/**********************/
/***************************************/
/****  DISPLAY SPECIFIC  ******/
#define DISPLAY_BIT_REGISTER_SELECT__COMMAND_DATA  LATBbits.LATB0 // LATAbits.LATA6
#define DISPLAY_BIT_SELECT_DIR__READ_WRITE LATBbits.LATB15
#define DISPLAY_BIT_ENABLE__HIGH_LO LATBbits.LATB1 // LATAbits.LATA4
//#define DISPLAY_BIT_BUSY_FLAG PORTBbits.RB7

#define DISPLAY_BIT_REGISTER_SELECT__COMMAND_DATA_DIR TRISBbits.TRISB0 // TRISAbits.TRISA6 // LATAbits.LATA6
#define DISPLAY_BIT_SELECT_DIR__READ_WRITE_DIR TRISBbits.TRISB15 // LATAbits.LATA7
#define DISPLAY_BIT_ENABLE__HIGH_LO_DIR TRISBbits.TRISB1 // TRISAbits.TRISA4 // LATAbits.LATA4


//#define DISPLAY_DATA_WRITE LATB
//#define DISPLAY_DATA_DIR_SET TRISB
//#define DISPLAY_DATA_DIR_BUSY_FLAG TRISBbits.TRISB7

//#define DISPLAY_LOOP_COUNT 5

#define DISPLAY_COMMAND_FUNCTION_SET 0b00111001
#define DISPLAY_COMMAND_OFF 0b00001000
#define DISPLAY_COMMAND_CLEAR 0b00000001
#define DISPLAY_COMMAND_ENTRY 0b00000110
#define DISPLAY_COMMAND_HOME 0b00000010
#define DISPLAY_COMMAND_ON 0b00001100

typedef enum _DisplayCommand {
    sendCommand = 1,
    sendData = 2,
    setPosition = 3
};

struct _DisplayData {
    enum _DisplayCommand command;
    unsigned char data;
};


void dspDisplayInit(void);
void dspDisplayReset(void);
void dspDisplaySend(enum _DisplayCommand, unsigned char);
void dspDisplayDataAddOne(enum _DisplayCommand, unsigned char);
void dspDisplayDataSetRow(unsigned char);
void dspDisplayDataAddString(unsigned char *, int size);
void dspDisplayDataAddInteger(int);
void dspDisplayLoop(int);


#define DISPLAY_DATA_POSITION_MAX 75
struct _DisplayData DisplayData[DISPLAY_DATA_POSITION_MAX];
unsigned char DisplayDataPositionRead = 0;
unsigned char DisplayDataPositionWrite = 0;
unsigned char DisplayDataPositionInverse;

/***********************************************
 END DISPLAY SPECIFIC
 *******************************************/

void DisplayInit(void);
void DisplayLoop(int);
void DisplayDataAddString(unsigned char *, int size);
void DisplayDataAddInteger(int);
void DisplayDataSetRow(unsigned char);
void DisplayValues(unsigned char, unsigned long, unsigned long, int, int);
void HeartBeat(void);



#define DSP0_PORT           LATBbits.LATB12
#define DSP1_PORT           LATBbits.LATB4 //LATAbits.LATA7     // pin 19 is on port a
#define DSP2_PORT           LATAbits.LATA7 // pin 19 // LATBbits.LATB9
#define DSP3_PORT           LATBbits.LATB5
#define DSP4_PORT           LATBbits.LATB14
#define DSP5_PORT           LATBbits.LATB2
#define DSP6_PORT           LATBbits.LATB13
#define DSP7_PORT           LATBbits.LATB3

// That may be fine, but i don't think so , for kicks and giggles lets try pin 19?

#define DSP0_PORT_DIR       TRISBbits.TRISB12 //  LATBbits.RB14
#define DSP1_PORT_DIR       TRISBbits.TRISB4 // TRISAbits.TRISA7 //    LATBbits.RB2
#define DSP2_PORT_DIR       TRISAbits.TRISA7 // pin 19 // TRISBbits.TRISB9 //    LATBbits.RB13
#define DSP3_PORT_DIR       TRISBbits.TRISB5 //    LATBbits.RB3
#define DSP4_PORT_DIR       TRISBbits.TRISB14 //    LATBbits.RB12
#define DSP5_PORT_DIR       TRISBbits.TRISB2 //   LATBbits.RB4
#define DSP6_PORT_DIR       TRISBbits.TRISB13 //    LATBbits.RB9
#define DSP7_PORT_DIR       TRISBbits.TRISB3 //    LATBbits.RB5


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
    ////------------Sets up all ports as digial inputs-----------------------
    //IO port control
    ANSA = 0; // Make PORTA digital I/O
    TRISA = 0xFFFF; // Make PORTA all inputs
    ANSB = 0; // All port B pins are digital. Individual ADC are set in the readADC function
    //TRISB = 0xFFFF; // Sets all of port B to input
    TRISB = 0x0DC0; //0xCEE0; // Set LCD outputs as outputs



    // Timer control (for WPS)
    T1CONbits.TCS = 0; // Source is Internal Clock (8MHz)
    T1CONbits.TCKPS = 0b11; // Prescalar to 1:256
    T1CONbits.TON = 1; // Enable the timer (timer 1 is used for the water sensor)

    // UART config
    U1BRG = 51; // Set baud to 9600, FCY = 8MHz (#pragma config FNOSC = FRC)
    U1STA = 0;
    U1MODE = 0x8000; //enable UART for 8 bit data//no parity, 1 stop bit
    U1STAbits.UTXEN = 1; //enable transmit

    //H2O sensor config
    // WPS_ON/OFF pin 2
    TRISAbits.TRISA0 = 0; //makes water presence sensor pin an output.
    PORTAbits.RA0 = 1; //turns on the water presnece sensor.

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
    if (PORTBbits.RB8) {
        while (PORTBbits.RB8) {
        }; //make sure you start at the beginning of the positive pulse
    }
    while (!PORTBbits.RB8) {
    }; //wait for rising edge
    int prevICTime = TMR1; //get time at start of positive pulse
    while (PORTBbits.RB8) {
    };
    int currentICTime = TMR1; //get time at end of positive pulse
    long pulseWidth = 0;
    if (currentICTime >= prevICTime) {
        pulseWidth = (currentICTime - prevICTime);
    } else {
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
void initAdc(void) {
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
    switch (channel) {
        case 4:
            ANSBbits.ANSB2 = 1; // AN4 is analog
            TRISBbits.TRISB2 = 1; // AN4 is an input
            AD1CHSbits.CH0SA = 4; // Connect AN4 as the S/H input
            break;
    }
    AD1CON1bits.ADON = 1; // Turn on ADC
    AD1CON1bits.SAMP = 1;
    while (!AD1CON1bits.DONE) {
    }
    return ADC1BUF0;
}

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

void __attribute__((__interrupt__, __auto_psv__)) _DefaultInterrupt() { //Tested 06-05-2014

}

void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
    if (IFS1bits.CNIF && PORTBbits.RB6) { // if button pushed it goes high

        buttonFlag = 1; // I have entered the ISR

        IFS1bits.CNIF = 0;
    }

    IFS1bits.CNIF = 0;

}

void hoursToAsciiDisplay(int hours, int decimalHour) {
    int startLcdView = 0;

    DisplayDataSetRow(1);
    unsigned char aryPtr[] = "Hours: ";
    DisplayDataAddString(aryPtr, sizeof ("Hours: "));
    //    DisplayDataAddCharacter(49); // can we cycle power, or ones mixed up.

    if (hours == 0) {
        DisplayDataAddCharacter(48); //        sendData(48); // send 0 as the number of hours
    } else {
        if (startLcdView || hours / 10000 != 0) {
            DisplayDataAddCharacter(hours / 10000 + 48);
            startLcdView = 1;
        }
        hours /= 10;
        if (startLcdView || hours / 1000 != 0) {
            DisplayDataAddCharacter(hours / 1000 + 48);
            startLcdView = 1;
        }
        hours /= 10;
        if (startLcdView || hours / 100 != 0) {
            DisplayDataAddCharacter(hours / 100 + 48);
            startLcdView = 1;
        }
        hours /= 10;
        if (startLcdView || hours / 10 != 0) {
            DisplayDataAddCharacter(hours / 10 + 48);
            startLcdView = 1;
        }
        hours /= 10;
        DisplayDataAddCharacter(hours + 48);
    }
    DisplayDataAddCharacter('.');

    if (decimalHour == 0) {
        DisplayDataAddCharacter(48); //        sendData(48); // send 0 as the number of hours
    } else {
        if (startLcdView || decimalHour / 1000 != 0) {
            DisplayDataAddCharacter(decimalHour / 1000 + 48);
            startLcdView = 1;
        }
        decimalHour /= 10;
        if (startLcdView || decimalHour / 100 != 0) {
            DisplayDataAddCharacter(decimalHour / 100 + 48);
            startLcdView = 1;
        }
        decimalHour /= 10;
        if (startLcdView || decimalHour / 10 != 0) {
            DisplayDataAddCharacter(decimalHour / 10 + 48);
            startLcdView = 1;
        }
        decimalHour /= 10;
        DisplayDataAddCharacter(decimalHour + 48);
    }


    DisplayDataAddCharacter(' ');
    DisplayDataAddCharacter(' ');
    DisplayDataAddCharacter(' ');
    DisplayDataAddCharacter(' ');
    DisplayDataAddCharacter(' ');

    DisplayLoop(10);
}

/*
 * 
 */
#define delayTime       500
#define msHr            (uint32_t)3600000
#define hourTicks       msHr / delayTime

int main(void) {
    initialization();

    DisplayInit();

    DSP0_PORT = 1;
    DSP1_PORT = 1;
    DSP2_PORT = 1;
    DSP3_PORT = 1;
    DSP4_PORT = 1;
    DSP5_PORT = 1;
    DSP6_PORT = 1;
    DSP7_PORT = 1;

    DisplayInit();

    DisplayDataSetRow(0);
    unsigned char aryPtr[] = "";
    DisplayDataAddString(aryPtr, sizeof (""));
    DisplayLoop(10);
    DisplayLoop(10);
    DisplayLoop(10);
    DisplayLoop(10);

    int counter = 0;
    int hourCounter = 0;
    //    PORTBbits.RB15 = 0; //R/W always low for write CTL2

    // There's a chance that our numbers aren't ASCII
    //sendData('h');   //H
    //            sendData(0x65);   //E
    //            sendData(0x6C);   //L
    //            sendData(0x6C);   //L
    //            sendData(0x6F);   //0


 
    while (1) {
        HeartBeat();
        DisplayLoop(10);

        //        delayMs(delayTime);
        // is there water?
        if (readWaterSensor()) {
            counter++; // increments every half a second

            if (counter >= hourTicks) { // 7200 counter has reached 1 hour's worth
                hourCounter++;
                counter = 0;
            }
        }

        if (buttonFlag) {
            buttonFlag = 0;
            //            hourCounter = 3;
            //            counter = 7000;
            hoursToAsciiDisplay(3, 952);
            //            hoursToAsciiDisplay(hourCounter, (counter / 7.2)); // divide by 7.2 to give us the decimal accuracy of 3 places, as an integer.
            delayMs(500);
        }
    }

    return -1;
}

/*********************************
 DISPLAY FUNCTIONS
 * first functions only call the internal functions if we are using them
 * second set are entirely excluded if we are not using display features
 * this should help turn on and off the display without much trouble
 *********************************/
void DisplayInit(void) {
    dspDisplayInit();
    return;
}

void DisplayLoop(int count) {
    for (int inx = 0; inx < count; inx++) {
        dspDisplayLoop(1);
    }
    return;
}

void HeartBeat(void) {
    static int ia = 0;
    static int ib = 0;
#define HB 5
    /*
        unsigned char ptr1[] = {
            0xAA,
            0x55,
            0xAA,
            0x55,
            NULL
        };
        DisplayDataSetRow(0);
        DisplayDataAddString(ptr1, 5);
        unsigned char ptr2[] = {
            0x55,
            0xAA,
            0x55,
            0xAA,
            NULL
        };
        DisplayDataSetRow(0);
        DisplayDataAddString(ptr2, 5);
     */
    ia++;
    if (ia > HB * 2) {
        DisplayDataSetRow(0);
        unsigned char aryPtr[] = "x";
        DisplayDataAddString(aryPtr, sizeof ("x"));
        ia = 0;
        ib++;
        //        DisplayDataAddInteger(ib);
        //        DisplayDataAddString("  ");

    } else if (ia == HB) {
        DisplayDataSetRow(0);
        unsigned char aryPtr[] = "o";
        DisplayDataAddString(aryPtr, sizeof ("o"));

        //      DisplayDataAddInteger(ib);
        //        DisplayDataAddString("  ");
    }
    //DisplayLoop(10);
    return;
}

void DisplayValues(unsigned char row, unsigned long a, unsigned long b, int c, int d) {



    char *buf[40];

    //sprintf ( buf, "%5ld %5ld %5d %1d", a, b, c, d );

    dspDisplayDataSetRow(row);
    dspDisplayDataAddString(buf, sizeof (*buf));


    return;
}

void DisplayDataAddString(unsigned char *string, int size) {

    dspDisplayDataAddString(string, size);
    return;
}

void DisplayDataAddCharacter(unsigned char ch) {

    dspDisplayDataAddOne(sendData, ch);
    return;
}

void DisplayDataAddInteger(int in) {

    dspDisplayDataAddInteger(in);
    return;
}

void DisplayDataSetRow(unsigned char row) {

    dspDisplayDataSetRow(row);
    return;
}

void dspDisplayInit(void) {

    DISPLAY_BIT_REGISTER_SELECT__COMMAND_DATA_DIR = 0b0;
    DISPLAY_BIT_SELECT_DIR__READ_WRITE_DIR = 0b0;
    DISPLAY_BIT_ENABLE__HIGH_LO_DIR = 0b0;

    DISPLAY_BIT_SELECT_DIR__READ_WRITE = 0b0;

    //    DISPLAY_DATA_WRITE = 0b00000000;

    DSP0_PORT_DIR = 0b0;
    DSP1_PORT_DIR = 0b0;
    DSP2_PORT_DIR = 0b0;
    DSP3_PORT_DIR = 0b0;
    DSP4_PORT_DIR = 0b0;
    DSP5_PORT_DIR = 0b0;
    DSP6_PORT_DIR = 0b0;
    DSP7_PORT_DIR = 0b0;

    dspDisplayDataAddOne(sendCommand, DISPLAY_COMMAND_FUNCTION_SET);
    dspDisplayDataAddOne(sendCommand, DISPLAY_COMMAND_OFF);
    dspDisplayDataAddOne(sendCommand, DISPLAY_COMMAND_CLEAR);
    dspDisplayDataAddOne(sendCommand, DISPLAY_COMMAND_ENTRY);
    dspDisplayDataAddOne(sendCommand, DISPLAY_COMMAND_HOME);
    dspDisplayDataAddOne(sendCommand, DISPLAY_COMMAND_ON);

    return;
}

void dspDisplayReset(void) {
    dspDisplayDataAddOne(sendCommand, DISPLAY_COMMAND_CLEAR);
    dspDisplayDataAddOne(sendCommand, DISPLAY_COMMAND_HOME);

    return;
}

void dspDisplaySend(enum _DisplayCommand command, unsigned char data) {

    switch (command) {
        case sendCommand:
            DISPLAY_BIT_REGISTER_SELECT__COMMAND_DATA = 0b0;
            break;
        case sendData:
            DISPLAY_BIT_REGISTER_SELECT__COMMAND_DATA = 0b1;
            break;
        case setPosition:
            DISPLAY_BIT_REGISTER_SELECT__COMMAND_DATA = 0b0;
            // bit 7 must be a 1
            data |= 1 << 7;
            break;
        default:
            break;
    }

    //    DISPLAY_BIT_SELECT_DIR__READ_WRITE = 0b0;

    //DISPLAY_DATA_DIR_SET = 0b00000000;

    DISPLAY_BIT_ENABLE__HIGH_LO = 0b1;

    //    DISPLAY_DATA_WRITE = data;
    //    PORTBbits.RB14 = (data & 0x01) >> 0;
    //    PORTBbits.RB2  = (data & 0x02) >> 1;
    //    PORTBbits.RB13 = (data & 0x04) >> 2;
    //    PORTBbits.RB3  = (data & 0x08) >> 3;
    //    PORTBbits.RB12 = (data & 0x10) >> 4;
    //    PORTBbits.RB4  = (data & 0x20) >> 5;
    //    PORTBbits.RB9  = (data & 0x40) >> 6;
    //    PORTBbits.RB5  = (data & 0x80) >> 7;



    DSP0_PORT = (data & 0b00000001) >> 0;
    DSP1_PORT = (data & 0b00000010) >> 1;
    DSP2_PORT = (data & 0b00000100) >> 2;
    DSP3_PORT = (data & 0b00001000) >> 3;
    DSP4_PORT = (data & 0b00010000) >> 4;
    DSP5_PORT = (data & 0b00100000) >> 5;
    DSP6_PORT = (data & 0b01000000) >> 6;
    DSP7_PORT = (data & 0b10000000) >> 7;



    DISPLAY_BIT_ENABLE__HIGH_LO = 0b0;
    //  __delay_us ( 200 ); // we now check the busy bit
    // need something else to happen here
    // this feels like a big kludge
    // this needs fixed

    return;
}


//#define DSP0_PORT           PORTBbits.RB14
//#define DSP1_PORT           PORTBbits.RB2
//#define DSP2_PORT           PORTBbits.RB13
//#define DSP3_PORT           PORTBbits.RB3
//#define DSP4_PORT           PORTBbits.RB12
//#define DSP5_PORT           PORTBbits.RB4
//#define DSP6_PORT           PORTBbits.RB9
//#define DSP7_PORT           PORTBbits.RB5
//
//    PORTBbits.RB0 = 0;                 //low for command CTL1    
//    PORTBbits.RB1  = 0b1; // E
//    
//    PORTBbits.RB14 = (command & 0x01) >> 0;
//    PORTBbits.RB2  = (command & 0x02) >> 1;
//    PORTBbits.RB13 = (command & 0x04) >> 2;
//    PORTBbits.RB3  = (command & 0x08) >> 3;
//    PORTBbits.RB12 = (command & 0x10) >> 4;
//    PORTBbits.RB4  = (command & 0x20) >> 5;
//    PORTBbits.RB9  = (command & 0x40) >> 6;
//    PORTBbits.RB5  = (command & 0x80) >> 7;

void dspDisplayDataAddOne(enum _DisplayCommand command, unsigned char data) {

    if (DisplayDataPositionWrite <= DISPLAY_DATA_POSITION_MAX) {
        DisplayData[DisplayDataPositionWrite].command = command;
        DisplayData[DisplayDataPositionWrite].data = data;
        DisplayDataPositionWrite++;
    }

    return;
}

void dspDisplayDataSetRow(unsigned char row) {
    unsigned char rowValue;
    switch (row) {
        case 0:
            rowValue = 0x00;
            break;
        case 1:
            rowValue = 0x40;
            break;
        case 2:
            rowValue = 0x14;
            break;
        case 3:
            rowValue = 0x54;
            break;
        default:
            rowValue = 0x00;
            break;
    }

    dspDisplayDataAddOne(setPosition, rowValue);

    return;
}

void dspDisplayDataAddString(unsigned char *string, int size) {
    for (unsigned char i = 0; i < size - 1; i++) {
        dspDisplayDataAddOne(sendData, string[i]);
    }

    return;
}

void dspDisplayDataAddInteger(int in) {
    char temp[6];

    //itoa(temp, in, 10);

    dspDisplayDataAddString(temp, sizeof (temp));

    return;
}

void dspDisplayLoop(int count) {
    // reading busy flag does not work correctly right now
    // need to check it out

    for (int i = 0; i < count; i++) {
        //if ( IsDisplayBusy ( ) == 0 )
        {
            if (DisplayDataPositionRead < DisplayDataPositionWrite) {
                dspDisplaySend(DisplayData[DisplayDataPositionRead].command, DisplayData[DisplayDataPositionRead].data);
                DisplayDataPositionRead++;

                if (DisplayDataPositionRead >= DisplayDataPositionWrite) {
                    // we have caught up with all of the write data - so reset
                    DisplayDataPositionRead = 0;
                    DisplayDataPositionWrite = 0;
                }
            }
        }
    }

    return;
}

//#define DSP0_PORT           14
//#define DSP1_PORT           2
//#define DSP2_PORT           13
//#define DSP3_PORT           3
//#define DSP4_PORT           12
//#define DSP5_PORT           4
//#define DSP6_PORT           9
//#define DSP7_PORT           5
//
//    PORTBbits.RB0 = 0;                 //low for command CTL1    
//    PORTBbits.RB1  = 0b1; // E
//    
//    PORTBbits.RB14 = (command & 0x01) >> 0;
//    PORTBbits.RB2  = (command & 0x02) >> 1;
//    PORTBbits.RB13 = (command & 0x04) >> 2;
//    PORTBbits.RB3  = (command & 0x08) >> 3;
//    PORTBbits.RB12 = (command & 0x10) >> 4;
//    PORTBbits.RB4  = (command & 0x20) >> 5;
//    PORTBbits.RB9  = (command & 0x40) >> 6;
//    PORTBbits.RB5  = (command & 0x80) >> 7;





/*
unsigned char IsDisplayBusy ( void )
{
    unsigned char isBusy;

    DISPLAY_DATA_DIR_BUSY_FLAG = 0b1;
//    DISPLAY_BIT_SELECT_DIR__READ_WRITE = 0b1;
    isBusy = DISPLAY_BIT_BUSY_FLAG;
//    DISPLAY_BIT_SELECT_DIR__READ_WRITE = 0b0;
    DISPLAY_DATA_DIR_BUSY_FLAG = 0b0;

    return (isBusy );

}

 */


