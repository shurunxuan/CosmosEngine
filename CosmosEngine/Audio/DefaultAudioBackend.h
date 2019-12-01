//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_DEFAULTAUDIOBACKEND_H
#define GAMEENGINE_DEFAULTAUDIOBACKEND_H

#if defined(_WIN32)

#include "XAudio2/XAudio2Backend.h"
typedef XAudio2Backend DefaultAudioBackend;

#elif defined(__APPLE__) && defined(__MACH__)

#include "CoreAudio/CoreAudioBackend.h"
typedef CoreAudioBackend DefaultAudioBackend;

#elif defined(__linux__)

#include "OpenAL/OpenALBackend.h"
typedef OpenALBackend DefaultAudioBackend;

#endif

#endif //GAMEENGINE_DEFAULTAUDIOBACKEND_H
