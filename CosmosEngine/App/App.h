//
// Created by 西村智 on 2019-05-15.
//

#ifndef COSMOSENGINE_APP_H
#define COSMOSENGINE_APP_H

#include "../Export.h"
#include "../Rendering/RenderingBackend.h"
#include "../Input/InputBackend.h"
#include "../Threading/JobSystem.h"
#include "../Core/Object.h"
#include "../Core/Camera.h"
#include "../Core/Scene.h"
#include "../Physics/PhysicsSystem.h"
#include <boost/chrono.hpp>

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

    /**
     * @brief Get the current active scene
     *
     * @todo This only returns App::currentScene currently. If we are going to have multiple scenes, there should be a scene manager.
     */
    Scene* CurrentActiveScene();

    void Exit();

private:

    RenderingBackend* renderingBackend;
    InputBackend* inputBackend;
    PhysicsSystem* physicsSystem;
    JobSystem* jobSystem;

    boost::chrono::high_resolution_clock::time_point startTime;
    boost::chrono::high_resolution_clock::time_point currentTime;
    boost::chrono::high_resolution_clock::time_point lastTime;
    boost::chrono::duration<float, boost::chrono::seconds::period> deltaTime;
    boost::chrono::duration<float, boost::chrono::seconds::period> totalTime;

    /**
	 * @brief Current active scene
	 */
    Scene* currentScene;
};

ENGINE_EXTERNAL_VAR CEApp* App;

#endif //COSMOSENGINE_APP_H
