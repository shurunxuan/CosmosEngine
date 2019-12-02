//
// Created by 西村智 on 12/1/19.
//

#include "CoreAudioBackend.h"
#include "CoreAudioPlayer.h"
#include "../../Logging/Logging.h"

CoreAudioPlayer::CoreAudioPlayer()
    : AudioPlayer()
{
    bufferEnd = true;
    bufferCount = 0;
}

CoreAudioPlayer::~CoreAudioPlayer()
{

}

void CoreAudioPlayer::Init(int sampleRate, int channels, int bytesPerSample)
{
    this->sampleRate = sampleRate;
    this->channels = channels;
    this->bytesPerSample = bytesPerSample;
    AVAudioEngine* engine = ((CoreAudioBackend*)presentedAudioBackend)->GetEngine();
    AVAudioMixerNode* mainMixerNode = ((CoreAudioBackend*)presentedAudioBackend)->GetMainMixerNode();
    CreatePlayerNode(&playerNode);
    CreateFormat(&format, this->sampleRate, this->channels);
    AttachNodeToEngine(playerNode, engine);
    ConnectNodeToNodeWithFormat(engine, playerNode, mainMixerNode, format);
}

void CoreAudioPlayer::DeInit()
{
    StopPlayerNode(playerNode);
}

void CoreAudioPlayer::StartAudio()
{
    StartPlayerNode(playerNode);
}

void CoreAudioPlayer::PauseAudio()
{
    PausePlayerNode(playerNode);
}

void CoreAudioPlayer::StopAudio()
{
    StopPlayerNode(playerNode);
}

void CoreAudioPlayer::ClearBuffer()
{
    bufferCount = 0;
}

int CoreAudioPlayer::AddBuffer(unsigned char* buffer, int bufferSize, bool finalBuffer)
{
    SetupAudioBuffer(buffer, bufferSize, playerNode, format,
            CoreAudioPlayer::SubAtomicInt, this);
    ++bufferCount;
    return bufferCount;
}

int CoreAudioPlayer::GetAddedBufferCount()
{
    return bufferCount;
}

void CoreAudioPlayer::SubAtomicInt(void* obj)
{
    auto* player = (CoreAudioPlayer*)obj;
    --(player->bufferCount);
    player->streamEndConditionVariable.notify_all();
    player->bufferEnd = true;
    player->bufferEndConditionVariable.notify_all();
}

void CoreAudioPlayer::WaitForBufferEnd()
{
    boost::unique_lock<boost::mutex> bufferEndLock(bufferEndMutex);
    bufferEndConditionVariable.wait(bufferEndLock, [this]{return bufferEnd;});
    bufferEnd = false;
    bufferEndLock.unlock();
    bufferEndConditionVariable.notify_all();
}

bool CoreAudioPlayer::WaitForStreamEnd(float timeout)
{
    boost::unique_lock<boost::mutex> streamEndLock(streamEndMutex);
    bool result = streamEndConditionVariable.wait_for(streamEndLock,
            boost::chrono::duration<float>(timeout),
                    [this]{return bufferCount == 0;});
    return result;
}
