/*
 * lab08.c
 *
 *  Created on:  Aug 13, 2018
 *  Modified on: Jul 19, 2019
 *  Modified on: May 29, 2020
 *      Author: khughes
 *
 *
 *      song is not stored like a map
 *
 */

#include <stdint.h>
#include <stdbool.h>

// Peripherals
#include "osc.h"
#include "ssi_uSD.h"
#include "lcd.h"
#include "ssi1_DAC.h"
#include "timer2A.h"
#include "UI.h" //not sure if ui header should be here as well
// FatFs
#include "ff.h"

// Utilities
#include "MP3utils.h"
#include "ID3.h"

// Other project files
#include "mp3.h"
#include "sound.h"
#include "control.h"

// ID3 tags
struct id3tags tags;

// Decode and play an MP3 file.
void playSong( uint8_t song  ) {
  FIL fp;

  // Get the file from the MicroSD card.
  findMP3( song, &fp );

  // Process ID3 header (if any).
  getID3Tags( &fp , &tags );

  // Prepare for sound output.
  initSound();

   // Decode and play the song.
  MP3decode( &fp );

   // Wait for the last data to be sent to the DACs.
  while( isSoundFinished() == false ) __asm( " wfi" );

  // Stop the DAC timer.
  enableTimer2A( false );
}

main() {
  // Initialize clock, SSIs, and Timer
  initOsc();
  initSSI3();
  initLCD( true );
  initSSI1();
  initTimer2A();
  initUI(); //call to ui file itself
  // Initialize structure.
  FATFS fs; f_mount( &fs, "", 0 );

  // Find out how many MP3 files are on the card.
  uint8_t numSongs = countMP3();
  setNumSongs( numSongs );

  // Obligatory endless loop.
  while( true ) {
    // Get the next file from the MicroSD card.
    uint8_t song = getSong();
    playSong( song );
//code meant to be revised later
    //write if statements for each possible keypress that could warrant a change
    //pseudocode -------------------------------------------------------------------
    if(user entered sequence){ //set song explicitly
        setSong(v); //v = value, not declared
    }
    //see if user chose to setDone
    //KEVIN: IMPLEMENTATION OF ADDITIONAL LCD INFO
    clearLCD(); //clear LCD so it loops
    positionLCD(0, 0); //should be upper left
    stringLCD("Now Playing: "+ getSong); //should get song name and place it in LCD
    positionLCD(7,7); //hopefully bottom right of screen, dont know max pixel amount
    stringLCD("VOL: " + volume); //uses volume from sound.c to show volume level
    if (paused == true){
        positionLCD(7,0);
        stringLCD("PAUSED");
    }
    else {
        position(7,0);
        stringLCD("PLAYING");
    }
//END "diSPLAY ADDITIONAL STATUS INFO"

    //send this to control.c and implement to getShuffle since getshuffle already checks
    if(isDone() == true){
        song = getSong();
        playSong(song);
    }


    //------------------------------------------------------------------------------
    // Send the file to the MP3 decoder

  }
}
