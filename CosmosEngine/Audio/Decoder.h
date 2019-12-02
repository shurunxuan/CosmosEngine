//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_DECODER_H
#define GAMEENGINE_DECODER_H

#include "../Export.h"

#define MAX_BUFFER_COUNT 16

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
}

/**
 * @brief The FFMpeg Framework of the DS Engine
 */
class ENGINE_LOCAL Decoder
{
public:
    /**
     * @brief Construct a new DSFFFmpeg object
     *
     * Should initialize all pointers as nullptr
     */
    Decoder();

    /**
     * @brief Destroy the DSFFFmpeg object
     *
     * Should delete or release or free all pointers
     */
    ~Decoder();

    /**
    * @brief Copy constructor of DSFFFmpeg is deleted
    * since the class is not meant to be copied/moved
    *
    * @param v Another instance
    */
    Decoder(const Decoder& v) = delete;

    /**
     * @brief Move constructor of DSFFFmpeg is deleted
     * since the class is not meant to be copied/moved
     *
     * @param v Another instance
     */
    Decoder(Decoder&& v) = delete;

    /**
     * @brief Copy assignment operator of DSFFFmpeg is deleted
     * since the class is not meant to be copied/moved
     *
     * @param v Another instance
     */
    Decoder& operator=(const Decoder& v) = delete;

    /**
     * @brief Move assignment operator of DSFFFmpeg is deleted
     * since the class is not meant to be copied/moved
     *
     * @param v Another instance
     */
    Decoder& operator=(Decoder&& v) = delete;

    /**
     * @brief Actual initialization of FFmpeg Framework
     *
     */
    void Init();

    /**
     * @brief Open an audio file and fill the codec & format context
     *
     * @param filename The name of the audio file to open
     * @return int 0 if succeeded, or other
     */
    int OpenFile(const char* filename);

    /**
     * @brief Read & decode a frame from the file
     *
     * @return int 0 if succeeded, AVERROR_EOF if end of file, or other
     */
    int ReadFrame();

    /**
     * @brief Initialize the resampler and output the parameters
     *
     * @param[out] channels Channel count
     * @param[out] sampleRate Sample rate
     * @param[out] bytesPerSample Bytes per sample
     */
    void InitSoftwareResampler(int* channels, int* sampleRate, int* bytesPerSample);

    /**
     * @brief Resample the frame
     *
     * @return int 0 if succeeded, or other
     */
    int ResampleFrame();

    int GetBuffer(unsigned char** buf, int* size);

    /**
     * @brief Send the decoded buffer to the XAudio2 source voice
     *
     * @param sourceVoice The source voice used for playing the corresponding file
     * @return int 0 if succeeded, AVERROR_EOF if end of file, or other
     */
    //int SendBuffer(IXAudio2SourceVoice* sourceVoice);

    /**
     * @brief Seek to timestamp
     *
     * @param timestamp Target timestamp
     * @param shouldFreeFrames Indicates if freeing frames is required
     *
     * @return int 0 if succeeded, or other
     */
    int Seek(int64_t timestamp, bool shouldFreeFrames);

private:
    /**
     * @brief The format context
     *
     */
    AVFormatContext* formatContext;
    /**
     * @brief The audio stream
     *
     */
    AVStream* audioStream;
    /**
     * @brief The codec
     *
     */
    AVCodec* codec;
    /**
     * @brief The codec context
     *
     */
    AVCodecContext* codecContext;

    AVSampleFormat outputFormat;
    /**
     * @brief Current frame
     *
     */
    AVFrame* frame;
    /**
     * @brief Last frame
     *
     * We need this because FFmpeg will return the EOF signal only **after** the final frame is read
     * so we are always processing the last frame
     *
     */
    AVFrame* lastFrame;
    /**
     * @brief Audio packet
     *
     */
    AVPacket packet;
    /**
     * @brief Resampler context
     *
     */
    SwrContext* swr;
    /**
     * @brief Resampler buffer
     *
     */
    unsigned char* swrBuffer;
    /**
     * @brief Resampler buffer length
     *
     */
    int swrBufferLength;
    /**
     * @brief Audio buffers
     *
     */
    unsigned char** buffer;
    /**
     * @brief Index of the buffer currently in use
     *
     */
    int bufferIndex;
    /**
     * @brief End of file signal
     *
     */
    bool eof;
};

#endif //GAMEENGINE_DECODER_H
