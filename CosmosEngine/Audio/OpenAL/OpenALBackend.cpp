//
// Created by 西村智 on 12/1/19.
//

#include "OpenALBackend.h"

OpenALBackend::OpenALBackend()
        : AudioBackend()
{

}

OpenALBackend::~OpenALBackend()
{

}

bool OpenALBackend::Init()
{
    return false;
}

void OpenALBackend::Calculate3DAudio(float deltaTime, float totalTime)
{

}

void OpenALBackend::DeInit()
{

}

bool OpenALBackend::IsFloat()
{
    return true;
}

bool OpenALBackend::Force32Bit()
{
    return false;
}