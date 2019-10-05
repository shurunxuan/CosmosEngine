//
// Created by 西村智 on 10/4/19.
//

#include "GLFWInputBackend.h"
#include "../../Logging/Logging.h"
#include <limits>
#include <cmath>

GLFWInputBackend* glfwInputBackend = nullptr;

int GetFrameworkKeyCode(KeyCode keyCode)
{
    switch (keyCode)
    {
        case Backspace:
            return GLFW_KEY_BACKSPACE;
        case Delete:
            return GLFW_KEY_DELETE;
        case Tab:
            return GLFW_KEY_TAB;
        case Return:
            return GLFW_KEY_ENTER;
        case Escape:
            return GLFW_KEY_ESCAPE;
        case Space:
            return GLFW_KEY_SPACE;
        case Keypad0:
            return GLFW_KEY_KP_0;
        case Keypad1:
            return GLFW_KEY_KP_1;
        case Keypad2:
            return GLFW_KEY_KP_2;
        case Keypad3:
            return GLFW_KEY_KP_3;
        case Keypad4:
            return GLFW_KEY_KP_4;
        case Keypad5:
            return GLFW_KEY_KP_5;
        case Keypad6:
            return GLFW_KEY_KP_6;
        case Keypad7:
            return GLFW_KEY_KP_7;
        case Keypad8:
            return GLFW_KEY_KP_8;
        case Keypad9:
            return GLFW_KEY_KP_9;
        case KeypadPeriod:
            return GLFW_KEY_KP_DECIMAL;
        case KeypadDivide:
            return GLFW_KEY_KP_DIVIDE;
        case KeypadMultiply:
            return GLFW_KEY_KP_MULTIPLY;
        case KeypadMinus:
            return GLFW_KEY_KP_SUBTRACT;
        case KeypadPlus:
            return GLFW_KEY_KP_ADD;
        case KeypadEnter:
            return GLFW_KEY_KP_ENTER;
        case Up:
            return GLFW_KEY_UP;
        case Down:
            return GLFW_KEY_DOWN;
        case Right:
            return GLFW_KEY_RIGHT;
        case Left:
            return GLFW_KEY_LEFT;
        case Insert:
            return GLFW_KEY_INSERT;
        case Home:
            return GLFW_KEY_HOME;
        case End:
            return GLFW_KEY_END;
        case PageUp:
            return GLFW_KEY_PAGE_UP;
        case PageDown:
            return GLFW_KEY_PAGE_DOWN;
        case F1:
            return GLFW_KEY_F1;
        case F2:
            return GLFW_KEY_F2;
        case F3:
            return GLFW_KEY_F3;
        case F4:
            return GLFW_KEY_F4;
        case F5:
            return GLFW_KEY_F5;
        case F6:
            return GLFW_KEY_F6;
        case F7:
            return GLFW_KEY_F7;
        case F8:
            return GLFW_KEY_F8;
        case F9:
            return GLFW_KEY_F9;
        case F10:
            return GLFW_KEY_F10;
        case F11:
            return GLFW_KEY_F11;
        case F12:
            return GLFW_KEY_F12;
        case F13:
            return GLFW_KEY_F13;
        case F14:
            return GLFW_KEY_F14;
        case F15:
            return GLFW_KEY_F15;
        case Number0:
            return GLFW_KEY_0;
        case Number1:
            return GLFW_KEY_1;
        case Number2:
            return GLFW_KEY_2;
        case Number3:
            return GLFW_KEY_3;
        case Number4:
            return GLFW_KEY_4;
        case Number5:
            return GLFW_KEY_5;
        case Number6:
            return GLFW_KEY_6;
        case Number7:
            return GLFW_KEY_7;
        case Number8:
            return GLFW_KEY_8;
        case Number9:
            return GLFW_KEY_9;
        case Quote:
            return GLFW_KEY_APOSTROPHE;
        case Comma:
            return GLFW_KEY_COMMA;
        case Minus:
            return GLFW_KEY_MINUS;
        case Period:
            return GLFW_KEY_PERIOD;
        case Slash:
            return GLFW_KEY_SLASH;
        case Semicolon:
            return GLFW_KEY_SEMICOLON;
        case Equal:
            return GLFW_KEY_EQUAL;
        case LeftBracket:
            return GLFW_KEY_LEFT_BRACKET;
        case Backslash:
            return GLFW_KEY_BACKSLASH;
        case RightBracket:
            return GLFW_KEY_RIGHT_BRACKET;
        case BackQuote:
            return GLFW_KEY_GRAVE_ACCENT;
        case A:
            return GLFW_KEY_A;
        case B:
            return GLFW_KEY_B;
        case C:
            return GLFW_KEY_C;
        case D:
            return GLFW_KEY_D;
        case E:
            return GLFW_KEY_E;
        case F:
            return GLFW_KEY_F;
        case G:
            return GLFW_KEY_G;
        case H:
            return GLFW_KEY_H;
        case I:
            return GLFW_KEY_I;
        case J:
            return GLFW_KEY_J;
        case K:
            return GLFW_KEY_K;
        case L:
            return GLFW_KEY_L;
        case M:
            return GLFW_KEY_M;
        case N:
            return GLFW_KEY_N;
        case O:
            return GLFW_KEY_O;
        case P:
            return GLFW_KEY_P;
        case Q:
            return GLFW_KEY_Q;
        case R:
            return GLFW_KEY_R;
        case S:
            return GLFW_KEY_S;
        case T:
            return GLFW_KEY_T;
        case U:
            return GLFW_KEY_U;
        case V:
            return GLFW_KEY_V;
        case W:
            return GLFW_KEY_W;
        case X:
            return GLFW_KEY_X;
        case Y:
            return GLFW_KEY_Y;
        case Z:
            return GLFW_KEY_Z;
        case NumLock:
            return GLFW_KEY_NUM_LOCK;
        case CapsLock:
            return GLFW_KEY_CAPS_LOCK;
        case ScrollLock:
            return GLFW_KEY_SCROLL_LOCK;
        case RightShift:
            return GLFW_KEY_RIGHT_SHIFT;
        case LeftShift:
            return GLFW_KEY_LEFT_SHIFT;
        case RightControl:
            return GLFW_KEY_RIGHT_CONTROL;
        case LeftControl:
            return GLFW_KEY_LEFT_CONTROL;
        case RightAlt:
            return GLFW_KEY_RIGHT_ALT;
        case LeftAlt:
            return GLFW_KEY_LEFT_ALT;
        case LeftWindows:
            return GLFW_KEY_LEFT_SUPER;
        case RightWindows:
            return GLFW_KEY_RIGHT_SUPER;
        case Menu:
            return GLFW_KEY_MENU;
        case ErrorKey:
            return GLFW_KEY_UNKNOWN;
    }
    return GLFW_KEY_UNKNOWN;
}

void SetupKeyStatesMap(boost::unordered_map<KeyCode, bool>& map)
{
    map.insert(std::make_pair(Backspace, false));
    map.insert(std::make_pair(Delete, false));
    map.insert(std::make_pair(Tab, false));
    map.insert(std::make_pair(Return, false));
    map.insert(std::make_pair(Escape, false));
    map.insert(std::make_pair(Space, false));
    map.insert(std::make_pair(Keypad0, false));
    map.insert(std::make_pair(Keypad1, false));
    map.insert(std::make_pair(Keypad2, false));
    map.insert(std::make_pair(Keypad3, false));
    map.insert(std::make_pair(Keypad4, false));
    map.insert(std::make_pair(Keypad5, false));
    map.insert(std::make_pair(Keypad6, false));
    map.insert(std::make_pair(Keypad7, false));
    map.insert(std::make_pair(Keypad8, false));
    map.insert(std::make_pair(Keypad9, false));
    map.insert(std::make_pair(KeypadPeriod, false));
    map.insert(std::make_pair(KeypadDivide, false));
    map.insert(std::make_pair(KeypadMultiply, false));
    map.insert(std::make_pair(KeypadMinus, false));
    map.insert(std::make_pair(KeypadPlus, false));
    map.insert(std::make_pair(KeypadEnter, false));
    map.insert(std::make_pair(Up, false));
    map.insert(std::make_pair(Down, false));
    map.insert(std::make_pair(Right, false));
    map.insert(std::make_pair(Left, false));
    map.insert(std::make_pair(Insert, false));
    map.insert(std::make_pair(Home, false));
    map.insert(std::make_pair(End, false));
    map.insert(std::make_pair(PageUp, false));
    map.insert(std::make_pair(PageDown, false));
    map.insert(std::make_pair(F1, false));
    map.insert(std::make_pair(F2, false));
    map.insert(std::make_pair(F3, false));
    map.insert(std::make_pair(F4, false));
    map.insert(std::make_pair(F5, false));
    map.insert(std::make_pair(F6, false));
    map.insert(std::make_pair(F7, false));
    map.insert(std::make_pair(F8, false));
    map.insert(std::make_pair(F9, false));
    map.insert(std::make_pair(F10, false));
    map.insert(std::make_pair(F11, false));
    map.insert(std::make_pair(F12, false));
    map.insert(std::make_pair(F13, false));
    map.insert(std::make_pair(F14, false));
    map.insert(std::make_pair(F15, false));
    map.insert(std::make_pair(Number0, false));
    map.insert(std::make_pair(Number1, false));
    map.insert(std::make_pair(Number2, false));
    map.insert(std::make_pair(Number3, false));
    map.insert(std::make_pair(Number4, false));
    map.insert(std::make_pair(Number5, false));
    map.insert(std::make_pair(Number6, false));
    map.insert(std::make_pair(Number7, false));
    map.insert(std::make_pair(Number8, false));
    map.insert(std::make_pair(Number9, false));
    map.insert(std::make_pair(Quote, false));
    map.insert(std::make_pair(Comma, false));
    map.insert(std::make_pair(Minus, false));
    map.insert(std::make_pair(Period, false));
    map.insert(std::make_pair(Slash, false));
    map.insert(std::make_pair(Semicolon, false));
    map.insert(std::make_pair(Equal, false));
    map.insert(std::make_pair(LeftBracket, false));
    map.insert(std::make_pair(Backslash, false));
    map.insert(std::make_pair(RightBracket, false));
    map.insert(std::make_pair(BackQuote, false));
    map.insert(std::make_pair(A, false));
    map.insert(std::make_pair(B, false));
    map.insert(std::make_pair(C, false));
    map.insert(std::make_pair(D, false));
    map.insert(std::make_pair(E, false));
    map.insert(std::make_pair(F, false));
    map.insert(std::make_pair(G, false));
    map.insert(std::make_pair(H, false));
    map.insert(std::make_pair(I, false));
    map.insert(std::make_pair(J, false));
    map.insert(std::make_pair(K, false));
    map.insert(std::make_pair(L, false));
    map.insert(std::make_pair(M, false));
    map.insert(std::make_pair(N, false));
    map.insert(std::make_pair(O, false));
    map.insert(std::make_pair(P, false));
    map.insert(std::make_pair(Q, false));
    map.insert(std::make_pair(R, false));
    map.insert(std::make_pair(S, false));
    map.insert(std::make_pair(T, false));
    map.insert(std::make_pair(U, false));
    map.insert(std::make_pair(V, false));
    map.insert(std::make_pair(W, false));
    map.insert(std::make_pair(X, false));
    map.insert(std::make_pair(Y, false));
    map.insert(std::make_pair(Z, false));
    map.insert(std::make_pair(NumLock, false));
    map.insert(std::make_pair(CapsLock, false));
    map.insert(std::make_pair(ScrollLock, false));
    map.insert(std::make_pair(RightShift, false));
    map.insert(std::make_pair(LeftShift, false));
    map.insert(std::make_pair(RightControl, false));
    map.insert(std::make_pair(LeftControl, false));
    map.insert(std::make_pair(RightAlt, false));
    map.insert(std::make_pair(LeftAlt, false));
    map.insert(std::make_pair(LeftWindows, false));
    map.insert(std::make_pair(RightWindows, false));
    map.insert(std::make_pair(Menu, false));
    map.insert(std::make_pair(ErrorKey, false));
}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    glfwInputBackend->CursorPositionCallback(xpos, ypos);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
}

GLFWInputBackend::GLFWInputBackend()
        : InputBackend()
{
    glfwInputBackend = this;
    window = nullptr;
    mouseX = 0.0;
    mouseY = 0.0;
    lastMouseX = std::numeric_limits<double>::quiet_NaN();
    lastMouseY = std::numeric_limits<double>::quiet_NaN();
    mouseDeltaX = 0.0;
    mouseDeltaY = 0.0;
}

GLFWInputBackend::~GLFWInputBackend()
{

}

void GLFWInputBackend::StartUp(void* userData)
{
    LOG_INFO << "Initializing GLFW Input Backend";

    window = reinterpret_cast<GLFWwindow*>(userData);

    SetupKeyStatesMap(keyPressStates);
    SetupKeyStatesMap(keyHoldStates);
    SetupKeyStatesMap(keyReleaseStates);

    mouseButtonPressStates.resize(6, false);
    mouseButtonHoldStates.resize(6, false);
    mouseButtonReleaseStates.resize(6, false);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void GLFWInputBackend::AsyncUpdate(float deltaTime)
{

}

void GLFWInputBackend::SyncUpdate(float deltaTime)
{
    InputBackend::SyncUpdate(deltaTime);
    
    glfwPollEvents();

    for (auto& k : keyHoldStates)
    {
        int fwKeyCode = GetFrameworkKeyCode(k.first);

        bool press = glfwGetKey(window, fwKeyCode) == GLFW_PRESS;

        keyPressStates[k.first] = press && !k.second && !keyPressStates[k.first];
        keyReleaseStates[k.first] = !press && k.second && !keyReleaseStates[k.first];

        k.second = press;
    }

    for (size_t i = 0; i < 6; ++i)
    {
        int buttonCode = static_cast<int>(i);

        bool press = glfwGetMouseButton(window, buttonCode) == GLFW_PRESS;

        mouseButtonPressStates[i] = press && !mouseButtonHoldStates[i] && !mouseButtonPressStates[i];
        mouseButtonReleaseStates[i] = !press && mouseButtonHoldStates[i] && !mouseButtonReleaseStates[i];

        mouseButtonHoldStates[i] = press;
    }

    if (!std::isnan(lastMouseX) && !std::isnan(lastMouseY))
    {
        mouseDeltaX = mouseX - lastMouseX;
        mouseDeltaY = mouseY - lastMouseY;

        lastMouseX = mouseX;
        lastMouseY = mouseY;
    }
}

bool GLFWInputBackend::GetMouseButton(int mouseButtonCode)
{
    return mouseButtonHoldStates[mouseButtonCode];
}

bool GLFWInputBackend::GetMouseButtonDown(int mouseButtonCode)
{
    return mouseButtonPressStates[mouseButtonCode];
}

bool GLFWInputBackend::GetMouseButtonUp(int mouseButtonCode)
{
    return mouseButtonReleaseStates[mouseButtonCode];
}

bool GLFWInputBackend::GetButton(JoystickButtonCode joystickButtonCode, int joystickNumber)
{
    return false;
}

bool GLFWInputBackend::GetButtonDown(JoystickButtonCode joystickButtonCode, int joystickNumber)
{
    return false;
}

bool GLFWInputBackend::GetButtonUp(JoystickButtonCode joystickButtonCode, int joystickNumber)
{
    return false;
}

bool GLFWInputBackend::GetKey(KeyCode keyCode)
{
    return keyHoldStates[keyCode];
}

bool GLFWInputBackend::GetKeyDown(KeyCode keyCode)
{
    return keyPressStates[keyCode];
}

bool GLFWInputBackend::GetKeyUp(KeyCode keyCode)
{
    return keyReleaseStates[keyCode];
}

float GLFWInputBackend::GetRawAxis(JoystickAxisCode axisCode, int joystickNumber)
{
    return 0;
}

double GLFWInputBackend::GetMouseDeltaX()
{
    return mouseDeltaX;
}

double GLFWInputBackend::GetMouseDeltaY()
{
    return mouseDeltaY;
}

double GLFWInputBackend::GetMouseDeltaWheel()
{
    return 0;
}

void GLFWInputBackend::CursorPositionCallback(double xPos, double yPos)
{
    mouseX = xPos;
    mouseY = yPos;
    if (std::isnan(lastMouseX)) lastMouseX = xPos;
    if (std::isnan(lastMouseY)) lastMouseY = yPos;
}
