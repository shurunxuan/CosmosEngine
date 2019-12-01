//
// Created by 西村智 on 12/1/19.
//

#ifndef GAMEENGINE_OPENALBACKEND_H
#define GAMEENGINE_OPENALBACKEND_H

#include "../AudioBackend.h"
#include "../../Export.h"

class ENGINE_API OpenALBackend final
        : public AudioBackend
{
public:

    OpenALBackend();

    ~OpenALBackend() final;

    bool Init() final;

    void Calculate3DAudio(float deltaTime, float totalTime) final;

    void DeInit() final;
};

#endif //GAMEENGINE_OPENALBACKEND_H
