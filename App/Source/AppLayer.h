
#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include "Core/Layer.h"
#include "Core/Window.h"
#include "Core/InputManager.h"
#include "Core/Renderer/Camera.h"
#include "Core/InputEvents.h"
#include "Core/WindowEvents.h"
#include "Core/EditorLayer.h"
#include "Core/ResourceManager.h"
#include <glm/glm.hpp>
#include "Core/Renderer/Model.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Entity.h"
#include "Core/Renderer/Framebuffer.h"

class AppLayer : public Core::Layer
{
public:
    AppLayer();
    virtual ~AppLayer();

    virtual void OnEvent(Core::Event& event) override;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;
    std::vector<Model> maps;




private:
    bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);
    bool OnMouseMoved(Core::MouseMovedEvent& event);
    bool OnWindowClosed(Core::WindowClosedEvent& event);
    bool OnKeyPressed(Core::KeyPressedEvent& event);

    uint32_t m_Shader = 0;
    uint32_t m_VertexArray = 0;
    uint32_t m_VertexBuffer = 0;
    std::shared_ptr<EditorLayer> m_GuiLayer = nullptr;
    std::shared_ptr<Core::Window> m_Window = nullptr;
    std::shared_ptr<Core::InputManager> m_InputManager = nullptr;
    std::shared_ptr<Core::ResourceManager> m_ResourceManager = nullptr;
    std::shared_ptr<Core::Scene> m_ActiveScene = nullptr;
    std::shared_ptr<Renderer::Framebuffer> m_Framebuffer = nullptr;

    Core::Entity m_PlayerEntity;

    glm::vec2 m_MousePosition { 0.0f};

    float deltaTime = 0.0;
    friend class EditorLayer;
};
