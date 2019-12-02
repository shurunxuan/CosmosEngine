//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_COREAUDIOBACKEND_H
#define GAMEENGINE_COREAUDIOBACKEND_H

#include "../AudioBackend.h"
#include "../../Export.h"
#include "CoreAudioBackendBridge.h"

OBJC_CLASS(AVAudioEngine);

class ENGINE_API CoreAudioBackend final
        : public AudioBackend
{
public:

    CoreAudioBackend();

    ~CoreAudioBackend() final;

    bool Init() final;

    void Calculate3DAudio(float deltaTime, float totalTime) final;

    void DeInit() final;

    bool IsFloat() final;

    bool Force32Bit() final;

    AVAudioEngine* GetEngine();
    AVAudioMixerNode* GetMainMixerNode();

private:
    AVAudioEngine* engine = nullptr;

    AVAudioMixerNode* mainMixerNode = nullptr;
};

#endif //GAMEENGINE_COREAUDIOBACKEND_H
