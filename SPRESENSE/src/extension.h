// Copyright 2014-2024 the IchigoJam authors. All rights reserved. MIT license.

#ifndef SUBCORE

#include "audioplayer.h"

#define TOKEN_SPR_PLAY   1000
#define TOKEN_SPR_STOP   1001
#define TOKEN_SPR_SOUND  1002
#define TOKEN_SPR_CAMERA 1003

void audioplayer_play(const char* fn);

void extension_init() {
  audioplayer_init();
}

#define EXT_COMMAND_LENGTH 8

int extension_parse() { // if match ret code else ret 0
  char buffer[EXT_COMMAND_LENGTH] = {0};

  const char* ext = "SPR.";
  for (int i = 0; i < strlen("SPR."); i++) {
    if (basic_toupper(*(pc + i)) != *(ext + i)) {
      return 0;
    }
  }
  pc += strlen("SPR.");

  for (int i = 0; i < EXT_COMMAND_LENGTH; i++) {
    buffer[i] = basic_toupper(*(pc + i));
  }

  if(strncmp(buffer,"PLAY",strlen("PLAY")) == 0 ) {
    pc += strlen("PLAY");
    return TOKEN_SPR_PLAY;
  }else if(strncmp(buffer,"STOP",strlen("STOP")) == 0 ){
    pc += strlen("STOP");
    return TOKEN_SPR_STOP;
  }else if(strncmp(buffer,"SOUND",strlen("SOUND")) == 0 ){
    pc += strlen("SOUND");
    return TOKEN_SPR_SOUND;
  }else if(strncmp(buffer,"CAM",strlen("CAM")) == 0 ){
    pc += strlen("CAM");
    return TOKEN_SPR_CAMERA;
  }

  return 0;
}

int extension_command(int token) { // if match ret 1

  switch (token){
  case TOKEN_SPR_PLAY:
    {int16 n = token_expression();
    if (n >= OFFSET_RAMROM) { // do not use.
      const char* p = (char*)(ram + n - OFFSET_RAMROM);
      char fn[128];
      fn[127] = '\0';
      for (int i = 0; i < 128 - 5; i++) {
        if (p[i] == '"' || p[i] == '\0') {
          fn[i++] = '.';
          fn[i++] = 'm';
          fn[i++] = 'p';
          fn[i++] = '3';
          fn[i++] = '\0';
          break;
        }
        fn[i] = p[i];
      }
      audioplayer_play(fn);
    } else if (n >= 0 && n < 10) {
      char fn[128];
      int i = 0;
      fn[i++] = '0' + n;
      fn[i++] = '.';
      fn[i++] = 'm';
      fn[i++] = 'p';
      fn[i++] = '3';
      fn[i++] = '\0';
      audioplayer_play(fn);
    }
    token_end();
    return 1;}

  case TOKEN_SPR_SOUND:
    {int16 n = token_expression();

    if (n >= 0 && n < 10) {
      char fn[128];
      int i = 0;
      fn[i++] = '0' + n;
      fn[i++] = '.';
      fn[i++] = 'm';
      fn[i++] = 'p';
      fn[i++] = '3';
       fn[i++] = '\0';
      audioplayer_sound(fn);
    }
    token_end();
    return 1;}

  case TOKEN_SPR_STOP:
    puts("play stop.");
    audioplayer_stop();
    token_end();
    return 1;
  }

  return 0;
}

#else  /* SUBCORE */

void extension_init() {
}

int extension_parse() { // if match ret code else ret 0
}

int extension_command(int token) { // if match ret 1
  return 0;
}

#endif /* SUBCORE */
