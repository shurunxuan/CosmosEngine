//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_AUDIOBACKEND_H
#define GAMEENGINE_AUDIOBACKEND_H

#include "../Export.h"

class ENGINE_API AudioBackend
{
public:

    AudioBackend();

    virtual ~AudioBackend();

    bool StartUp();

    void Update(float deltaTime, float totalTime);

    void Shutdown();

    virtual bool Init() = 0;

    virtual void Calculate3DAudio(float deltaTime, float totalTime) = 0;

    virtual void DeInit() = 0;

    virtual bool IsFloat() = 0;

    virtual bool Force32Bit() = 0;
};

ENGINE_EXTERNAL_VAR AudioBackend* presentedAudioBackend;

#endif //GAMEENGINE_AUDIOBACKEND_H
