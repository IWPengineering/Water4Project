/* File:    DisplayRTCC.c
 * Authors: Dan Baker
 *          Nathan Chaney
 */


/* Includes *******************************************************************/

#include <p24FV32KA301.h>
#include "../Shared/ExternSharedDefinitions.h"

char BcdToDec(char val);
char DecToBcd(char val);

/* Functions ******************************************************************/

/* readTime
 * reads time from RTCC, writes to global time variables
 */
void readTime(void) {
    unsigned int temp;

    _RTCPTR = 0b11;
    timeYear = RTCVAL; //11
    temp = RTCVAL; //10
    timeMonth = temp >> 8;
    timeDay = temp & 0xFF;
    timeHour = RTCVAL & 0xFF; //01
    temp = RTCVAL; //00
    timeMinute = temp >> 8;
    timeSecond = temp & 0xFF;

    timeYear   = BcdToDec(timeYear);
    timeMonth  = BcdToDec(timeMonth);
    timeDay    = BcdToDec(timeDay);
    timeHour   = BcdToDec(timeHour);
    timeMinute = BcdToDec(timeMinute);
    timeSecond = BcdToDec(timeSecond);
}

char writeTime(char newYear, char newMonth, char newDay, char newHour, char newMinute, char newSecond) {

    switch(tempMonth) {
        case 4:
        case 6:
        case 9:
        case 11:
            if(tempDay > 30)
                return -1;
            break;

        case 2:
            if(tempYear % 4) {
                if(tempDay > 28)
                    return -1;
            } else {
                if(tempDay > 29)
                    return -1;
            }
            break;
    }

    // put values into BCD
    newYear   = DecToBcd(newYear);
    newMonth  = DecToBcd(newMonth);
    newDay    = DecToBcd(newDay);
    newHour   = DecToBcd(newHour);
    newMinute = DecToBcd(newMinute);
    newSecond = DecToBcd(newSecond);

    _RTCEN = 0; // disable clock

    //does unlock sequence to enable write to RTCC, sets RTCWEN
    __builtin_write_RTCWEN();

    _RTCPTR = 0b11;                                // decrements with read or write
    RTCVAL = newYear;                              // set year
    RTCVAL = (((int) newMonth) << 8) + newDay;     // set month, day
    RTCVAL = newHour;                              // clear weekday, set hour
    RTCVAL = (((int) newMinute) << 8) + newSecond; // set minute, second

    _RTCEN = 1;   // enable clock

    _RTCWREN = 0; // Disable Writing

    resetTime = 59;

    return 1;
}

char BcdToDec(char val) {
    return (val / 16 * 10) + (val % 16);
}

char DecToBcd(char val) {
    return (val / 10 * 16) + (val % 10);
}
