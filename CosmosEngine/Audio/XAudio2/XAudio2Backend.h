//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_XAUDIO2BACKEND_H
#define GAMEENGINE_XAUDIO2BACKEND_H

#include "../AudioBackend.h"
#include "../../Export.h"

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
};

#endif //GAMEENGINE_XAUDIO2BACKEND_H
