//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_AUDIOPLAYER_H
#define GAMEENGINE_AUDIOPLAYER_H

#include "../Export.h"
#include "Decoder.h"
#include <boost/thread.hpp>
#include <boost/container/string.hpp>

class ENGINE_LOCAL AudioPlayer
{
public:
    AudioPlayer();

    virtual ~AudioPlayer();

    virtual void StartAudio() = 0;

    virtual void PauseAudio() = 0;

    virtual void StopAudio() = 0;

    virtual void AddBuffer(unsigned char* buffer, int bufferSize) = 0;

protected:

    virtual void Init(int sampleRate, int channels) = 0;

    virtual void DeInit() = 0;

    int sampleRate;

    int channels;
};

#endif //GAMEENGINE_AUDIOPLAYER_H
