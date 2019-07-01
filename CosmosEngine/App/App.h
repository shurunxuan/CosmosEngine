//
// Created by 西村智 on 2019-05-15.
//

#ifndef COSMOSENGINE_APP_H
#define COSMOSENGINE_APP_H

#include "../Export.h"

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

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
	virtual bool Initialize(int screenWidth, int screenHeight);

	/**
	 * @brief Initialize of the game logic
	 *
	 */
	virtual void Init() = 0;

	/**
	 * @brief The game loop. Called in function DSEngine
	 *
	 * @todo Add/change return type or parameters as needed
	 */
	void Loop();

	virtual void DeInitialize();

	virtual void DeInit() = 0;

private:

	void initVulkan();

	GLFWwindow* window;

	VkInstance vulkanInstance;
};

#endif //COSMOSENGINE_APP_H
