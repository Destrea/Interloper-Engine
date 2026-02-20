//Input manager
#pragma once

//#include "Common.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Renderer/PerspectiveCamera.h"
#include "Layer.h"
#include "Application.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
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

        void processKeyboardInput(GLFWwindow *window, Renderer::PerspectiveCamera *p_Camera, float deltaTime);
        void processMouseInput(Renderer::PerspectiveCamera *p_Camera, double xposIn, double yposIn);

        void processPlayerInput(Entity playerEntity, float deltaTime, glm::vec2 mousePos, GLFWwindow* window);

        bool cursor_locked = true;

        static InputManager& Get();

    private:

        //Mouse Variables
        float lastX;
        float lastY;
        bool firstMouse = true;

    };

}

