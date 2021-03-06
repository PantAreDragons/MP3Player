/*
 * UI.c
 *
 *  Created on:  Nov 1, 2018
 *  Modified on: Aug 9, 2019
 *  Modified on: Oct 27, 2020
 *      Author: khughes
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Peripherals
#include "osc.h"
#include "gptm.h"
#include "gpio.h"
#include "ppb.h"
#include "sysctl.h"
#include "keypad.h"
#include "lookup.h"

// Various hooks for MP3 player
#include "control.h"
#include "UI.h"

// Definitions for function keys
enum keycmds_t {
  PLAY_PAUSE    = 'A',
  SHUFFLE       = 'B',
  VOLUME_UP     = 'C',
  VOLUME_DOWN   = 'D',
  SKIP_BACKWARD = 'E',
  SKIP_FORWARD  = 'F',
};

// Your keypad key assignments from Lab 4.
static const uint8_t keymap[4][4] = {
                                     {'1','2','3','A'},
                                     {'4','5','6','B'},
                                     {'7','8','9','C'},
                                     {'E','0','F','D'},
};

// Your keypad pin assignments from Lab 4.
const struct portinfo rowdef = {
                                //Kevin Acosta Rowdef
                                /*{GPIO_PIN_7, GPIO_PIN_6, GPIO_PIN_2, GPIO_PIN_0},
                                 GPIO_PORTK,
                                 */

};

const struct portinfo columndef = {
                                   //TODO: find coldef meaning
                                   //Kevin Acosta: Could not find columndef within my lab4 folder

};

// Return key value upon initial key press.  Since keys are
// ASCII, they cannot have a value greater than 0xff, so when
// no new key is pressed the procedure returns UINT16_MAX.
static uint16_t UIKey( void ) { 
  static enum {NOT_PRESSED, PRESSED} state = NOT_PRESSED;
  uint8_t column, row, key;

#define ADDR keymap
#define SIZE 4

  switch( state ) {
  case NOT_PRESSED:
    if( getKey( &column, &row ) == true ) {
      key = LOOKUP();
      state = PRESSED;
      return (uint16_t)key;
    }
    break;
  case PRESSED:
    if( getKey( &column, &row ) == false )
      state = NOT_PRESSED;
    break;
  }
  return UINT16_MAX;
}

// Interrupt handler for user interface.  When called it scans the
// keypad and if a new key is detected, performs necessary actions.
void UIHandler( void ) {
  uint16_t key = UIKey( ); 
  if( key != UINT16_MAX ) {
    switch( (enum keycmds_t)key ) {
    case PLAY_PAUSE:    // 'A'
      setPaused( isPaused() == false );
      break;
    case SHUFFLE:       // 'B'
      setShuffle( isShuffle() == false );
      break;
    case VOLUME_UP:     // 'C'
      upVolume();
      break;
    case VOLUME_DOWN:   // 'D'
      downVolume();
      break;
    case SKIP_BACKWARD: // 'E'
      break;
    case SKIP_FORWARD:  // 'F'
      setDone();
      break;
	case Queue:  //have to find out how queue will be selected to enable this
	  setQueue(isQueued() == false);
	  break;
    default:            // Numeric keys
      break;
    }
  }

  // Clear the time-out.
  GPTM_TIMER5[GPTM_ICR] |= GPTM_ICR_TATOCINT;
}

void initUI( void ) {
  // Prep the keypad.
  initKeypad ( &columndef, &rowdef );

  // Enable Run Clock Gate Control
  SYSCTL[SYSCTL_RCGCTIMER] |= SYSCTL_RCGCTIMER_TIMER5;
  SYSCTL[SYSCTL_RCGCTIMER] |= SYSCTL_RCGCTIMER_TIMER5;

  // 32-bit periodic timer.
  GPTM_TIMER5[GPTM_CFG] &= ~GPTM_CFG_M;
  GPTM_TIMER5[GPTM_CFG] |= GPTM_CFG_32BITTIMER;
  GPTM_TIMER5[GPTM_TAMR] &= ~GPTM_TAMR_TAMR_M;
  GPTM_TIMER5[GPTM_TAMR] |= GPTM_TAMR_TAMR_PERIODIC;

  // Set reload value for 20Hz
  GPTM_TIMER5[GPTM_TAILR] = (MAINOSCFREQ/20)-1;

  // Enable the interrupt.
  GPTM_TIMER5[GPTM_IMR] |= GPTM_IMR_TATOIM;

  // Enable interrupt for timer.
  PPB[PPB_EN2] |= PPB_EN2_TIMER5A;

  // Set priority level to 1 (lower priority than Timer2A).
  PPB[PPB_PRI16] = ( PPB[PPB_PRI16] & PPB_PRI_INTB_M ) | ( 1 << PPB_PRI_INTB_S );

  // Clear the time-out.
  GPTM_TIMER5[GPTM_ICR] |= GPTM_ICR_TATOCINT;

  // Enable the timer.
  GPTM_TIMER5[GPTM_CTL] |= GPTM_CTL_TAEN | GPTM_CTL_TASTALL;
}
