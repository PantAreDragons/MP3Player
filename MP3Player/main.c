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

  //Cordero: Saving input sequence for later
  uint16_t key1 = 'X';
  uint16_t key2 = 'X';
  uint8_t song_out;


  // Obligatory endless loop.
  while( true ) {
    // Get the next file from the MicroSD card.
    uint8_t song = getSong();
    playSong( song );
//code meant to be revised later
    //write if statements for each possible keypress that could warrant a change
    //pseudocode -------------------------------------------------------------------
       while(isPaused()){ //MP3 must be paused to allow input sequence.
        if (UIkey() = 'E'){
            if ((key1 == 'X') && (key2 == 'X')){ //No value entered or we cleared it.
                setPaused(false);                   //Prepare to resume playing after 'E' key
                break;                              //Exit loop, resume normal function
            }
            else {
            song_out = (uint8_t)(key2 >> 8);    // casting key2 as 8 bit and discarding the upper 8 bits after shifting
            song_out *= 10;                     // multiplying result by 10 to get 10's place
            song_out += (uint8_t)(key1 >> 8);   //adding 1's place to result
            song_out -= 1;                      //Machines start at 00. A human's not going to know better. So song "1" is "0"
                if (song_out <= numSongs){           //Check to make sure song number is valid
                setSong(song_out);                //If true, set the song
                }
            setPaused(false);                   //End
            }
        if (UIkey() = 'C'){ //return to false value. Not planning to load 100 songs on this thing.
        key1 = 'X';
        key2 = 'X';
        }
        else{                   //I'm only planning to operate on double-digits for this MP3 player.
        key2 = key1;            //Cycles the previous keypress (if valid) into leftmost position
        key1 = UIkey();         //Obtain new keypress and save
        }
        }
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
