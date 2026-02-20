#include "LinuxInput.h"

#include <GLFW/glfw3.h>
#include "Core/Application.h"

namespace Core
{

    Input* Input::s_Instance = new LinuxInput();

    bool LinuxInput::IsKeyPressedImpl(int keycode)
    {
        //TODO: Platform window abstraction

        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetHandle());

        auto state = glfwGetKey( window, keycode);
        //Return if GLFW says that a key is pressed or held
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool LinuxInput::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetHandle());

        auto state = glfwGetMouseButton( window, button);
        //Return if GLFW says that a mouse button is pressed
        return state == GLFW_PRESS;
    }

    glm::vec2 LinuxInput::GetMousePositionImpl()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetHandle());
        double xPos, yPos;

        glfwGetCursorPos(window, &xPos, &yPos);
        return glm::vec2{(float)xPos, (float)yPos };
    }


    float LinuxInput::GetMouseXImpl()
    {
        glm::vec2 pos = GetMousePositionImpl();

        return pos.x;
    }

    float LinuxInput::GetMouseYImpl()
    {
        glm::vec2 pos = GetMousePositionImpl();

        return pos.y;

    }

}
