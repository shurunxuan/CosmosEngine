//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_COREAUDIOPLAYER_H
#define GAMEENGINE_COREAUDIOPLAYER_H

#include "../AudioPlayer.h"
#include "CoreAudioBackendBridge.h"
#include <boost/atomic.hpp>
#include <boost/thread.hpp>

class ENGINE_LOCAL CoreAudioPlayer final
        : public AudioPlayer
{
public:

    CoreAudioPlayer();

    ~CoreAudioPlayer() final;

    void StartAudio() final;

    void PauseAudio() final;

    void StopAudio() final;

    void ClearBuffer() final;

    int AddBuffer(unsigned char* buffer, int bufferSize, bool finalBuffer) final;

    int GetAddedBufferCount() final;

    void Init(int sampleRate, int channels, int bytesPerSample) final;

    void DeInit() final;

    void WaitForBufferEnd() final;

    bool WaitForStreamEnd(float timeout) final;

private:

    static void SubAtomicInt(void* obj);

    AVAudioPlayerNode* playerNode;

    AVAudioFormat* format;

    boost::atomic<int> bufferCount;

    boost::mutex bufferEndMutex;

    boost::condition_variable bufferEndConditionVariable;

    boost::mutex streamEndMutex;

    boost::condition_variable streamEndConditionVariable;

    bool bufferEnd;

    bool streamGoingToEnd;
};

#endif //GAMEENGINE_COREAUDIOPLAYER_H
