//
// Created by 西村智 on 12/1/19.
//

#include "XAudio2Player.h"
#include "XAudio2Backend.h"
#include "../../Logging/Logging.h"

XAudio2Player::XAudio2Player()
        : AudioPlayer()
{

}

XAudio2Player::~XAudio2Player()
{

}

void XAudio2Player::Init(int sampleRate, int channels, int bytesPerSample)
{
    ((XAudio2Backend*) presentedAudioBackend)->
            CreateSourceVoice(&sourceVoice, channels, sampleRate, bytesPerSample, &callback);
    //UpdateX3DAudioSettings();
}

void XAudio2Player::DeInit()
{
    sourceVoice->DestroyVoice();
}

void XAudio2Player::StartAudio()
{
    sourceVoice->Start();
}

void XAudio2Player::PauseAudio()
{
    sourceVoice->Stop();
}

void XAudio2Player::StopAudio()
{
    sourceVoice->Stop();
    SetEvent(callback.bufferEvent);
}

void XAudio2Player::ClearBuffer()
{
    sourceVoice->FlushSourceBuffers();
}

int XAudio2Player::AddBuffer(unsigned char* buffer, int bufferSize, bool finalBuffer)
{
    UINT flag = 0;
    if (finalBuffer)
    {
        flag = XAUDIO2_END_OF_STREAM;
    }
    // Submit the buffer
    XAUDIO2_BUFFER xAudio2Buffer = {flag};
    xAudio2Buffer.AudioBytes = bufferSize;
    xAudio2Buffer.pAudioData = buffer;
    sourceVoice->SubmitSourceBuffer(&xAudio2Buffer);
    // Get the state for the queued buffer count
    XAUDIO2_VOICE_STATE voiceState = {};
    sourceVoice->GetState(&voiceState);
    return voiceState.BuffersQueued;
}

int XAudio2Player::GetAddedBufferCount()
{
    XAUDIO2_VOICE_STATE voiceState = {};
    sourceVoice->GetState(&voiceState);
    return voiceState.BuffersQueued;
}

void XAudio2Player::WaitForBufferEnd()
{
    WaitForSingleObject(callback.bufferEvent, INFINITE);
}

bool XAudio2Player::WaitForStreamEnd(float timeout)
{
    return WaitForSingleObject(callback.streamEvent, DWORD(timeout * 1000.0f)) != WAIT_TIMEOUT;
}