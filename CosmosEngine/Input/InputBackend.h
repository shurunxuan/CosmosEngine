//
// Created by vs1571 on 10/3/2019.
//

#ifndef GAMEENGINE_INPUTBACKEND_H
#define GAMEENGINE_INPUTBACKEND_H

#include "../Export.h"
#include <boost/container/string.hpp>
#include <boost/function.hpp>
#include <boost/container/list.hpp>

/**
 * @brief The types of inputs
 *
 */

enum InputType
{
    Button,                    // Keyboard, mouse or joystick button
    Movement,                // Mouse movement
    Axis                    // Joystick axes
};

/**
 * @brief The axis code if the input type is not Button
 *
 */
enum AxisCode
{
    MouseX,                    // Mouse X
    MouseY,                    // Mouse Y
    MouseWheel,                // Mouse Wheel
    JoystickLT,                // Left Trigger
    JoystickRT,                // Right Trigger
    JoystickLX,                // Left Thumb Horizontal
    JoystickLY,                // Left Thumb Vertical
    JoystickRX,                // Right Thumb Horizontal
    JoystickRY,                // Right Thumb Vertical
    ErrorAxis                // Doesn't exist
};


/**
 * @brief The button codes that maps to physical buttons on a gamepad
 *
 */
enum JoystickButtonCode
{
    DPAD_UP = 0x0001,            // DPad Up
    DPAD_DOWN = 0x0002,            // DPad Down
    DPAD_LEFT = 0x0004,        // DPad Left
    DPAD_RIGHT = 0x0008,        // DPad Right
    START = 0x0010,                // Start / Menu
    BACK = 0x0020,                // Back / View
    LEFT_THUMB = 0x0040,        // Left Thumb Button
    RIGHT_THUMB = 0x0080,        // Right Thumb Button
    LEFT_SHOULDER = 0x0100,        // Left Shoulder Button / LB
    RIGHT_SHOULDER = 0x0200,    // Right Shoulder Button / RB
    RESERVED_0 = 0x0400,        // Reserved
    RESERVED_1 = 0x0800,        // Reserved
    A = 0x1000,                    // A Button
    B = 0x2000,                    // B Button
    X = 0x4000,                    // X Button
    Y = 0x8000                    // Y Button
};

/**
 * @brief The codes of analog axis that maps to a gamepad
 *
 */
enum JoystickAxisCode
{
    LT,                // Left Trigger
    RT,                // Right Trigger
    LX,                // Left Thumb Horizontal
    LY,                // Left Thumb Vertical
    RX,                // Right Thumb Horizontal
    RY,                // Right Thumb Vertical
    ERROR_AXIS        // Doesn't exist
};

/**
 * @brief Class that represents a input
 *
 */
class ENGINE_API InputAxis
{
public:
    friend class InputBackend;

private:
    /**
     * @brief Name of the input
     *
     */
    boost::container::string name;
    /**
     * @brief Positive button of the input
     *
     */
    boost::container::string posButton;
    /**
     * @brief Negative button of the input
     *
     */
    boost::container::string negButton;
    /**
     * @brief Alternative positive button
     *
     */
    boost::container::string altPosButton;
    /**
     * @brief Alternative negative button
     *
     */
    boost::container::string altNegButton;
    /**
     * @brief How fast the input re-centers. This property applies only when the type is Button
     *
     */
    float gravity;
    /**
     * @brief Deadzone
     *
     */
    float dead;
    /**
     * @brief If the input type is Button, this represents the respond speed; if the input type is Movement, this will be multiplied to the raw movement data
     *
     */
    float sensitivity;
    /**
     * @brief Invert the input or not
     *
     */
    bool invert;
    /**
     * @brief The input type
     *
     */
    InputType type;
    /**
     * @brief The axis to be used if the input type is not Button
     *
     */
    AxisCode axis;
    /**
     * @brief The joystick to be used if the input requires the joystick
     *
     */
    int joyNum;

    /**
     * @brief The input backend pointer
     *
     */
    InputBackend* inputBackend;

    /**
     * @brief The smoothed axis value
     *
     */
    float processedAxisValue;

    // 1: pos, 2: altPos, 3: neg, 4: altNeg
    /**
     * @brief The get button functions. 1: positive, 2: altPositive, 3: negative, 4: altNegative
     *
     */
    boost::function<bool()> getButton[4];
    /**
     * @brief The get button down functions. 1: positive, 2: altPositive, 3: negative, 4: altNegative
     *
     */
    boost::function<bool()> getButtonDown[4];
    /**
     * @brief The get button up functions. 1: positive, 2: altPositive, 3: negative, 4: altNegative
     *
     */
    boost::function<bool()> getButtonUp[4];

    /**
     * @brief The get raw axis data function
     *
     */
    boost::function<float()> getRawAxis;

    /**
     * @brief Bind the functions for buttons
     *
     * @param buttonName The name of the button
     * @param buttonFuncIndex The function index to be bound. 1: positive, 2: altPositive, 3: negative, 4: altNegative
     */
    void BindButtonCallback(const boost::container::string& buttonName, int buttonFuncIndex);

    /**
     * @brief Bind the functions for buttons and axes
     *
     */
    void BindCallbackFunctions();

    /**
     * @brief Update the smoothed value of the axis
     *
     * @param deltaTime The time between two asynchronized input system updates
     */
    void Update(float deltaTime);

public:
    /**
     * @brief Construct a new DSSInputAxis object
     *
     * @param name The name of the input
     * @param posButton The positive button
     * @param negButton The negative button
     * @param altPosButton The alternative positive button
     * @param altNegButton The alternative negative button
     * @param gravity The speed the input re-centers. This property applies only when the type is Button
     * @param dead Deadzone of an axis
     * @param sensitivity The speed of a Button type of input responds, or the scale factor of a Movement type of input
     * @param invert Invert the axis/button
     * @param type Button: any kind of button (keyboard, joystick, mouse); Movement: mouse movement; Axis: axis on a joystick
     * @param axis The hardware axis if the type is not Button
     * @param joyNum The number of the joystick
     * @param system The input system pointer
     */
    InputAxis(const boost::container::string& name, const boost::container::string& posButton,
              const boost::container::string& negButton, const boost::container::string& altPosButton,
              const boost::container::string& altNegButton, float gravity, float dead, float sensitivity, bool invert,
              InputType type, AxisCode axis, int joyNum, InputBackend* backend);
};

class ENGINE_API InputBackend
{
public:

    friend class InputAxis;

    /**
     * @brief Construct a new InputBackend object
     *
     * Should set all pointers to nullptr
     */
    InputBackend();

    /**
     * @brief Destroy the InputBackend object
     *
     * Should delete or release or free all pointers
     */
    ~InputBackend();

    /**
     * @brief Copy constructor of InputBackend is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    InputBackend(const InputBackend& v) = delete;

    /**
     * @brief Move constructor of InputBackend is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    InputBackend(InputBackend&& v) = delete;

    /**
     * @brief Copy assignment operator of InputBackend is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    InputBackend& operator=(const InputBackend& v) = delete;

    /**
     * @brief Move assignment operator of InputBackend is deleted
     * since the class is meant to be a singleton
     *
     * @param v Another instance
     */
    InputBackend& operator=(InputBackend&& v) = delete;

    /**
     * @brief Actual initialization of the input backend
     *
     * @param userData Any useful data for initialize the input backend
     */
    virtual void Init(void* userData) = 0;

    /**
     * @brief Asynchronized Update of the input system for polls
     *
     * @param deltaTime The time between two asynchronized input backend updates
     */
    virtual void AsyncUpdate(float deltaTime) = 0;

    /**
     * @brief Synchronized Update of the input system for callbacks
     *
     * @param deltaTime The time between two synchronized input system updates
     */
    virtual void SyncUpdate(float deltaTime) = 0;

    /**
     * @brief Get if the input button is being held
     *
     * @param name The name of the input
     * @return true if the input button is being held
     */
    bool GetButton(const boost::container::string& name);

    /**
     * @brief Get if the input button is pressed
     *
     * @param name The name of the input
     * @return true if the input button is pressed
     */
    bool GetButtonDown(const boost::container::string& name);

    /**
     * @brief Get if the input button is released
     *
     * @param name The name of the input
     * @return true if the input button is released
     */
    bool GetButtonUp(const boost::container::string& name);

    /**
     * @brief Get the input axis data
     *
     * @param name The name of the input
     * @return float The corresponding axis data
     */
    float GetAxis(const boost::container::string& name);

    /**
     * @brief Get the input raw axis data
     *
     * @param name The name of the input
     * @return float The corresponding raw axis data
     */
    float GetRawAxis(const boost::container::string& name);

    /**
     * @brief Get the Mouse Position
     *
     * @param[out] x The x position of the mouse pointer
     * @param[out] y The y position of the mouse pointer
     */
    virtual void GetMousePosition(float* x, float* y) const = 0;

    /**
     * @brief Register a new input into the input system
     *
     * @param name The name of the input
     * @param posButton The positive button
     * @param negButton The negative button
     * @param altPosButton The alternative positive button
     * @param altNegButton The alternative negative button
     * @param gravity The speed the input re-centers. This property applies only when the type is Button
     * @param dead Deadzone of an axis
     * @param sensitivity The speed of a Button type of input responds, or the scale factor of a Movement type of input
     * @param invert Invert the axis/button
     * @param type Button: any kind of button (keyboard, joystick, mouse); Movement: mouse movement; Axis: axis on a joystick
     * @param axis The hardware axis if the type is not Button
     * @param joyNum The number of the joystick
     */
    void RegisterInput(const boost::container::string& name, const boost::container::string& posButton,
                  const boost::container::string& negButton, const boost::container::string& altPosButton,
                  const boost::container::string& altNegButton, float gravity, float dead, float sensitivity,
                  bool invert, InputType type, AxisCode axis, int joyNum);

private:
//    /**
//     * @brief The XInput Framework reference
//     *
//     */
//    DSFXInput xInput;
//    /**
//     * @brief The Raw Input Framework reference
//     *
//     */
//    DSFRawInput rawInput;

    /**
     * @brief All registered inputs
     *
     */
    boost::container::list<InputAxis> inputs;
};

ENGINE_EXTERNAL_VAR InputBackend* presentedInputBackend;

#endif //GAMEENGINE_INPUTBACKEND_H
