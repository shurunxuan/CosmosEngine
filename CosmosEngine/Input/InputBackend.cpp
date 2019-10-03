//
// Created by vs1571 on 10/3/2019.
//

#include "InputBackend.h"
#include "../Logging/Logging.h"

#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/container/vector.hpp>


InputAxis::InputAxis(const boost::container::string& name, const boost::container::string& posButton,
                     const boost::container::string& negButton, const boost::container::string& altPosButton,
                     const boost::container::string& altNegButton, float gravity, float dead, float sensitivity,
                     bool invert, InputType type, AxisCode axis, int joyNum, InputBackend* backend)
        : name(name), posButton(posButton), negButton(negButton), altPosButton(altPosButton),
          altNegButton(altNegButton), gravity(gravity), dead(dead), sensitivity(sensitivity), invert(invert),
          type(type), axis(axis), joyNum(joyNum), inputBackend(backend)
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
            getButton[buttonFuncIndex] = boost::bind(&DSFRawInput::GetMouseButton, &inputBackend->rawInput, buttonCode);
            getButtonDown[buttonFuncIndex] = boost::bind(&DSFRawInput::GetMouseButtonDown, &inputBackend->rawInput,
                                                         buttonCode);
            getButtonUp[buttonFuncIndex] = boost::bind(&DSFRawInput::GetMouseButtonUp, &inputBackend->rawInput,
                                                       buttonCode);
        }
    }
    else if (boost::starts_with(buttonName, "joystick"))
    {
        // joystick

        // button name
        boost::container::vector<boost::container::string> strs;
        split(strs, buttonName, boost::is_any_of(" "));

        JoystickButtonCode buttonCode = GetJoystickButtonCodeFromName(strs[1]);

        if (buttonCode == RESERVED_0)
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
            getButton[buttonFuncIndex] = std::bind(&DSFXInput::GetButton, &inputBackend->xInput, buttonCode, joyNum);
            getButtonDown[buttonFuncIndex] = std::bind(&DSFXInput::GetButtonDown, &inputBackend->xInput, buttonCode,
                                                       joyNum);
            getButtonUp[buttonFuncIndex] = std::bind(&DSFXInput::GetButtonUp, &inputBackend->xInput, buttonCode,
                                                     joyNum);
        }
    }
    else
    {
        // keyboard
        int keyCode = GetVirtualKeyCodeFromName(buttonName);

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
            getButton[buttonFuncIndex] = boost::bind(&DSFRawInput::GetKey, &inputBackend->rawInput, keyCode);
            getButtonDown[buttonFuncIndex] = boost::bind(&DSFRawInput::GetKeyDown, &inputBackend->rawInput, keyCode);
            getButtonUp[buttonFuncIndex] = boost::bind(&DSFRawInput::GetKeyUp, &inputBackend->rawInput, keyCode);
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
            return inputBackend->xInput.GetRawAxis(joystickAxisCode, joyNum) * (invert ? -1 : 1);
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

        boost::function<long()> getMouseDelta;

        switch (axis)
        {
            case MouseX:
                getMouseDelta = boost::bind(&DSFRawInput::GetMouseDeltaX, &inputBackend->rawInput);
                break;
            case MouseY:
                getMouseDelta = boost::bind(&DSFRawInput::GetMouseDeltaY, &inputBackend->rawInput);
                break;
            case MouseWheel:
                getMouseDelta = [this]()
                {
                    return long(inputBackend->rawInput.GetMouseWheelDelta());
                };
                break;
            default:
                getMouseDelta = []()
                {
                    LOG_ERROR << "Can't get joystick axis with input type movement!";
                    return long(0);
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


