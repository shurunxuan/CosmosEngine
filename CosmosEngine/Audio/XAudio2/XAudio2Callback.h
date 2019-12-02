//
// Created by shuru on 12/2/2019.
//

#ifndef GAMEENGINE_XAUDIO2CALLBACK_H
#define GAMEENGINE_XAUDIO2CALLBACK_H

#include "../../Export.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <xaudio2.h>
#include <x3daudio.h>

/**
 * @brief Callback for XAudio2 Source Voice
 *
 */
class ENGINE_LOCAL XAudio2Callback final : public IXAudio2VoiceCallback
{
public:
    /**
     * @brief Buffer end event
     *
     */
    HANDLE bufferEvent;
    /**
     * @brief Stream end event
     *
     */
    HANDLE streamEvent;

    /**
     * @brief Construct a new DSFVoiceCallback object
     *
     * Set the events
     *
     */
    XAudio2Callback();

    /**
     * @brief Destroy the DSFVoiceCallback object
     *
     * Close all events' handles
     *
     */
    ~XAudio2Callback();

    /**
     * @brief Called when the voice has just finished playing a contiguous audio stream
     *
     */
    void STDMETHODCALLTYPE OnStreamEnd() override;

    /**
     * @brief Called just after the processing pass for the voice ends
     *
     */
    void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() override;

    /**
     * @brief Called during each processing pass for each voice, just before XAudio2 reads data from the voice's buffer queue.
     *
     * @param samples The number of bytes that must be submitted immediately to avoid starvation.
     */
    void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 samples) override;

    /**
     * @brief Called when the voice finishes processing a buffer
     *
     * @param context Context pointer that was assigned to the pContext member of the XAUDIO2_BUFFER structure when the buffer was submitted
     */
    void STDMETHODCALLTYPE OnBufferEnd(void* context) override;

    /**
     * @brief Called when the voice is about to start processing a new audio buffer
     *
     * @param context Context pointer that was assigned to the pContext member of the XAUDIO2_BUFFER structure when the buffer was submitted
     */
    void STDMETHODCALLTYPE OnBufferStart(void* context) override;

    /**
     * @brief Called when the voice reaches the end position of a loop
     *
     * @param context Context pointer that was assigned to the pContext member of the XAUDIO2_BUFFER structure when the buffer was submitted
     */
    void STDMETHODCALLTYPE OnLoopEnd(void* context) override;

    /**
     * @brief Called when a critical error occurs during voice processing
     *
     * @param context Context pointer that was assigned to the pContext member of the XAUDIO2_BUFFER structure when the buffer was submitted
     * @param Error
     */
    void STDMETHODCALLTYPE OnVoiceError(void* context, HRESULT Error) override;
};

#endif //GAMEENGINE_XAUDIO2CALLBACK_H
