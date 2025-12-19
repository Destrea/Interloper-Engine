//Input manager
#pragma once

//#include "Common.h"
#include <GLFW/glfw3.h>
#include "Renderer/Camera.h"
#include "Layer.h"
#include "Application.h"
#include <memory>

namespace Core
{
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();

        bool get_cursor();
        void set_cursor(bool val);
        void processKeyboardInput(GLFWwindow *window, Camera *p_Camera, float deltaTime);
        void processMouseInput(Camera *p_Camera, double xposIn, double yposIn);

        bool cursor_locked = true;

        static InputManager& Get();

    private:

        //Mouse Variables
        float lastX;
        float lastY;
        bool firstMouse = true;

    };

}

