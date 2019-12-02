//
// Created by 西村智 on 12/1/19.
//

#include "CoreAudioBackend.h"
#include "CoreAudioPlayer.h"
#include "../../Logging/Logging.h"

CoreAudioPlayer::CoreAudioPlayer()
    : AudioPlayer()
{

}

CoreAudioPlayer::~CoreAudioPlayer()
{

}

void CoreAudioPlayer::Init(int sampleRate, int channels)
{
    this->sampleRate = sampleRate;
    this->channels = channels;
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

void CoreAudioPlayer::AddBuffer(unsigned char* buffer, int bufferSize)
{
    SetupAudioBuffer(buffer, bufferSize, playerNode, format,
            CoreAudioPlayer::SubAtomicInt, this);
    ++bufferCount;
}

void CoreAudioPlayer::SubAtomicInt(void* obj)
{
    --(((CoreAudioPlayer*)obj)->bufferCount);
}
