/* Configuration Bits *********************************************************/
// FBS
#pragma config BWRP = OFF         // Boot Segment Write Protect (Disabled)
#pragma config BSS = OFF          // Boot segment Protect (No boot program
// flash segment)

// FGS
#pragma config GWRP = OFF         // General Segment Write Protect (General
// segment may be written)
#pragma config GSS0 = OFF         // General Segment Code Protect (No
// Protection)

// FOSCSEL
//#pragma config FNOSC = FRCDIV      Oscillator Select (8MHz FRC oscillator With
// Postscaler (FRCDIV))
#pragma config FNOSC = PRI//SOSC

#pragma config SOSCSRC = DIG      // SOSC Source Type (Analog Mode for use with
// crystal)
#pragma config LPRCSEL = HP       // LPRC Oscillator Power and Accuracy (High
// Power, High Accuracy Mode)
#pragma config IESO = ON          // Internal External Switch Over bit (Internal
// External Switchover mode enabled (Two-speed
// Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE     // Primary Oscillator Configuration bits
// (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF     // CLKO Enable Configuration bit (CLKO output
// signal is active on the OSCO pin)
#pragma config POSCFREQ = HS      // Primary Oscillator Frequency Range
// Configuration bits (Primary oscillator/
// external clock input frequency greater than
// 8MHz)
#pragma config SOSCSEL = SOSCHP   // SOSC Power Selection Configuration bits
// (Secondary Oscillator configured for
// high-power operation)
#pragma config FCKSM = CSDCMD     // Clock Switching and Monitor Selection (Both
// Clock Switching and Fail-safe Clock Monitor
// are disabled)

// FWDT
#pragma config WDTPS = PS16384    // Watchdog Timer Postscale Select bits
// (1:16384)
#pragma config FWPSA = PR32       // WDT Prescaler bit (WDT prescaler ratio of
// 1:32)
#pragma config FWDTEN = ON        // Watchdog Timer Enable bits (WDT enabled in
// hardware; SWDTEN bit enabled)
#pragma config WINDIS = OFF       // Windowed Watchdog Timer Disable bit
// (Standard WDT selected(windowed WDT
// disabled))

// FPOR
#pragma config BOREN = BOR0       // Brown-out Reset Enable bits (Brown-out
// Reset disabled in hardware, SBOREN bit
// disabled)
#pragma config LVRCFG = OFF       //  (Low Voltage regulator is not available)
#pragma config PWRTEN = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI      // Alternate I2C1 Pin Mapping bit (Use Default
// SCL1/SDA1 Pins For I2C1)
#pragma config BORV = V20         // Brown-out Reset Voltage bits (Brown-out
// Reset set to lowest voltage (2.0V))
#pragma config MCLRE = ON         // MCLR Pin Enable bit (RA5 input pin
//disabled,MCLR pin enabled)

// FICD
#pragma config ICS = PGx2//PGx1   // changed to get on chip debugger working DSB
// ICD Pin Placement Select bits (EMUC/EMUD
// share PGC2/PGD2)

// FDS
#pragma config DSWDTPS = DSWDTPSF // Deep Sleep Watchdog Timer Postscale Select
// bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC    // DSWDT Reference Clock Select bit (DSWDT
// uses Low Power RC Oscillator (LPRC))
#pragma config DSBOREN = OFF      // Deep Sleep Zero-Power BOR Enable bit (Deep
// Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON       // Deep Sleep Watchdog Timer Enable bit (DSWDT
// enabled)