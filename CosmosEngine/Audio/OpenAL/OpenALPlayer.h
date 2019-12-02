//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_OPENALPLAYER_H
#define GAMEENGINE_OPENALPLAYER_H

#include "../AudioPlayer.h"

class ENGINE_LOCAL OpenALPlayer final
        : public AudioPlayer
{
public:

    OpenALPlayer();

    ~OpenALPlayer() final;

    void StartAudio() final;

    void PauseAudio() final;

    void StopAudio() final;

    void ClearBuffer() final;

    int AddBuffer(unsigned char* buffer, int bufferSize, bool finalBuffer) final;

    void Init(int sampleRate, int channels, int bytesPerSample) final;

    int GetAddedBufferCount() final;

    void DeInit() final;

    void WaitForBufferEnd() final;

    bool WaitForStreamEnd(float timeout) final;
};

#endif //GAMEENGINE_OPENALPLAYER_H
