//
// Created by 西村智 on 12/1/19.
//

#include "XAudio2Backend.h"
#include "../../Logging/Logging.h"

XAudio2Backend::XAudio2Backend()
        : AudioBackend()
{
    xAudio2 = nullptr;
    masterVoice = nullptr;

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
}

XAudio2Backend::~XAudio2Backend()
{
    SAFE_RELEASE(xAudio2);
}

bool XAudio2Backend::Init()
{
    LOG_INFO << "Initializing XAudio2 Backend";
    HRESULT hr = CreateXAudio2Engine();
    if (!(SUCCEEDED(hr))) return false;
    hr = CreateMasteringVoice();
    if (!(SUCCEEDED(hr))) return false;
    hr = CreateX3DAudioInstance();
    if (!(SUCCEEDED(hr))) return false;
    return true;
}

void XAudio2Backend::Calculate3DAudio(float deltaTime, float totalTime)
{

}

void XAudio2Backend::DeInit()
{

}

bool XAudio2Backend::IsFloat()
{
    return false;
}

int XAudio2Backend::ForceBitsPerSample()
{
    return 0;
}

HRESULT XAudio2Backend::CreateXAudio2Engine()
{
    HRESULT hr = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    XAUDIO2_DEBUG_CONFIGURATION debugConfig = {};
    debugConfig.TraceMask |=
            XAUDIO2_LOG_DETAIL | XAUDIO2_LOG_WARNINGS | XAUDIO2_LOG_TIMING | XAUDIO2_LOG_MEMORY | XAUDIO2_LOG_STREAMING;
    xAudio2->SetDebugConfiguration(&debugConfig);
    LOG_INFO << "XAudio2 Engine Created";
    return hr;
}

HRESULT XAudio2Backend::CreateMasteringVoice()
{
    HRESULT hr = xAudio2->CreateMasteringVoice(&masterVoice);
    voiceDetails = {};
    masterVoice->GetVoiceDetails(&voiceDetails);
    LOG_INFO << "Mastering voice created with " << GetMasteringVoiceChannel() << " channel(s), sample rate "
             << GetMasteringVoiceSampleRate() << "Hz.";
    return hr;
}

HRESULT XAudio2Backend::CreateX3DAudioInstance()
{
    DWORD dwChannelMask;
    masterVoice->GetChannelMask(&dwChannelMask);

    return X3DAudioInitialize(dwChannelMask, X3DAUDIO_SPEED_OF_SOUND, X3DInstance);
}

unsigned int XAudio2Backend::GetMasteringVoiceSampleRate() const
{
    return voiceDetails.InputSampleRate;
}

unsigned int XAudio2Backend::GetMasteringVoiceChannel() const
{
    return voiceDetails.InputChannels;
}

HRESULT
XAudio2Backend::CreateSourceVoice(IXAudio2SourceVoice** ppSourceVoice, int channels, int sampleRate, int bytesPerSample,
                                  IXAudio2VoiceCallback* pCallback) const
{
    WAVEFORMATEXTENSIBLE wfx;
    memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));

    wfx.Format.wFormatTag = WAVE_FORMAT_PCM;
    wfx.Format.nSamplesPerSec = sampleRate;
    wfx.Format.nChannels = channels;
    wfx.Format.wBitsPerSample = bytesPerSample * 8;
    wfx.Format.nBlockAlign = wfx.Format.nChannels * bytesPerSample;
    wfx.Format.nAvgBytesPerSec = wfx.Format.nSamplesPerSec * wfx.Format.nBlockAlign;
    wfx.Format.cbSize = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
    wfx.Samples.wValidBitsPerSample = wfx.Format.wBitsPerSample;
    masterVoice->GetChannelMask(&wfx.dwChannelMask);
    wfx.SubFormat = KSDATAFORMAT_SUBTYPE_PCM;

    const HRESULT hr = xAudio2->CreateSourceVoice(ppSourceVoice, reinterpret_cast<WAVEFORMATEX*>(&wfx), 0, 2,
                                                  pCallback);
    if (FAILED(hr))
        LOG_ERROR << "Failed to create source voice";
    return hr;
}

//void XAudio2Backend::AudioCalculate3D(AudioListener* listener, AudioSource* emitter)
//{
//    X3DAudioCalculate(X3DInstance, &(listener->x3dListener), &(emitter->x3dEmitter),
//                      X3DAUDIO_CALCULATE_MATRIX |
//                      X3DAUDIO_CALCULATE_DOPPLER |
//                      X3DAUDIO_CALCULATE_LPF_DIRECT |
//                      X3DAUDIO_CALCULATE_REVERB,
//                      &(emitter->dspSettings));
//
//    // Transpose the matrix coefficients.
//    // TODO: NEED TO FIND WHY
//    boost::container::vector<FLOAT32> coefficientsTransposed;
//    coefficientsTransposed.resize(emitter->dspSettings.SrcChannelCount * emitter->dspSettings.DstChannelCount);
//    for (size_t d = 0; d < emitter->dspSettings.DstChannelCount; ++d)
//        for (size_t s = 0; s < emitter->dspSettings.SrcChannelCount; ++s)
//        {
//            coefficientsTransposed[d * emitter->dspSettings.SrcChannelCount + s] =
//                    emitter->dspSettings.pMatrixCoefficients[s * emitter->dspSettings.DstChannelCount + d];
//        }
//
//    // Apply the result
//    emitter->sourceVoice->SetOutputMatrix(masterVoice, emitter->dspSettings.SrcChannelCount, emitter->dspSettings.DstChannelCount, &*coefficientsTransposed.begin());
//    emitter->sourceVoice->SetFrequencyRatio(emitter->dspSettings.DopplerFactor * emitter->frequencyRatio);
//
//    XAUDIO2_FILTER_PARAMETERS FilterParameters = { LowPassFilter, 2.0f * sinf(X3DAUDIO_PI / 6.0f * emitter->dspSettings.LPFDirectCoefficient), 1.0f };
//    emitter->sourceVoice->SetFilterParameters(&FilterParameters);
//}
