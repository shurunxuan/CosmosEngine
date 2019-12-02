//
// Created by 西村智 on 12/1/19.
//

#include "OpenALBackend.h"
#include "../../Logging/Logging.h"

OpenALBackend::OpenALBackend()
        : AudioBackend()
{
    device = nullptr;
    context = nullptr;
}

OpenALBackend::~OpenALBackend()
{

}

bool OpenALBackend::Init()
{
    LOG_INFO << "Initializing OpenAL Backend";

    device = alcOpenDevice(nullptr);
    if (device == nullptr)
    {
        LOG_ERROR << "Failed to init OpenAL device.";
        return false;
    }

    context = alcCreateContext(device, nullptr);
    if (!alcMakeContextCurrent(context))
    {
        LOG_ERROR << "Failed to make default context";
        return false;
    }

    return true;
}

void OpenALBackend::Calculate3DAudio(float deltaTime, float totalTime)
{

}

void OpenALBackend::DeInit()
{
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);

    LOG_INFO << "OpenAL Audio Backend Shutdown Complete";
}

bool OpenALBackend::IsFloat()
{
    return false;
}

int OpenALBackend::ForceBitsPerSample()
{
    return 16;
}