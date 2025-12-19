
//#include "../include/Common.h"
#include "InputManager.h"
#include <assert.h>

namespace Core {
    static InputManager* s_InputManager = nullptr;

    InputManager::InputManager()
    {
        float SCR_WIDTH = Core::Application::Get().GetSpec().WindowSpec.Width;
        float SCR_HEIGHT = Core::Application::Get().GetSpec().WindowSpec.Height;
        s_InputManager = this;
        lastX = SCR_WIDTH / 2.0f;
        lastY = SCR_HEIGHT / 2.0f;
    }

    InputManager::~InputManager()
    {
        s_InputManager = nullptr;
    }

    void InputManager::set_cursor(bool val)
    {
        cursor_locked = val;
    }

    bool InputManager::get_cursor()
    {
        return cursor_locked;
    }

    void InputManager::processKeyboardInput(GLFWwindow* window, Camera *p_Camera, float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && get_cursor() == true)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            set_cursor(false);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            p_Camera->ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            p_Camera->ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            p_Camera->ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            p_Camera->ProcessKeyboard(RIGHT, deltaTime);
    }

    void InputManager::processMouseInput(Camera *p_Camera, double xposIn, double yposIn)
    {

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        //If the cursor is locked in place, we process camera mouse input.
        if(cursor_locked == true)
        {

            //Camera mouse calculations
            if(firstMouse)
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xOffset = xpos - lastX;
            float yOffset = lastY - ypos;

            lastX = xpos;
            lastY = ypos;

            p_Camera->ProcessMouseMovement(xOffset, yOffset);
        }
        else
        {
            //If the mouse isnt locked, we update the x and y pos so that the p_Camera doesnt whip when the mouse gets re-locked.
            lastX = xpos;
            lastY = ypos;
        }

    }


    InputManager& InputManager::Get()
    {
        assert(s_InputManager);
        return *s_InputManager;
    }


    //Mouse Handling


}
