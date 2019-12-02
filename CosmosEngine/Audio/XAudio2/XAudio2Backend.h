//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_XAUDIO2BACKEND_H
#define GAMEENGINE_XAUDIO2BACKEND_H

#include "../AudioBackend.h"
#include "../../Export.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <xaudio2.h>
#include <x3daudio.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
   if(x != NULL)        \
   {                    \
      x->Release();     \
      x = NULL;         \
   }
#endif

class ENGINE_API XAudio2Backend final
        : public AudioBackend
{
public:

    XAudio2Backend();

    ~XAudio2Backend() final;

    bool Init() final;

    void Calculate3DAudio(float deltaTime, float totalTime) final;

    void DeInit() final;

    bool IsFloat() final;

    bool Force32Bit() final;

    /**
     * @brief Create the XAudio2 Engine
     *
     * @return HRESULT S_OK if creation succeed, or other
     */
    HRESULT CreateXAudio2Engine();

    /**
     * @brief Create the mastering voice
     *
     * @return HRESULT S_OK if creation succeed, or other
     */
    HRESULT CreateMasteringVoice();

    /**
     * @brief Create the X3DAudio Instance
     *
     * @return HRESULT S_OK if creation succeed, or other
     */
    HRESULT CreateX3DAudioInstance();

    /**
     * @brief Get the Mastering Voice Sample Rate
     *
     * @return unsigned int Mastering voice sample rate
     */
    unsigned int GetMasteringVoiceSampleRate() const;

    /**
     * @brief Get the Mastering Voice Channel
     *
     * @return unsigned int Mastering voice channel
     */
    unsigned int GetMasteringVoiceChannel() const;

    /**
     * @brief Create the source voice
     *
     * @param ppSourceVoice The pointer to an output source voice pointer
     * @param channels The target channels of the source voice
     * @param sampleRate The target sample rate of the source voice
     * @param bytesPerSample The target bytes per sample of the source voice
     * @param pCallback The pointer to the client defined callback class, default to nullptr
     *
     * @return HRESULT S_OK if creation succeed, or other
     */
    HRESULT CreateSourceVoice(IXAudio2SourceVoice** ppSourceVoice, int channels, int sampleRate, int bytesPerSample,
                              IXAudio2VoiceCallback* pCallback = nullptr) const;

    /**
     * @brief Calculate the 3D Audio Matrix
     *
     * @param listener The audio listener
     * @param emitter The audio source
     */
    //void AudioCalculate3D(AudioListener* listener, AudioSource* emitter);

private:
    /**
     * @brief XAudio2 Engine
     *
     */
    IXAudio2* xAudio2;
    /**
     * @brief Mastering Voice
     *
     */
    IXAudio2MasteringVoice* masterVoice;
    /**
     * @brief X3DAudio Handle
     *
     */
    X3DAUDIO_HANDLE X3DInstance;
    /**
     * @brief The Voice Details
     */
    XAUDIO2_VOICE_DETAILS voiceDetails;
};

#endif //GAMEENGINE_XAUDIO2BACKEND_H
