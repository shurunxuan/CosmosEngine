//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_XAUDIO2PLAYER_H
#define GAMEENGINE_XAUDIO2PLAYER_H

#include "../AudioPlayer.h"
#include "XAudio2Callback.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <xaudio2.h>
#include <x3daudio.h>

class ENGINE_LOCAL XAudio2Player final
        : public AudioPlayer
{
public:

    XAudio2Player();

    ~XAudio2Player() final;

    void StartAudio() final;

    void PauseAudio() final;

    void StopAudio() final;

    void ClearBuffer() final;

    int AddBuffer(unsigned char* buffer, int bufferSize, bool finalBuffer) final;

    int GetAddedBufferCount() final;

    void Init(int sampleRate, int channels, int bytesPerSample) final;

    void DeInit() final;

    void WaitForBufferEnd() final;

    bool WaitForStreamEnd(float timeout) final;

private:
    /**
	 * @brief The XAudio2 Source Voice
	 *
	 */
    IXAudio2SourceVoice* sourceVoice;

    /**
     * @brief The Callback that will be used by the source voice
     *
     */
    XAudio2Callback callback;

    /**
     * @brief The X3DAudio Emitter struct
     *
     */
    X3DAUDIO_EMITTER x3dEmitter;

    /**
     * @brief The X3DAudio DSP Settings
     *
     */
    X3DAUDIO_DSP_SETTINGS dspSettings;
};

#endif //GAMEENGINE_XAUDIO2PLAYER_H
