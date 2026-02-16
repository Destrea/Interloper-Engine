
#pragma once

#include <glm/glm.hpp>

namespace Core
{

    class Input
    {
    public:
        //Key Input
        inline static bool IsKeyPressed( int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

        //Mouse Input
        inline static bool IsMouseButtonPressed( int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        inline static glm::vec2 GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
        inline static float GetMouseY() { return s_Instance->GetMouseXImpl(); }

    protected:
        //Key Input
        virtual bool IsKeyPressedImpl(int keycode) = 0;

        //Mouse Input
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual glm::vec2 GetMousePositionImpl() = 0;
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;

    private:
        static Input* s_Instance;
    };
}
