//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_DEFAULTAUDIOBACKEND_H
#define GAMEENGINE_DEFAULTAUDIOBACKEND_H

#if defined(_WIN32)

#include "XAudio2/XAudio2Backend.h"
#include "XAudio2/XAudio2Player.h"

typedef XAudio2Backend DefaultAudioBackend;
typedef XAudio2Player DefaultAudioPlayer;

#elif defined(__APPLE__) && defined(__MACH__)

#include "CoreAudio/CoreAudioBackend.h"
#include "CoreAudio/CoreAudioPlayer.h"
typedef CoreAudioBackend DefaultAudioBackend;
typedef CoreAudioPlayer DefaultAudioPlayer;

#elif defined(__linux__)

#include "OpenAL/OpenALBackend.h"
#include "OpenAL/OpenALPlayer.h"
typedef OpenALBackend DefaultAudioBackend;
typedef OpenALPlayer DefaultAudioPlayer;

#endif

#endif //GAMEENGINE_DEFAULTAUDIOBACKEND_H
