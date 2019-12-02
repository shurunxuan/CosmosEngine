//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_AUDIOPLAYER_H
#define GAMEENGINE_AUDIOPLAYER_H

#include "../Export.h"
#include "Decoder.h"

class ENGINE_LOCAL AudioPlayer
{
public:
    AudioPlayer();

    virtual ~AudioPlayer();

    virtual void StartAudio() = 0;

    virtual void PauseAudio() = 0;

    virtual void StopAudio() = 0;

    virtual void ClearBuffer() = 0;

    virtual void AddBuffer(unsigned char* buffer, int bufferSize) = 0;

    virtual void Init(int sampleRate, int channels) = 0;

    virtual void DeInit() = 0;

    virtual void WaitForBufferEnd() = 0;

    virtual bool WaitForStreamEnd(float timeout) = 0;

protected:

    int sampleRate;

    int channels;
};

#endif //GAMEENGINE_AUDIOPLAYER_H
