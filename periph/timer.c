/*
 * timer.c
 *
 *  Created on: Aug 3, 2020
 *      Author: khughes
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "ppb.h"

// The code relies on a symbol "USING_TIMER3A" that controls whether to 
// compile the SYSTICK code or your Timer3A code.  You will need to define 
// this symbol within the peripheral project configuration.  DO NOT define
// within this file!

#ifdef USING_TIMER3A
#include "sysctl.h"
#include "gptm.h"


#define PRESCALE 10
//assuming the prescale is 10, now going to put a register value of 10-1 in futures lines that use this definition
#else

#include "ppb.h"
// Move these definitions into a "ppb.h" header file.  Create it using the
// same format as sysctl.h and gpio.h.
// Frequency of switching between left and right LED display

/*#define PPB                     ((volatile uint32_t *) 0xe000e000)

enum {
  PPB_STCTRL = (0x10 >> 2),
#define   PPB_STCTRL_COUNT   (1<<16)
#define   PPB_STCTRL_CLK_SRC (1<<2)
#define   PPB_STCTRL_INTEN   (1<<1)
#define   PPB_STCTRL_ENABLE  (1<<0)
  PPB_STRELOAD,
  PPB_STCURRENT,
};
*/
#endif

// osc.h defines MAINOSCFREQ
#include "osc.h"

#define   SWITCHFREQ    200 //taken from number 5 on canvas lab page

// Add the calculation for your reload value here.
#define RELOAD_VALUE    (MAINOSCFREQ/SWITCHFREQ) //120M/200HZ
/*
 Code placed in periph.h
// Frequency of switching between left and right LED display
#define   SWITCHFREQ    200 //taken from number 5 on canvas lab page

// Add the calculation for your reload value here.
#define RELOAD_VALUE    (600000)
*/
// Configure Timer3A as a periodic timer with a period of 1/200th second.
// The names of each register you will need are defined in gptm.h; you do not 
// need to use any registers not named there.  You will need to define the 
// offsets for each bit or field in the registers.
//
// The "size" parameter should be either 16 or 32, depending on whether
// Timer3A should be configured as a 16-bit or 32-bit timer.
// 
// ALSO: remember to enable the Run Mode Clock Gating for the timer before
// attempting to access any of its registers!
void initTimer( uint8_t size ) {
#ifdef USING_TIMER3A
//regular C code can go in here if we set it
    //ex for timer2 as 32-bit periodic- taken from canvas video on topic of GPTM
    //config and utilization
//TODO: FIGURE OUT WHY THIS CODE IS NOT HIGHLIGHTED DESPITE DECLARATION IN GPTM.H
    //Configure as 32-bit timer
    //enable run clock gate control
    SYSCTL[SYSCTL_RCGCTIMER] |= SYSCTL_RCGCTIMER_3;
    SYSCTL[SYSCTL_RCGCTIMER] |= SYSCTL_RCGCTIMER_3;
     //DISABLE TIMER NOT NEEDED ON FIRST INITIALIZATION

    GPTM_TIMER3[GPTM_CTL] &= ~GPTM_CTL_TAEN;
    //CONFIGURE 32 BIT TIMER
    if (size == 32){
        //MASKING TO 32 BIT
        GPTM_TIMER3[GPTM_CFG] &= ~GPTM_CFG_M;
        GPTM_TIMER3[GPTM_CFG] |= GPTM_CFG_32BITTIMER;
        GPTM_TIMER3[GPTM_TAILR] = RELOAD_VALUE -1;
    }
    else{
        //MASKING TO 16 BIT
        GPTM_TIMER3[GPTM_CFG] &= ~GPTM_CFG_M;
        GPTM_TIMER3[GPTM_CFG] |= GPTM_CFG_16BITTIMERS;
        //how many counts do we want per period, coutning down to 0
        GPTM_TIMER3[GPTM_TAILR] = (RELOAD_VALUE/PRESCALE) -1;
        //100,000 for example
            //over 16 bit, divide by prescale
        //every two clock counts timer counts once
        GPTM_TIMER3[GPTM_TAILR] = PRESCALE -1;
    }
//PERIODIC
    GPTM_TIMER3[GPTM_TAMR] = (GPTM_TIMER3[GPTM_TAMR] & ~GPTM_TAMR_TAMR_M) | GPTM_TAMR_TAMR_PERIODIC;
//RELOAD VALUE

//ENABLE TIMER
    //divide reload value by 2, prescalar would be 2-1
    GPTM_TIMER3[GPTM_CTL] |= GPTM_CTL_TAEN | GPTM_CTL_TASTALL;

//CODE DOES NOT CURRENTLY COMPILE DUE TO UNDERLYING ISSUES WITHIN THE lab002.c FILE
//TODO: Insure proper designation of timer attributes respective to
    //that of bits (16 or 32)

#else

  PPB[PPB_STCTRL] = 0;
  PPB[PPB_STRELOAD] = RELOAD_VALUE-1;
  PPB[PPB_STCTRL] = PPB_STCTRL_CLK_SRC | PPB_STCTRL_ENABLE;

#endif
}

// Wait for Timer3A to reload, then return.  Perform any necessary actions to
// clear the reload condition.  DO NOT turn off the timer!
void waitOnTimer( void ) {
#ifdef USING_TIMER3A
//WAIT FOR TIMEOUT FLAG FOR OVERFLOW
    while( (GPTM_TIMER3[GPTM_RIS] & GPTM_RIS_TATORIS) == 0);

    //CLEARS TIMER A TIM-OUT FLAG, without this, it wont execute
    GPTM_TIMER3[GPTM_ICR] = GPTM_ICR_TATOCINT;
//TODO: FIGURE OUT REMAINING CODE NEEDED HERE
#else

  while( ( PPB[PPB_STCTRL] & PPB_STCTRL_COUNT ) == 0 );

#endif
}
