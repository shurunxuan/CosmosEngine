//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_XAUDIO2PLAYER_H
#define GAMEENGINE_XAUDIO2PLAYER_H

#include "../AudioPlayer.h"

class ENGINE_LOCAL XAudio2Player final
        : public AudioPlayer
{
public:

    XAudio2Player();

    ~XAudio2Player() final;

    void StartAudio() final;

    void PauseAudio() final;

    void StopAudio() final;

    void ClearBuffer() final;

    void AddBuffer(unsigned char* buffer, int bufferSize) final;

    void Init(int sampleRate, int channels) final;

    void DeInit() final;

    void WaitForBufferEnd() final;

    void WaitForStreamEnd(float timeout) final;
};

#endif //GAMEENGINE_XAUDIO2PLAYER_H
