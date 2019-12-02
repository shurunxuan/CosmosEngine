//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_OPENALBACKEND_H
#define GAMEENGINE_OPENALBACKEND_H

#include "../AudioBackend.h"
#include "../../Export.h"

#include <AL/al.h>
#include <AL/alc.h>

class ENGINE_API OpenALBackend final
        : public AudioBackend
{
public:

    OpenALBackend();

    ~OpenALBackend() final;

    bool Init() final;

    void Calculate3DAudio(float deltaTime, float totalTime) final;

    void DeInit() final;

    bool IsFloat() final;

    int ForceBitsPerSample() final;

private:

    ALCdevice* device;
    ALCcontext* context;

};

#endif //GAMEENGINE_OPENALBACKEND_H
