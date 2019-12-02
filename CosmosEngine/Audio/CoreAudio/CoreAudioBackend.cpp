//
// Created by 西村智 on 12/1/19.
//

#include "CoreAudioBackend.h"
#include "CoreAudioBackendBridge.h"
#include "../../Logging/Logging.h"

CoreAudioBackend::CoreAudioBackend()
{
    presentedAudioBackend = this;
}

CoreAudioBackend::~CoreAudioBackend()
{

}

bool CoreAudioBackend::Init()
{
    LOG_INFO << "Initializing CoreAudio Backend";
    CreateEngine(&engine);
    GetMainMixerNodeFromEngine(engine, &mainMixerNode);
    StartEngine(engine);
    return false;
}

void CoreAudioBackend::Calculate3DAudio(float deltaTime, float totalTime)
{

}

void CoreAudioBackend::DeInit()
{
    StopEngine(engine);
    LOG_INFO << "CoreAudio Audio Backend Shutdown Completed.";
}

AVAudioEngine* CoreAudioBackend::GetEngine()
{
    return engine;
}

AVAudioMixerNode* CoreAudioBackend::GetMainMixerNode()
{
    return mainMixerNode;
}

bool CoreAudioBackend::IsFloat()
{
    return true;
}

bool CoreAudioBackend::Force32Bit()
{
    return true;
}
