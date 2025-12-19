
#include "AppLayer.h"

#include "Core/Application.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader.h"
#include "Core/InputManager.h"
#include "Core/ResourceManager.h"
#include "Core/Renderer/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/Renderer/Model.h"
#include "Core/Scene/Entity.h"
#include "Core/Scene/Components.h"
#include <print>

using namespace Core;

AppLayer::AppLayer()
{
    //Create Shader and load it.


    Renderer::Shader shader = Core::ResourceManager::LoadShader("Resources/Shaders/Vertex.glsl", "Resources/Shaders/Frag.glsl", "test");
    printf("Shader compile debug");
    Renderer::Texture2D testTex = Core::ResourceManager::LoadTexture("Resources/Textures/wall.jpg", false, "wall");
    m_ActiveScene = std::make_shared<Core::Scene>();
    //m_GuiLayer = std::make_shared<ImGuiLayer>();

    Entity m_Player = m_ActiveScene->CreateEntity("Player");
    m_Player.AddComponent<CameraComponent>(glm::vec3{0.0f,0.0f,3.0f});

    m_PlayerEntity = m_Player;

    m_InputManager = std::make_shared<Core::InputManager>();


    //Disable cursor by default.
    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Model test("Resources/Maps/unnamed.obj");
    maps.push_back(test);
    Renderer::Texture2D tex = Core::ResourceManager::GetTexture("wall");
    shader.setInteger("texture1", tex.ID);

    //squareEntity.HasComponent<TransformComponent>();
}

AppLayer::~AppLayer()
{
    glDeleteVertexArrays(1, &m_VertexArray);
    glDeleteBuffers(1, &m_VertexBuffer);

    glDeleteProgram(m_Shader);
}


void AppLayer::OnEvent(Core::Event& event)
{
    //Debugging console output, tied to every event.
    //std::println("{}", event.ToString());

    Core::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) {return OnMouseButtonPressed(e); });

    dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) {return OnMouseMoved(e);});

    dispatcher.Dispatch<Core::WindowClosedEvent>([this](Core::WindowClosedEvent& e) {return OnWindowClosed(e); });

    dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) {return OnKeyPressed(e); });
}

//Handle physics or player Input. Things that need to be tied to framerate/game tick system for accuracy or fluidity.
void AppLayer::OnUpdate(float ts)
{
    //Keyboard Input
    m_InputManager->processKeyboardInput(Core::Application::Get().GetWindow()->GetHandle(), &m_PlayerEntity.GetComponent<CameraComponent>().p_Camera, ts);
    m_InputManager->processMouseInput(&m_PlayerEntity.GetComponent<CameraComponent>().p_Camera, m_MousePosition.x, m_MousePosition.y);
    //Mouse Input

}

void AppLayer::OnRender()
{
    Renderer::Shader newShader = Core::ResourceManager::GetShader("test");

    Renderer::Texture2D tex = Core::ResourceManager::GetTexture("wall");

    glm::mat4 projection = glm::perspective(glm::radians(m_PlayerEntity.GetComponent<CameraComponent>().p_Camera.Zoom), (float)1920 / (float)1080, 0.1f, 100.0f);
    glm::mat4 view = m_PlayerEntity.GetComponent<CameraComponent>().p_Camera.GetViewMatrix();

    newShader.setMatrix4("projection", projection);
    newShader.setMatrix4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f/20.0f, 1.0f/20.0f, 1.0f/20.0f));	// it's a bit too big for our scene, so scale it down
    newShader.setMatrix4("model", model);

    //Core::MapScene test = maps[0];


    //Uniforms
    //glUniform1f(0, Core::Application::GetTime());

    //glm::vec2 framebufferSize = Core::Application::Get().GetFramebufferSize();
    //glUniform2f(1, framebufferSize.x, framebufferSize.y);

    //glViewport(0, 0, static_cast<GLsizei>(framebufferSize.x), static_cast<GLsizei>(framebufferSize.y));

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, tex.ID);



    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    newShader.Use();
    maps[0].Draw(newShader);


    //test.DrawMap(newShader);

    // Render
    //tex.IDglBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glBindTexture(GL_TEXTURE_2D, tex.ID);
    //glBindVertexArray(m_VertexArray);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
}





//The rest of these can be used for UI or Engine related handling

bool AppLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{

    //Mouse button press handling.
    //Double check with TheCherno github to see how its used.



    if(m_InputManager->get_cursor() == false)
    {
        glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_InputManager->set_cursor(true);
    }

    return false;
}

bool AppLayer::OnMouseMoved(Core::MouseMovedEvent& event)
{
    //Mouse movement handling.
    m_MousePosition = {static_cast<float>(event.GetX()), static_cast<float>(event.GetY()) };
    //
    return false;
}


bool AppLayer::OnWindowClosed(Core::WindowClosedEvent& event)
{
    //Window closing Handling.
    std::println("Window Closed!");
    AppLayer::~AppLayer();
    return false;
}

bool AppLayer::OnKeyPressed(Core::KeyPressedEvent& event)
{
    //Keyboard Input handling ONLY for AppLayer

    //If ESC is pressed, we unlock the mouse from the camera, and ignore camera input.
    if(event.GetKeyCode() == GLFW_KEY_ESCAPE)
    {
        //Re-enable cursor, and signal the Input Manager that the cursor is unlocked.
        glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_InputManager->set_cursor(false);
    }
    return false;
}




