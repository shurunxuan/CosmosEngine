//
// Created by shuru on 12/2/2019.
//

#include "XAudio2Callback.h"

XAudio2Callback::XAudio2Callback() :
        bufferEvent(CreateEvent(nullptr, FALSE, FALSE, nullptr)),
        streamEvent(CreateEvent(nullptr, FALSE, FALSE, nullptr))
{

}

XAudio2Callback::~XAudio2Callback()
{
    CloseHandle(bufferEvent);
    CloseHandle(streamEvent);
}

void XAudio2Callback::OnStreamEnd()
{
    SetEvent(streamEvent);
}

void XAudio2Callback::OnVoiceProcessingPassEnd()
{
}

void XAudio2Callback::OnVoiceProcessingPassStart(UINT32 samples)
{
}

void XAudio2Callback::OnBufferEnd(void* context)
{
    SetEvent(bufferEvent);
}

void XAudio2Callback::OnBufferStart(void* context)
{
}

void XAudio2Callback::OnLoopEnd(void* context)
{
}

void XAudio2Callback::OnVoiceError(void* context, HRESULT Error)
{
}