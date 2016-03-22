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
