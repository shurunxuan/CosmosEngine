//
// Created by 西村智 on 10/4/19.
//

#ifndef GAMEENGINE_GLFWINPUTBACKEND_H
#define GAMEENGINE_GLFWINPUTBACKEND_H

#include "../../Export.h"
#include "../InputBackend.h"

#include <boost/unordered_map.hpp>
#include <boost/container/vector.hpp>
#include <GLFW/glfw3.h>

class ENGINE_LOCAL GLFWInputBackend : public InputBackend
{
public:

    GLFWInputBackend();

    ~GLFWInputBackend() final;

    /**
     * @brief Actual initialization of the input backend
     *
     * @param userData Any useful data for initialize the input backend
     */
    void StartUp(void* userData) final;

    /**
     * @brief Asynchronized Update of the input backend for polls
     *
     * @param deltaTime The time between two asynchronized input backend updates
     */
    void AsyncUpdate(float deltaTime) final;

    /**
     * @brief Synchronized Update of the input backend for callbacks
     *
     * @param deltaTime The time between two synchronized input backend updates
     */
    void SyncUpdate(float deltaTime) final;

    bool GetMouseButton(int mouseButtonCode) final;

    bool GetMouseButtonDown(int mouseButtonCode) final;

    bool GetMouseButtonUp(int mouseButtonCode) final;

    bool GetButton(JoystickButtonCode joystickButtonCode, int joystickNumber) final;

    bool GetButtonDown(JoystickButtonCode joystickButtonCode, int joystickNumber) final;

    bool GetButtonUp(JoystickButtonCode joystickButtonCode, int joystickNumber) final;

    bool GetKey(KeyCode keyCode) final;

    bool GetKeyDown(KeyCode keyCode) final;

    bool GetKeyUp(KeyCode keyCode) final;

    float GetRawAxis(JoystickAxisCode axisCode, int joystickNumber) final;

    double GetMouseDeltaX() final;

    double GetMouseDeltaY() final;

    double GetMouseDeltaWheel() final;

    void CursorPositionCallback(double xPos, double yPos);

    void CursorWheelCallback(double offset);

    void GetMousePosition(float* x, float* y) final;

    void DisableCursor() final;

    void HideCursor() final;

    void RestoreCursor() final;

private:

    GLFWwindow* window;

    double mouseX;
    double mouseY;
    double lastMouseX;
    double lastMouseY;

    double mouseDeltaX;
    double mouseDeltaY;

    double mouseDeltaWheel;
    bool mouseWheelUpdated;

    boost::unordered_map<KeyCode, bool> keyPressStates;
    boost::unordered_map<KeyCode, bool> keyHoldStates;
    boost::unordered_map<KeyCode, bool> keyReleaseStates;

    boost::container::vector<bool> mouseButtonPressStates;
    boost::container::vector<bool> mouseButtonHoldStates;
    boost::container::vector<bool> mouseButtonReleaseStates;


};

extern ENGINE_LOCAL GLFWInputBackend* glfwInputBackend;

#endif //GAMEENGINE_GLFWINPUTBACKEND_H
