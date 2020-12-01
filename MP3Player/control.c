/*
 * control.c
 *
 *  Created on:  Aug 13, 2018
 *  Modified on: May 29, 2020
 *      Author: khughes
 */

#include <stdint.h>
#include <stdbool.h>

#include "control.h"

// The total number of songs on the MicroSD card.
static uint8_t numSongs;
// The next song to play.
static uint8_t song = 0;
// Indicates termination of current song by user.
static bool done = false;
// Indicates whether decoding/playing is playing or paused.
static bool paused = false;
// Indicates if the MP3 player is in song shuffle mode.
static bool shuffle = false;

static bool queuedSong = false;

static uint8_t previous1;
static uint8_t previous2;
static uint8_t previous3;


static uint8_t queueMusic(uint8_t chosenOne)
{
	//Lets see if we can get the music to queue one ahead.
	//variable user will need to choose
	if (chosenOne != NULL)
	{
		
		previous3 = previous2;  //pushes songs through the variables
		previous2 = previous1;
		previous1 = song;       //stores the music that was last and returns the chosen song

		return song;

	}

	else
	{
		previous3 = previous2;  //pushes songs through the variables
		previous2 = previous1;
		previous1 = song;
		song = song++;

		return song;
	}
		
}
// Private procedure for selecting the next song in shuffle mode.
static uint8_t getShuffle( uint8_t song ) {
	uint8_t randSong;

	randSong = GPTM_TIMER3[GPTM_TAV] % numSongs;
	songSums = previous1 + previous2 + previous3;

	if (songSums == (numSongs * 3)) // if no songs are in the variables; set from setNumSongs()
	{
		previous1 = song;

	}
	else {
		previous3 = previous2;  //pushes songs through the variables
		previous2 = previous1;
		previous1 = song;
	}
  return randSong; //since here we want to return the song we found out
}

// Return the number of the song to play.  Initially, just
// return the next sequential song, and wrap back to 0 when all
// songs have been returned.
uint8_t getSong( void ) {
  static bool initial = false;

  // On the first call, always return the first song.
  if( initial == false ) {
    initial = true;
    return song = 0;
  }

  if (queuedSong == true)
  {
	  queueMusic()//come back to this
  }

  // Otherwise pick the next song to play.
  if( shuffle == false ) {
    song = ( song + 1 ) % numSongs;
  } else {
    song = getShuffle( song );
  }

  // Return song number.
  return song;
}

// Store the total number of songs (needed by getSong()).
void setNumSongs( uint8_t num ) {
  numSongs = num;
}

// Store the total number of songs (needed by getSong()).
uint8_t getNumSongs( void ) {
  return numSongs;
}

// Set next song explicitly.
void setSong( uint8_t v ) {
  song = v;
}

// Indicates whether the current MP3 song should be terminated early.
bool isDone( void ) {
  bool tmp = done;
  done = false;
  return tmp;
}

// Set song to terminate.
void setDone( void ) {
  done = true;
}

// Indicates whether the MP3 player is paused.
bool isPaused( void ) {
  return paused;
}

// Set state of pause.
void setPaused( bool v ) {
  paused = v;
}

// Indicates state of shuffle mode.
bool isShuffle( void ) {
  return shuffle;
}

// Set state of shuffle mode.
void setShuffle( bool v ) {
  shuffle = v;
}

void setQueue(bool select) {
	queuedSong = select;
}
bool isQueued(void) {
	return queue;
}

// previous song
void prevSong( void ) {
	if (shuffle == true) 
	{
		//need a boolean variable to set when it was shuffled to undo the shuffle
		if (previous1 + previous2 + previous3 == (numSongs*3)) //checking if songs are stored in each of the previous songs
		{
			song = (numSongs + (song - 1)) % numSongs;            
			setDone();                                      
			setSong(song);
		}
		else {
			song = previous1;
			previous1 = previous2;
			previous2 = previous3;
			//previous3 = something
			setDone();
			setSong(song);
		}
	}
	
	else if (shuffle == false)
	{
		setDone();
		song = (song - 1) % numSongs;
		//setDone();
		setSong(song);
	}


}
