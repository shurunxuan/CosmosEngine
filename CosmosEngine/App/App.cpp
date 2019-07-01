//
// Created by 西村智 on 2019-05-15.
//

#include "App.h"



#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include "../Compiler/Compiler.h"
#include <vector>

CEApp* App = nullptr;

int CosmosEngine(int argc, char** argv)
{
	App->Initialize(1366, 768);
	App->Loop();
	App->DeInitialize();

	return 0;
}

CEApp::CEApp()
{
	App = this;
	window = nullptr;
}

CEApp::~CEApp()
{
}

bool CEApp::Initialize(int screenWidth, int screenHeight)
{
	const auto enumerate_instance_version =
		reinterpret_cast<PFN_vkEnumerateInstanceVersion>(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));

	uint32_t instance_version = VK_API_VERSION_1_0;

	if (enumerate_instance_version != nullptr) {
		enumerate_instance_version(&instance_version);
	}

	const uint32_t vulkan_major = VK_VERSION_MAJOR(instance_version);
	const uint32_t vulkan_minor = VK_VERSION_MINOR(instance_version);
	const uint32_t vulkan_patch = VK_VERSION_PATCH(VK_HEADER_VERSION);

	std::cout << "Vulkan Instance Version: "
		<< vulkan_major << "."
		<< vulkan_minor << "."
		<< vulkan_patch << std::endl;

	std::cout << CompilerString() << std::endl;

	glm::mat4 matrix;
	glm::vec4 vec;
	auto test = matrix * vec;

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(screenWidth, screenHeight, "Cosmos Engine", nullptr, nullptr);

	initVulkan();

	Init();

	return true;
}

void CEApp::Loop()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void CEApp::DeInitialize()
{
	DeInit();

	vkDestroyInstance(vulkanInstance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}

void CEApp::initVulkan()
{
	// Create vulkan instance
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;

	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;

	if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}

	// Enumerate extensions
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::cout << extensionCount << " extensions supported" << std::endl;

	std::vector<VkExtensionProperties> extensions(extensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions:" << std::endl;

	for (const auto& extension : extensions) {
		std::cout << "  ";
		bool required = false;
		for (unsigned int i = 0; i < glfwExtensionCount; ++i)
		{
			if (strcmp(extension.extensionName, glfwExtensions[i]) == 0)
			{
				required = true;
				break;
			}
		}
		if (required)
			std::cout << "* ";
		else
			std::cout << "  ";
		std::cout << extension.extensionName << std::endl;
	}
}
