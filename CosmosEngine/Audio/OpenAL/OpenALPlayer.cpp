//
// Created by 西村智 on 12/1/19.
//

#include "OpenALPlayer.h"
#include "../../Logging/Logging.h"
#include <boost/thread.hpp>

OpenALPlayer::OpenALPlayer()
        : AudioPlayer()
{
    bufferQueueSize = 0;
    bufferIndex = 0;
    source = 0;

    for (unsigned int & buffer : buffers)
    {
        buffer = 0;
    }

    streamGoingToEnd = false;
}

OpenALPlayer::~OpenALPlayer()
{

}

void OpenALPlayer::Init(int sampleRate, int channels, int bytesPerSample)
{
    this->sampleRate = sampleRate;
    this->channels = channels;
    this->bytesPerSample = bytesPerSample;
    streamGoingToEnd = false;
    alGenSources(1, &source);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    alSourcef(source, AL_GAIN, 1);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alGenBuffers(MAX_BUFFER_COUNT, buffers);
}

void OpenALPlayer::DeInit()
{
    alDeleteBuffers(16, buffers);
    alDeleteSources(1, &source);
}

void OpenALPlayer::StartAudio()
{
    alSourcePlay(source);
}

void OpenALPlayer::PauseAudio()
{
    alSourcePause(source);
}

void OpenALPlayer::StopAudio()
{
    alSourceStop(source);
}

void OpenALPlayer::ClearBuffer()
{
    ALuint unqueuedBuffers[MAX_BUFFER_COUNT];
    alSourceUnqueueBuffers(source, bufferQueueSize, unqueuedBuffers);
    bufferQueueSize = 0;
    streamGoingToEnd = false;
}

int OpenALPlayer::AddBuffer(unsigned char* buffer, int bufferSize, bool finalBuffer)
{
    alBufferData(buffers[bufferIndex],
            channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
            buffer, bufferSize, sampleRate);
    alSourceQueueBuffers(source, 1, &buffers[bufferIndex]);

    // OpenAL Source will automatically stop while no buffer is queued.
    ALint state = AL_INITIAL;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
    {
        alSourcePlay(source);
    }

    ++bufferIndex;
    if (bufferIndex >= MAX_BUFFER_COUNT) bufferIndex = 0;
    ++bufferQueueSize;
    streamGoingToEnd = finalBuffer;
    return bufferQueueSize;
}

int OpenALPlayer::GetAddedBufferCount()
{
    return bufferQueueSize;
}

void OpenALPlayer::WaitForBufferEnd()
{
    while (true)
    {
        boost::this_thread::sleep_for(boost::chrono::microseconds(10));
        ALint buffersProcessed = 0;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);
        if (buffersProcessed > 0)
        {
            ALuint unqueuedBuffers[MAX_BUFFER_COUNT];
            alSourceUnqueueBuffers(source, buffersProcessed, unqueuedBuffers);
            bufferQueueSize -= buffersProcessed;
            break;
        }
    }
}

bool OpenALPlayer::WaitForStreamEnd(float timeout)
{
    boost::chrono::time_point start = boost::chrono::high_resolution_clock::now();
    while (true)
    {
        boost::this_thread::sleep_for(boost::chrono::microseconds(10));
        ALint buffersProcessed = 0;
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &buffersProcessed);
        if (buffersProcessed == 0 && streamGoingToEnd) return true;
        boost::chrono::time_point current = boost::chrono::high_resolution_clock::now();
        if (boost::chrono::duration_cast<boost::chrono::milliseconds>(current - start).count() / 1000.0f >= timeout)
        {
            return false;
        }
    }
}