#pragma once


#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "Layer.h"
#include "Window.h"
#include "InputEvents.h"
#include "WindowEvents.h"
#include "InputManager.h"
#include "Application.h"
#include "Renderer/Framebuffer.h"

#include <memory>
#include <GLFW/glfw3.h>
#include <glad/glad.h>


class EditorLayer : public Core::Layer
{
public:
    EditorLayer();
    virtual ~EditorLayer();

    virtual void OnEvent(Core::Event& event) override;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;

    void EditorViewport();
    void BeginEngineUI();
    void EndEngineUI();
    void toggleHidden();
    void GetFPS();
private:
    bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);
    bool OnMouseMoved(Core::MouseMovedEvent& event);
    bool OnWindowClosed(Core::WindowClosedEvent& event);
    bool OnKeyPressed(Core::KeyPressedEvent& event);

    glm::vec2 m_ViewportSize;

    GLFWwindow* window = nullptr;
    std::shared_ptr<Core::Window> m_Window = nullptr;
    std::shared_ptr<Renderer::Framebuffer> m_Framebuffer = nullptr;
    ImGuiIO m_io;
    bool m_IsHidden = false;
    float m_Timestep;
    float m_Time = 0.0f;
    uint32_t textureID;
};
