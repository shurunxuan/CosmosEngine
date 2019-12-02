//
// Created by 西村智 on 12/1/19.
//

#include "OpenALPlayer.h"

OpenALPlayer::OpenALPlayer()
        : AudioPlayer()
{

}

OpenALPlayer::~OpenALPlayer()
{

}

void OpenALPlayer::Init(int sampleRate, int channels, int bytesPerSample)
{
    this->sampleRate = sampleRate;
    this->channels = channels;
    this->bytesPerSample = bytesPerSample;
}

void OpenALPlayer::DeInit()
{

}

void OpenALPlayer::StartAudio()
{

}

void OpenALPlayer::PauseAudio()
{

}

void OpenALPlayer::StopAudio()
{

}

void OpenALPlayer::ClearBuffer()
{

}

int OpenALPlayer::AddBuffer(unsigned char* buffer, int bufferSize, bool finalBuffer)
{
    return 16;
}

int OpenALPlayer::GetAddedBufferCount()
{
    return 16;
}

void OpenALPlayer::WaitForBufferEnd()
{

}

bool OpenALPlayer::WaitForStreamEnd(float timeout)
{
    return true;
}