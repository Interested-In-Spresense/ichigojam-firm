/*
 *  player.ino - Simple sound player example application
 *  Copyright 2018 Sony Semiconductor Solutions Corporation
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <SDHCI.h>
#include <Audio.h>
#include <File.h>

SDClass theSD;
AudioClass* theAudio = NULL;

static char fname[32]="\0";

enum PlayerState {
  PlayerReady,
  PlayerStart,
  PlayerRun,
  PlayerStop
};

PlayerState player0_state = PlayerReady;



bool ErrEnd = false;

/**
 * @brief Audio attention callback
 *
 * When audio internal error occurs, this function will be called back.
 */

static void audio_attention_cb(const ErrorAttentionParam *atprm) {
  puts("Attention!");
  
  if (atprm->error_code >= AS_ATTENTION_CODE_WARNING) {
    ErrEnd = true;
  }
}



bool player_start(File& myFile) {

  /* Verify file open */
  if (!myFile.available()) {
    printf("Invalid File.\n");
    return false;
  }

  /* Send first frames to be decoded */
  int err = theAudio->writeFrames(AudioClass::Player0, myFile);

  if ((err != AUDIOLIB_ECODE_OK) && (err != AUDIOLIB_ECODE_FILEEND)) {
    printf("File Read Error! =%d\n",err);
    myFile.close();
    myFile = NULL;
    return false;
  }

  theAudio->setVolume(-40,-180,-80);
  theAudio->startPlayer(AudioClass::Player0);

  if (err == AUDIOLIB_ECODE_FILEEND) {
    usleep(10*1000);
    theAudio->stopPlayer(AudioClass::Player0, AS_STOPPLAYER_ESEND);
    myFile.close();
    myFile = NULL;
  }

  return true;

}

void play_process(int argc, FAR char *argv[])
{
  File myFile = NULL;

  while (1) {

    switch (player0_state) {

    case PlayerStart:
      myFile = theSD.open(fname);
      if(player_start(myFile)) {
        player0_state = PlayerRun;
      }else{
        player0_state = PlayerReady;
      }
      break;

    case PlayerRun:
      {
      err_t err = theAudio->writeFrames(AudioClass::Player0, myFile);

      if (err == AUDIOLIB_ECODE_FILEEND) {
        printf("Player%d File End!\n", AudioClass::Player0);
        theAudio->stopPlayer(AudioClass::Player0, AS_STOPPLAYER_ESEND);
        myFile.close();
        myFile = NULL;
        player0_state = PlayerReady;
      } else if (err != AUDIOLIB_ECODE_OK) {
        printf("Player%d error code: %d\n", AudioClass::Player0, err);
        theAudio->stopPlayer(AudioClass::Player0, AS_STOPPLAYER_NORMAL);
        myFile.close();
        myFile = NULL;
        player0_state = PlayerReady;
      }
      }
      break;

    case PlayerStop:
      theAudio->stopPlayer(AudioClass::Player0, AS_STOPPLAYER_NORMAL);
      usleep(20*1000);
      myFile.close();
      myFile = NULL;
      player0_state = PlayerReady;
      break;

    }

    usleep(10000);

  }

}

void audioplayer_init2() {
  // start audio system
  theAudio = AudioClass::getInstance();

  theAudio->begin(audio_attention_cb);

  puts("initialization Audio Library");

  /* Set clock mode to normal */
  theAudio->setRenderingClockMode(AS_CLKMODE_NORMAL);

  /* Set output device to speaker with first argument.
   * If you want to change the output device to I2S,
   * specify "AS_SETPLAYER_OUTPUTDEVICE_I2SOUTPUT" as an argument.
   * Set speaker driver mode to LineOut with second argument.
   * If you want to change the speaker driver mode to other,
   * specify "AS_SP_DRV_MODE_1DRIVER" or "AS_SP_DRV_MODE_2DRIVER" or "AS_SP_DRV_MODE_4DRIVER"
   * as an argument.
   */
  theAudio->setPlayerMode(AS_SETPLAYER_OUTPUTDEVICE_SPHP, AS_SP_DRV_MODE_LINEOUT);

  /*
   * Set main player to decode stereo mp3. Stream sample rate is set to "auto detect"
   * Search for MP3 decoder in "/mnt/sd0/BIN" directory
   */
  err_t err = theAudio->initPlayer(AudioClass::Player0, AS_CODECTYPE_MP3, "/mnt/sd0/BIN", AS_SAMPLINGRATE_AUTO, AS_CHANNEL_STEREO);

  /* Verify player initialize */
  if (err != AUDIOLIB_ECODE_OK) {
      printf("Player0 initialize error\n");
      //exit(1);
  }

  err = theAudio->initPlayer(AudioClass::Player1, AS_CODECTYPE_MP3, "/mnt/sd0/BIN", AS_SAMPLINGRATE_AUTO, AS_CHANNEL_STEREO);

  /* Verify player initialize */
  if (err != AUDIOLIB_ECODE_OK) {
      printf("Player1 initialize error\n");
      //exit(1);
  }

 /* Initialize task parameter. */

  /* Start task */
  const char *argv[3] = {0};

  task_create("player_thread0", 155, 2048, play_process, (char* const*)argv);
}

void audioplayer_init() {
  /* Initialize SD */
  while (!theSD.begin()) {
    Serial.println("Insert SD card.");
  }

  audioplayer_init2();
}


/**
 * @brief Setup audio player to play mp3 file
 *
 * Set clock mode to normal <br>
 * Set output device to speaker <br>
 * Set main player to decode stereo mp3. Stream sample rate is set to "auto detect" <br>
 * System directory "/mnt/sd0/BIN" will be searched for MP3 decoder (MP3DEC file)
 * Open "Sound.mp3" file <br>
 * Set master volume to -16.0 dB
 */
void audioplayer_play(const char* mp3fn) {

  puts("Play!");

  /* Open file placed on SD card */
  strcpy(fname,"sound0/");
  strcat(fname,mp3fn);

  player0_state = PlayerStart;

}

void audioplayer_stop() {

  puts("Stop!");

  player0_state = PlayerStop;

}

/**
 * @brief Setup audio player to play mp3 file
 *
 * Set clock mode to normal <br>
 * Set output device to speaker <br>
 * Set main player to decode stereo mp3. Stream sample rate is set to "auto detect" <br>
 * System directory "/mnt/sd0/BIN" will be searched for MP3 decoder (MP3DEC file)
 * Open "Sound.mp3" file <br>
 * Set master volume to -16.0 dB
 */
void audioplayer_sound(const char* mp3fn) {

  static  char fname[32]="\0";
  File myFile = NULL;

  int flg = myFile != NULL;
  if (myFile) {
    myFile.close();
    myFile = NULL;
  }
  if (ErrEnd) {
    ErrEnd = false;
    flg = 0;
    return;
  }

  /* Open file placed on SD card */
  strcpy(fname,"sound1/");
  strcat(fname,mp3fn);
  myFile = theSD.open(fname);

  /* Verify file open */
  if (!myFile) {
    printf("File open error\n");
    return;
  }
  printf("Open! 0x%08lx\n", (uint32_t)myFile);

  /* Send first frames to be decoded */
  int err = theAudio->writeFrames(AudioClass::Player1, myFile);

  if ((err != AUDIOLIB_ECODE_OK) && (err != AUDIOLIB_ECODE_FILEEND)) {
    printf("File Read Error! =%d\n",err);
    myFile.close();
    myFile = NULL;
    return;
  }

  if (flg) {
    return;
  }
  /* Main volume set to -16.0 dB */
  puts("Sound!");
  theAudio->setVolume(-40,-180,-80);
  theAudio->startPlayer(AudioClass::Player1);
  usleep(10*1000);
  theAudio->stopPlayer(AudioClass::Player1, AS_STOPPLAYER_ESEND);
  myFile.close();
  myFile = NULL;
}

