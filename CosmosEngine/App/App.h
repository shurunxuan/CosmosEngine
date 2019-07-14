//
// Created by 西村智 on 2019-05-15.
//

#ifndef COSMOSENGINE_APP_H
#define COSMOSENGINE_APP_H

#include "../Export.h"
#include "../Rendering/RenderingBackend.h"

int ENGINE_API CosmosEngine(int argc, char** argv);

class ENGINE_API CEApp
{
public:
    CEApp();

    ~CEApp();

    CEApp(const CEApp&) = delete;

    CEApp(CEApp&&) = delete;

    CEApp& operator=(const CEApp&) = delete;

    CEApp& operator=(CEApp&&) = delete;

    /**
     * @brief Actual initialization of the app
     *
     * @param screenWidth Screen width
     * @param screenHeight Screen height
     * @return true if initialization succeeded, or false
     */
    virtual bool StartUp(unsigned int screenWidth, unsigned int screenHeight);

    /**
     * @brief The game loop. Called in function DSEngine
     *
     * @todo Add/change return type or parameters as needed
     */
    void Loop();

    virtual void Shutdown();

private:

    RenderingBackend* renderingBackend;
};

#endif //COSMOSENGINE_APP_H
