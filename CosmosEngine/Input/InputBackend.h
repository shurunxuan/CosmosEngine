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
    ErrorAxisCode                // Doesn't exist
};


/**
 * @brief The button codes that maps to physical buttons on a gamepad
 *
 */
enum JoystickButtonCode
{
    DPadUp = 0x0001,            // DPad Up
    DPadDown = 0x0002,            // DPad Down
    DPadLeft = 0x0004,        // DPad Left
    DPadRight = 0x0008,        // DPad Right
    Start = 0x0010,                // Start / Menu
    Back = 0x0020,                // Back / View
    LeftThumb = 0x0040,        // Left Thumb Button
    RightThumb = 0x0080,        // Right Thumb Button
    LeftShoulder = 0x0100,        // Left Shoulder Button / LB
    RightShoulder = 0x0200,    // Right Shoulder Button / RB
    Reserved0 = 0x0400,        // Reserved
    Reserved1 = 0x0800,        // Reserved
    ButtonA = 0x1000,                    // A Button
    ButtonB = 0x2000,                    // B Button
    ButtonX = 0x4000,                    // X Button
    ButtonY = 0x8000                    // Y Button
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
    ErrorAxis        // Doesn't exist
};

enum KeyCode
{
    Backspace,
    Delete,
    Tab,
    Return,
    Esc,
    Space,
    Keypad0,
    Keypad1,
    Keypad2,
    Keypad3,
    Keypad4,
    Keypad5,
    Keypad6,
    Keypad7,
    Keypad8,
    Keypad9,
    KeypadPeriod,
    KeypadDivide,
    KeypadMultiply,
    KeypadMinus,
    KeypadPlus,
    KeypadEnter,
    Up,
    Down,
    Right,
    Left,
    Insert,
    Home,
    End,
    PageUp,
    PageDown,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    Number0,
    Number1,
    Number2,
    Number3,
    Number4,
    Number5,
    Number6,
    Number7,
    Number8,
    Number9,
    Quote,
    Comma,
    Minus,
    Period,
    Slash,
    Semicolon,
    Equal,
    LeftBracket,
    Backslash,
    RightBracket,
    BackQuote,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    NumLock,
    CapsLock,
    ScrollLock,
    RightShift,
    LeftShift,
    RightControl,
    LeftControl,
    RightAlt,
    LeftAlt,
    LeftWindows,
    RightWindows,
    Menu,
    ErrorKey
};

class InputBackend;

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
    InputAxis(boost::container::string name, boost::container::string posButton,
              boost::container::string negButton, boost::container::string altPosButton,
              boost::container::string altNegButton, float gravity, float dead, float sensitivity, bool invert,
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
    virtual ~InputBackend();

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
    virtual void StartUp(void* userData) = 0;

    /**
     * @brief Asynchronized Update of the input backend for polls
     *
     * @param deltaTime The time between two asynchronized input backend updates
     */
    virtual void AsyncUpdate(float deltaTime) = 0;

    /**
     * @brief Synchronized Update of the input backend for callbacks
     *
     * @param deltaTime The time between two synchronized input backend updates
     */
    virtual void SyncUpdate(float deltaTime);

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
    virtual void GetMousePosition(float* x, float* y) = 0;

    virtual void DisableCursor() = 0;

    virtual void HideCursor() = 0;

    virtual void RestoreCursor() = 0;

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

    virtual bool GetMouseButton(int mouseButtonCode) = 0;

    virtual bool GetMouseButtonDown(int mouseButtonCode) = 0;

    virtual bool GetMouseButtonUp(int mouseButtonCode) = 0;

    virtual bool GetButton(JoystickButtonCode joystickButtonCode, int joystickNumber) = 0;

    virtual bool GetButtonDown(JoystickButtonCode joystickButtonCode, int joystickNumber) = 0;

    virtual bool GetButtonUp(JoystickButtonCode joystickButtonCode, int joystickNumber) = 0;

    virtual bool GetKey(KeyCode keyCode) = 0;

    virtual bool GetKeyDown(KeyCode keyCode) = 0;

    virtual bool GetKeyUp(KeyCode keyCode) = 0;

    virtual float GetRawAxis(JoystickAxisCode axisCode, int joystickNumber) = 0;

    virtual double GetMouseDeltaX() = 0;

    virtual double GetMouseDeltaY() = 0;

    virtual double GetMouseDeltaWheel() = 0;

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
