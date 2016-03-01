/* File:    DisplayUART.c
 * Authors: Dan Baker
 *          Nathan Chaney
 */

/* Includes *******************************************************************/

#include <p24FV32KA301.h>
#include "../Shared/ExternSharedDefinitions.h"

/* Functions ******************************************************************/

void initUART(void) {
  // FCY = 2 MHz;
  // BAUD = FCY / 16 / (U2BRG + 1)
  // Ok to be slightly off
  // U2BRG = 1;  // baud rate is 62500
  // U2BRG = 3;  // baud rate is 31250
  // U2BRG = 6;  // baud rate is 17829
   U2BRG = 12; // baud rate is 9615
  // U2BRG = 25; // baud rate is 4800
  // U2BRG = 50; // baud rate is 2447


    U2MODEbits.USIDL = 0b0;
    U2MODEbits.IREN = 0b0;
    U2MODEbits.RTSMD = 0b1;
    U2MODEbits.UEN = 0b00;
    U2MODEbits.WAKE = 0b0;
    U2MODEbits.LPBACK = 0b0;
    U2MODEbits.ABAUD = 0b0;
    U2MODEbits.RXINV = 0b0;
    U2MODEbits.BRGH = 0b0;
    U2MODEbits.PDSEL = 0b00;
    U2MODEbits.STSEL = 0b0;

    U2STAbits.UTXISEL0 = 0b0;
    U2STAbits.UTXISEL1 = 0b0;
    U2STAbits.UTXINV = 0b0;
    U2STAbits.UTXBRK = 0b0;
    U2STAbits.URXISEL1 = 0b0; // interrupt after one RX character is received
    U2STAbits.ADDEN = 0;

    //interrupts
    _U2RXIP = 1;     // set RX interrupt priority (1-7, default 4)
    _U2ERIF = 0;     // clear Error Flag
    _U2ERIE = 1;     // enable Error Interrupt
    _U2TXIF = 0;     // clear TX Flag
    _U2TXIE = 0;     // disable TX Interrupt
    _U2RXIF = 0;     // clear RX interrupt flag
    _U2RXIE = 1;     // enable RX interrupt

    U2MODEbits.UARTEN = 0b1; // turn it on
    U2STAbits.UTXEN   = 0b1; // enable transmit

}

void __attribute__((__interrupt__,__no_auto_psv__)) _U2RXInterrupt(void) {

    unsigned char tempChar;

    U2STAbits.OERR = 0;

    while (U2STAbits.URXDA == 1) { // something to receive

        tempChar = U2RXREG;

        if (tempChar > 31 && tempChar < 127) { // ignore junk characters
            if (tempChar > 64 && tempChar < 91)
                tempChar += 32;
            receiveBufferU2[receiveIndexU2++] = tempChar;
        }

        if (receiveIndexU2 > RECEIVE_STRING_LENGTH - 1 || tempChar == '.' || tempChar == 0) {
            while (receiveIndexU2 < RECEIVE_STRING_LENGTH)
                receiveBufferU2[receiveIndexU2++] = 0;
            receiveIndexU2 = 0;

            stringCopy(receiveBufferU2, receiveBuffer());
            newReceiveData = 1;
        }
    }

    _U2RXIF = 0; // clear interrupt flag
}

void __attribute__((__interrupt__,__no_auto_psv__)) _U2ErrInterrupt(void) {
//    U2MODEbits.UARTEN = 0b0;
//    delayMS(1);
//    U2MODEbits.UARTEN = 0b1;
    U2STAbits.PERR;
    U2STAbits.FERR;
    U2STAbits.OERR = 0;

    _U2ERIF = 0; // clear error interrupt flag
}
