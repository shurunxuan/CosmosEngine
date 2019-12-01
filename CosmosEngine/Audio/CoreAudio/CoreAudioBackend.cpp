//
// Created by 西村智 on 12/1/19.
//

#include "CoreAudioBackend.h"
#include "CoreAudioBackendBridge.h"
#include "../../Logging/Logging.h"

CoreAudioBackend::CoreAudioBackend()
{

}

CoreAudioBackend::~CoreAudioBackend()
{

}

bool CoreAudioBackend::Init()
{
    LOG_INFO << "Initializing CoreAudio Backend";
    CreateEngine(&engine);
    return false;
}

void CoreAudioBackend::Calculate3DAudio(float deltaTime, float totalTime)
{

}

void CoreAudioBackend::DeInit()
{

}
