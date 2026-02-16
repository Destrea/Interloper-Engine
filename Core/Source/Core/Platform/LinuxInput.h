
#pragma once

#include "Core/Input.h"

namespace Core
{
    class LinuxInput : public Input
    {
    protected:

        //Key Input
        virtual bool IsKeyPressedImpl(int keycode) override;

        //Mouse Input
        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual glm::vec2 GetMousePositionImpl() override;
        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;
    };
}
