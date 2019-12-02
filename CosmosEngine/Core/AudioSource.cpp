//
// Created by 西村智 on 12/1/19.
//

#include "AudioSource.h"
#include "../Logging/Logging.h"

AudioSource::AudioSource(Object* owner) :
        Component(owner), player(nullptr), isPlaying(false),
        stopped(true), fileOpened(false), channels(0),
        sampleRate(0), bytesPerSample(0), Loop(false)
{

}

AudioSource::~AudioSource()
{
    if (player != nullptr)
    {
        // Stop the source voice
        player->StopAudio();
        player->ClearBuffer();

        playbackThread.interrupt();
        playbackThread.join();

        // Destroy source voice
        player->DeInit();
    }
}

void AudioSource::Start()
{
    player = new DefaultAudioPlayer();
}

void AudioSource::Update(float deltaTime, float totalTime)
{

}

void AudioSource::LoadAudioFile(const boost::container::string& filename)
{
    if (fileOpened) return;
    this->filename = filename;
    // Open file with FFmpeg, and initialize codecs
    int res = ffmpeg.OpenFile(this->filename.c_str());
    fileOpened = res >= 0;

    if (!fileOpened) return;

    // Parameters to be used when creating source voice
    // Initialize the resampler, and get the parameters required
    ffmpeg.InitSoftwareResampler(&channels, &sampleRate, &bytesPerSample);

    player->Init(sampleRate, channels, bytesPerSample);
}

void AudioSource::Play()
{
    if (isPlaying)
    {
        return;
    }
    else
    {
        if (stopped)
        {
            // stopped
            playbackThread = boost::thread(&AudioSource::PlaySync, this);
        }
        else
        {
            // paused
            // resume
            player->StartAudio();
        }
    }
    isPlaying = true;
    stopped = false;
}

void AudioSource::Stop()
{
    if (isPlaying || (!isPlaying && !stopped))
    {
        player->StopAudio();
        playbackThread.interrupt();
        playbackThread.join();
    }
}

void AudioSource::Pause()
{
    if (!isPlaying) return;
    if (stopped) return;

    player->PauseAudio();

    isPlaying = false;
    stopped = false;
}

bool AudioSource::Playing() const
{
    return isPlaying && !stopped;
}

bool AudioSource::Stopped() const
{
    return !isPlaying && stopped;
}

bool AudioSource::Paused() const
{
    return !isPlaying && !stopped;
}

void AudioSource::PlaySync()
{
    if (!fileOpened) return;

    // Start the source voice
    player->StartAudio();

    LOG_INFO << "Starting audio file \"" << filename << "\" playback at " <<
             std::hex << std::showbase << boost::this_thread::get_id() << std::noshowbase << std::dec;

    bool stoppedByStreamEnd = true;

    try
    {
        while (true)
        {
            unsigned char* buffer;
            int bufferSize;
            // If the source voice is going to be starved, decode & send buffer with FFmpeg
            //int i = ffmpeg.SendBuffer(sourceVoice);
            int i = 0;
            int bufferCount = player->GetAddedBufferCount();
            while (bufferCount < MAX_BUFFER_COUNT)
            {
                i = ffmpeg.GetBuffer(&buffer, &bufferSize);
                bufferCount = player->AddBuffer(buffer, bufferSize, i != 0);
                if (i != 0) break;
            }
            boost::this_thread::interruption_point();

            // Waiting for the source voice buffer to end
            player->WaitForBufferEnd();

            // If FFmpeg hit the end of file
            if (i != 0)
            {
                if (Loop)
                {
                    // Seek to the start of the file
                    ffmpeg.Seek(0, false);
                }
                else
                {
                    // Break the loop
                    break;
                }
            }
        }

        LOG_TRACE << "Waiting for the stream to end";

        // Waiting the source voice stream to end
        if (!player->WaitForStreamEnd(10.0f))
        {
            // If timeout, force the stream to end
            LOG_WARNING << "Timeout. Forcing the stream to end.";
        }

    }
    catch (boost::thread_interrupted const&)
    {
        LOG_WARNING << "Audio playback thread interrupted!";

        stoppedByStreamEnd = false;
    }

    // Stop the source voice
    player->StopAudio();

    LOG_INFO << "Stopping audio playback";

    // Clear all queued buffers
    player->ClearBuffer();

    ffmpeg.Seek(0, !stoppedByStreamEnd);

    isPlaying = false;
    stopped = true;
}
