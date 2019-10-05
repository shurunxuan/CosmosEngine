//
// Created by vs1571 on 10/3/2019.
//

#include "InputBackend.h"
#include "../Logging/Logging.h"

#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <utility>
#include <boost/container/vector.hpp>

InputBackend* presentedInputBackend = nullptr;

JoystickButtonCode GetJoystickButtonCodeFromName(const boost::container::string& buttonName)
{
    if (buttonName == "up")
        return DPadUp;
    if (buttonName == "down")
        return DPadDown;
    if (buttonName == "left")
        return DPadLeft;
    if (buttonName == "right")
        return DPadRight;
    if (buttonName == "start")
        return Start;
    if (buttonName == "back")
        return Back;
    if (buttonName == "ls")
        return LeftThumb;
    if (buttonName == "rs")
        return RightThumb;
    if (buttonName == "lb")
        return LeftShoulder;
    if (buttonName == "rb")
        return RightShoulder;
    if (buttonName == "a")
        return ButtonA;
    if (buttonName == "b")
        return ButtonB;
    if (buttonName == "x")
        return ButtonX;
    if (buttonName == "y")
        return ButtonY;
    return Reserved0;
}

KeyCode GetKeyCodeFromName(const boost::container::string& keyName)
{
    if (keyName == "backspace") return Backspace;
    if (keyName == "delete") return Delete;
    if (keyName == "tab") return Tab;
    if (keyName == "return") return Return;
    if (keyName == "escape") return Escape;
    if (keyName == "space") return Space;
    if (keyName == "keypad 0") return Keypad0;
    if (keyName == "keypad 1") return Keypad1;
    if (keyName == "keypad 2") return Keypad2;
    if (keyName == "keypad 3") return Keypad3;
    if (keyName == "keypad 4") return Keypad4;
    if (keyName == "keypad 5") return Keypad5;
    if (keyName == "keypad 6") return Keypad6;
    if (keyName == "keypad 7") return Keypad7;
    if (keyName == "keypad 8") return Keypad8;
    if (keyName == "keypad 9") return Keypad9;
    if (keyName == "keypad period") return KeypadPeriod;
    if (keyName == "keypad divide") return KeypadDivide;
    if (keyName == "keypad multiply") return KeypadMultiply;
    if (keyName == "keypad minus") return KeypadMinus;
    if (keyName == "keypad plus") return KeypadPlus;
    if (keyName == "keypad enter") return KeypadEnter;
    if (keyName == "up") return Up;
    if (keyName == "down") return Down;
    if (keyName == "right") return Right;
    if (keyName == "left") return Left;
    if (keyName == "insert") return Insert;
    if (keyName == "home") return Home;
    if (keyName == "end") return End;
    if (keyName == "page up") return PageUp;
    if (keyName == "page down") return PageDown;
    if (keyName == "f1") return F1;
    if (keyName == "f2") return F2;
    if (keyName == "f3") return F3;
    if (keyName == "f4") return F4;
    if (keyName == "f5") return F5;
    if (keyName == "f6") return F6;
    if (keyName == "f7") return F7;
    if (keyName == "f8") return F8;
    if (keyName == "f9") return F9;
    if (keyName == "f10") return F10;
    if (keyName == "f11") return F11;
    if (keyName == "f12") return F12;
    if (keyName == "f13") return F13;
    if (keyName == "f14") return F14;
    if (keyName == "f15") return F15;
    if (keyName == "0") return Number0;
    if (keyName == "1") return Number1;
    if (keyName == "2") return Number2;
    if (keyName == "3") return Number3;
    if (keyName == "4") return Number4;
    if (keyName == "5") return Number5;
    if (keyName == "6") return Number6;
    if (keyName == "7") return Number7;
    if (keyName == "8") return Number8;
    if (keyName == "9") return Number9;
    if (keyName == "quote") return Quote;
    if (keyName == "comma") return Comma;
    if (keyName == "minus") return Minus;
    if (keyName == "period") return Period;
    if (keyName == "slash") return Slash;
    if (keyName == "semicolon") return Semicolon;
    if (keyName == "equals") return Equal;
    if (keyName == "left bracket") return LeftBracket;
    if (keyName == "backslash") return Backslash;
    if (keyName == "right bracket") return RightBracket;
    if (keyName == "back quote") return BackQuote;
    if (keyName == "a") return A;
    if (keyName == "b") return B;
    if (keyName == "c") return C;
    if (keyName == "d") return D;
    if (keyName == "e") return E;
    if (keyName == "f") return F;
    if (keyName == "g") return G;
    if (keyName == "h") return H;
    if (keyName == "i") return I;
    if (keyName == "j") return J;
    if (keyName == "k") return K;
    if (keyName == "l") return L;
    if (keyName == "m") return M;
    if (keyName == "n") return N;
    if (keyName == "o") return O;
    if (keyName == "p") return P;
    if (keyName == "q") return Q;
    if (keyName == "r") return R;
    if (keyName == "s") return S;
    if (keyName == "t") return T;
    if (keyName == "u") return U;
    if (keyName == "v") return V;
    if (keyName == "w") return W;
    if (keyName == "x") return X;
    if (keyName == "y") return Y;
    if (keyName == "z") return Z;
    if (keyName == "num lock") return NumLock;
    if (keyName == "caps lock") return CapsLock;
    if (keyName == "scroll lock") return ScrollLock;
    if (keyName == "right shift") return RightShift;
    if (keyName == "left shift") return LeftShift;
    if (keyName == "right control") return RightControl;
    if (keyName == "left control") return LeftControl;
    if (keyName == "right alt") return RightAlt;
    if (keyName == "left alt") return LeftAlt;
    if (keyName == "left windows") return LeftWindows;
    if (keyName == "right windows") return RightWindows;
    if (keyName == "menu") return Menu;

    return ErrorKey;
}

int GetMouseButtonFromName(const boost::container::string& mouseButtonName)
{
    if (mouseButtonName == "0") return 0;
    if (mouseButtonName == "1") return 1;
    if (mouseButtonName == "2") return 2;
    if (mouseButtonName == "3") return 3;
    if (mouseButtonName == "4") return 4;
    if (mouseButtonName == "5") return 5;
    return -1;
}

JoystickAxisCode GetJoystickAxisCodeFromAxisCode(AxisCode axisCode)
{
    switch (axisCode)
    {
        case JoystickLT:
            return LT;
        case JoystickRT:
            return RT;
        case JoystickLX:
            return LX;
        case JoystickLY:
            return LY;
        case JoystickRX:
            return RX;
        case JoystickRY:
            return RY;
        default:
            return ErrorAxis;
    }
}

InputAxis::InputAxis(boost::container::string name, boost::container::string posButton,
                     boost::container::string negButton, boost::container::string altPosButton,
                     boost::container::string altNegButton, float gravity, float dead, float sensitivity, bool invert,
                     InputType type, AxisCode axis, int joyNum, InputBackend* backend)
        : name(std::move(name)), posButton(std::move(posButton)), negButton(std::move(negButton)),
          altPosButton(std::move(altPosButton)), altNegButton(std::move(altNegButton)), gravity(gravity), dead(dead),
          sensitivity(sensitivity), invert(invert), type(type), axis(axis), joyNum(joyNum), inputBackend(backend)
{
    processedAxisValue = 0;

    for (int i = 0; i < 4; ++i)
    {
        getButton[i] = []()
        { return false; };
        getButtonDown[i] = []()
        { return false; };
        getButtonUp[i] = []()
        { return false; };
    }

    BindCallbackFunctions();
}

void InputAxis::BindButtonCallback(const boost::container::string& buttonName, int buttonFuncIndex)
{
    // Determine if the buttonName is from keyboard or joystick or mouse
    if (buttonName.empty())
    {
        // Not assigned, does nothing
    }
    else if (boost::starts_with(buttonName, "mouse"))
    {
        // mouse
        boost::container::vector<boost::container::string> strs;
        split(strs, buttonName, boost::is_any_of(" "));

        int buttonCode = GetMouseButtonFromName(strs[1]);

        if (buttonCode < 0)
        {
            getButton[buttonFuncIndex] = [buttonName]()
            {
                LOG_ERROR << "Mouse Button \"" << buttonName << "\" Can Not Be Parsed!";
                return false;
            };
            getButtonDown[buttonFuncIndex] = [buttonName]()
            {
                LOG_ERROR << "Mouse Button \"" << buttonName << "\" Can Not Be Parsed!";
                return false;
            };
            getButtonUp[buttonFuncIndex] = [buttonName]()
            {
                LOG_ERROR << "Mouse Button \"" << buttonName << "\" Can Not Be Parsed!";
                return false;
            };
        }
        else
        {
            getButton[buttonFuncIndex] = boost::bind(&InputBackend::GetMouseButton, inputBackend, buttonCode);
            getButtonDown[buttonFuncIndex] = boost::bind(&InputBackend::GetMouseButtonDown, inputBackend, buttonCode);
            getButtonUp[buttonFuncIndex] = boost::bind(&InputBackend::GetMouseButtonUp, inputBackend, buttonCode);
        }
    }
    else if (boost::starts_with(buttonName, "joystick"))
    {
        // joystick

        // button name
        boost::container::vector<boost::container::string> strs;
        split(strs, buttonName, boost::is_any_of(" "));

        JoystickButtonCode buttonCode = GetJoystickButtonCodeFromName(strs[1]);

        if (buttonCode == Reserved0)
        {
            getButton[buttonFuncIndex] = [buttonName]()
            {
                LOG_ERROR << "Joystick Button \"" << buttonName << "\" Can Not Be Parsed!";
                return false;
            };
            getButtonDown[buttonFuncIndex] = [buttonName]()
            {
                LOG_ERROR << "Joystick Button \"" << buttonName << "\" Can Not Be Parsed!";
                return false;
            };
            getButtonUp[buttonFuncIndex] = [buttonName]()
            {
                LOG_ERROR << "Joystick Button \"" << buttonName << "\" Can Not Be Parsed!";
                return false;
            };
        }
        else
        {
            getButton[buttonFuncIndex] = boost::bind(
                    (bool (InputBackend::*)(JoystickButtonCode, int)) &InputBackend::GetButton, inputBackend,
                    buttonCode, joyNum);
            getButtonDown[buttonFuncIndex] = boost::bind(
                    (bool (InputBackend::*)(JoystickButtonCode, int)) &InputBackend::GetButtonDown, inputBackend,
                    buttonCode,
                    joyNum);
            getButtonUp[buttonFuncIndex] = boost::bind(
                    (bool (InputBackend::*)(JoystickButtonCode, int)) &InputBackend::GetButtonUp, inputBackend,
                    buttonCode, joyNum);
        }
    }
    else
    {
        // keyboard
        KeyCode keyCode = GetKeyCodeFromName(buttonName);

        if (keyCode < 0)
        {
            getButton[buttonFuncIndex] = [buttonName]()
            {
                LOG_ERROR << "Key \"" << buttonName << "\" Can Not Be Parsed!";
                return false;
            };
            getButtonDown[buttonFuncIndex] = [buttonName]()
            {
                LOG_ERROR << "Key \"" << buttonName << "\" Can Not Be Parsed!";
                return false;
            };
            getButtonUp[buttonFuncIndex] = [buttonName]()
            {
                LOG_ERROR << "Key \"" << buttonName << "\" Can Not Be Parsed!";
                return false;
            };
        }
        else
        {
            getButton[buttonFuncIndex] = boost::bind(&InputBackend::GetKey, inputBackend, keyCode);
            getButtonDown[buttonFuncIndex] = boost::bind(&InputBackend::GetKeyDown, inputBackend, keyCode);
            getButtonUp[buttonFuncIndex] = boost::bind(&InputBackend::GetKeyUp, inputBackend, keyCode);
        }
    }
}

void InputAxis::BindCallbackFunctions()
{
    if (type == Button)
    {
        if (invert)
        {
            BindButtonCallback(posButton, 2);
            BindButtonCallback(altPosButton, 3);
            BindButtonCallback(negButton, 0);
            BindButtonCallback(altNegButton, 1);
        }
        else
        {
            BindButtonCallback(posButton, 0);
            BindButtonCallback(altPosButton, 1);
            BindButtonCallback(negButton, 2);
            BindButtonCallback(altNegButton, 3);
        }

        getRawAxis = [this]()
        {
            float result = 0;

            if (getButton[0]() || getButton[1]()) result += 1;
            if (getButton[2]() || getButton[3]()) result -= 1;

            return result;
        };
    }
    else if (type == Axis)
    {
        for (int i = 0; i < 3; i += 2)
        {
            getButton[i] = []()
            {
                LOG_ERROR << "Can't call GetButton on an axis type input!";
                return false;
            };
            getButtonDown[i] = []()
            {
                LOG_ERROR << "Can't call GetButtonDown on an axis type input!";
                return false;
            };
            getButtonUp[i] = []()
            {
                LOG_ERROR << "Can't call GetButtonUp on an axis type input!";
                return false;
            };
        }

        JoystickAxisCode joystickAxisCode = GetJoystickAxisCodeFromAxisCode(axis);

        getRawAxis = [this, joystickAxisCode]()
        {
            return inputBackend->GetRawAxis(joystickAxisCode, joyNum) * (invert ? -1.0f : 1.0f);
        };
    }
    else if (type == Movement)
    {
        for (int i = 0; i < 3; i += 2)
        {
            getButton[i] = []()
            {
                LOG_ERROR << "Can't call GetButton on an movement type input!";
                return false;
            };
            getButtonDown[i] = []()
            {
                LOG_ERROR << "Can't call GetButtonDown on an movement type input!";
                return false;
            };
            getButtonUp[i] = []()
            {
                LOG_ERROR << "Can't call GetButtonUp on an movement type input!";
                return false;
            };
        }

        boost::function<double()> getMouseDelta;

        switch (axis)
        {
            case MouseX:
                getMouseDelta = boost::bind(&InputBackend::GetMouseDeltaX, inputBackend);
                break;
            case MouseY:
                getMouseDelta = boost::bind(&InputBackend::GetMouseDeltaY, inputBackend);
                break;
            case MouseWheel:
                getMouseDelta = [this]()
                {
                    return inputBackend->GetMouseDeltaWheel();
                };
                break;
            default:
                getMouseDelta = []()
                {
                    LOG_ERROR << "Can't get joystick axis with input type movement!";
                    return 0.0;
                };
                break;
        }

        getRawAxis = [this, getMouseDelta]()
        {
            auto mouseDelta = float(getMouseDelta());
            mouseDelta = mouseDelta * sensitivity * (invert ? -1.0f : 1.0f);
            return mouseDelta;
        };
    }
}

void InputAxis::Update(float deltaTime)
{
    const float rawData = getRawAxis();
    if (type == Axis)
    {
        if (abs(rawData) < dead)
            processedAxisValue = 0;
        else
            processedAxisValue = rawData;
    }
    else if (type == Button)
    {
        if (abs(rawData) <= dead)
        {
            if (processedAxisValue > 0)
            {
                processedAxisValue -= gravity * deltaTime;
                if (processedAxisValue < 0) processedAxisValue = 0;
            }
            else if (processedAxisValue < 0)
            {
                processedAxisValue += gravity * deltaTime;
                if (processedAxisValue > 0) processedAxisValue = 0;
            }
        }
        else
        {
            if (rawData > 0) processedAxisValue += sensitivity * deltaTime;
            else processedAxisValue -= sensitivity * deltaTime;

            if (processedAxisValue > 1.0f) processedAxisValue = 1.0f;
            else if (processedAxisValue < -1.0f) processedAxisValue = -1.0f;
        }
    }
    else if (type == Movement)
    {
        if (abs(rawData) < dead)
            processedAxisValue = 0;
        else
            processedAxisValue = rawData / (deltaTime * 7500.0f);
    }
}

InputBackend::InputBackend()
{
    presentedInputBackend = this;
}

InputBackend::~InputBackend()
= default;

void InputBackend::RegisterInput(const boost::container::string& name, const boost::container::string& posButton,
                                 const boost::container::string& negButton,
                                 const boost::container::string& altPosButton,
                                 const boost::container::string& altNegButton, float gravity, float dead,
                                 float sensitivity, bool invert, InputType type, AxisCode axis, int joyNum)
{
    inputs.emplace_back(name, posButton, negButton, altPosButton, altNegButton, gravity, dead, sensitivity, invert,
                        type, axis, joyNum, this);
}

bool InputBackend::GetButton(const boost::container::string& name)
{
    bool result = false;
    for (const auto& input : inputs)
    {
        if (input.name == name)
            result |= input.getButton[0]() || input.getButton[1]();
    }
    return result;
}

bool InputBackend::GetButtonDown(const boost::container::string& name)
{
    bool result = false;
    for (const auto& input : inputs)
    {
        if (input.name == name)
            result |= input.getButtonDown[0]() || input.getButtonDown[1]();
    }
    return result;
}

bool InputBackend::GetButtonUp(const boost::container::string& name)
{
    bool result = false;
    for (const auto& input : inputs)
    {
        if (input.name == name)
            result |= input.getButtonUp[0]() || input.getButtonUp[1]();
    }
    return result;
}

float InputBackend::GetAxis(const boost::container::string& name)
{
    float result = 0;
    for (const auto& input : inputs)
    {
        if (input.name == name)
            result += input.processedAxisValue;
    }
    return result;
}

float InputBackend::GetRawAxis(const boost::container::string& name)
{
    float result = 0;
    for (const auto& input : inputs)
    {
        if (input.name == name)
            result += input.getRawAxis();
    }
    return result;
}

void InputBackend::SyncUpdate(float deltaTime)
{
    for (auto& input : inputs)
    {
        input.Update(deltaTime);
    }
}


