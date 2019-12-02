//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_COREAUDIOPLAYER_H
#define GAMEENGINE_COREAUDIOPLAYER_H

#include "../AudioPlayer.h"
#include "CoreAudioBackendBridge.h"
#include <boost/atomic.hpp>

class ENGINE_LOCAL CoreAudioPlayer final
        : AudioPlayer
{
public:

    CoreAudioPlayer();

    ~CoreAudioPlayer() final;

    void StartAudio() final;

    void PauseAudio() final;

    void StopAudio() final;

    void AddBuffer(unsigned char* buffer, int bufferSize) final;

private:

    void Init(int sampleRate, int channels) final;

    void DeInit() final;

    static void SubAtomicInt(void* obj);

    AVAudioPlayerNode* playerNode;

    AVAudioFormat* format;

    boost::atomic<int> bufferCount;
};

#endif //GAMEENGINE_COREAUDIOPLAYER_H
