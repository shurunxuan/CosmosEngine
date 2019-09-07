#ifndef TESTGAMEAPP_H
#define TESTGAMEAPP_H

#include "CosmosEngine/App/App.h"
#include "CosmosEngine/Core/Object.h"
#include "CosmosEngine/Rendering/RenderingPipeline.h"

class TestGameApp :
        public CEApp
{
public:
    TestGameApp();

    ~TestGameApp();

    bool StartUp(unsigned int screenWidth, unsigned int screenHeight) override;

    void Shutdown() override;

    void Update(float deltaTime, float totalTime) override;

    RenderingPipeline* pipeline;
};


#endif // TESTGAMEAPP_H
