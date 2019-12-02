//
// Created by 西村智 on 12/1/19.
//

#include "AudioBackend.h"
#include "../Logging/Logging.h"

AudioBackend* presentedAudioBackend = nullptr;

AudioBackend::AudioBackend()
{
    presentedAudioBackend = this;
}

AudioBackend::~AudioBackend()
{

}

bool AudioBackend::StartUp()
{
    LOG_INFO << "Initializing Audio Backend";

    Init();

    return true;
}

void AudioBackend::Update(float deltaTime, float totalTime)
{
    Calculate3DAudio(deltaTime, totalTime);
}

void AudioBackend::Shutdown()
{
    DeInit();

    LOG_INFO << "Audio Backend Shutdown Completed.";
}
