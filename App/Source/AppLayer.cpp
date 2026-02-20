
#include "AppLayer.h"

#include "Core/Application.h"

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/Shader.h"
#include "Core/InputManager.h"
#include "Core/ResourceManager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/Renderer/Model.h"
#include "Core/Scene/Entity.h"
#include "Core/Scene/Components.h"
#include "Core/Scene/ScriptableEntity.h"
#include "Core/MapLoader.h"
#include <print>

using namespace Core;

AppLayer::AppLayer()
{
    //Create Shader and load it.
    Renderer::Shader shader = Core::ResourceManager::LoadShader("Resources/Shaders/Vertex.glsl", "Resources/Shaders/Frag.glsl", "test");

    Renderer::Texture2D testTex = Core::ResourceManager::LoadTexture("Resources/Textures/wall.jpg", false, "wall");
    //m_ActiveScene = std::make_shared<Core::Scene>();
    m_ActiveScene = Core::Application::Get().GetActiveScene();

    Entity m_Player = m_ActiveScene->CreateEntity("Player");
    //m_Player.AddComponent<CameraComponent>(glm::vec3{0.0f,3.0f,0.0f});
    auto& playerTransform = m_Player.GetComponent<TransformComponent>();
    //m_Player.AddComponent<CameraComponent>(playerTransform.Translation, playerTransform.Rotation);
    m_Player.AddComponent<CameraComponent>();
    auto& playerCamera = m_Player.GetComponent<CameraComponent>();
    playerCamera.p_Camera.SetTransform(playerTransform.Translation, playerTransform.Rotation, playerTransform.Scale);
    playerCamera.p_Camera.SetAspectRatio(1920, 1080);



    m_PlayerEntity = m_Player;

    m_InputManager = std::make_shared<Core::InputManager>();

    m_Framebuffer = Core::Application::Get().GetFramebuffer();



    //Disable cursor by default.
    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Entity m_TestMap = m_ActiveScene->CreateEntity("TestMap");
    m_TestMap.AddComponent<MapDataComponent>("Resources/Maps/Test1.map");
    m_TestMap.AddComponent<ModelComponent>("Resources/Maps/Test1.obj");
    maps.push_back(m_TestMap);
    //Model test("Resources/Maps/Test1.obj");
    //MapData test1("Resources/Maps/Test1.map");

    //Set player camera position and transform

    m_PlayerEntity.GetComponent<CameraComponent>().p_Camera.SetPosition(m_PlayerEntity.GetComponent<CameraComponent>().p_Camera.GetPosition() + m_TestMap.GetComponent<MapDataComponent>().data.playerSpawn);
    m_PlayerEntity.GetComponent<TransformComponent>().Translation = glm:: vec4(m_PlayerEntity.GetComponent<CameraComponent>().p_Camera.GetPosition(), 1.0f);

    m_CurrentLevel = m_TestMap;
    auto& levelTC = m_CurrentLevel.GetComponent<TransformComponent>();
    levelTC.Scale = glm::vec3(1.0f/32.0f,1.0f/32.0f,1.0f/32.0f);
    levelTC.Translation = glm::vec3(0.0f, 0.0f, 0.0f);

    m_CurrentLevel.GetComponent<ModelComponent>().EntityShader = Core::ResourceManager::GetShader("test");;


    Renderer::Shader Othershader = Core::ResourceManager::LoadShader("Resources/Shaders/defaultVertex.glsl", "Resources/Shaders/defaultFragment.glsl", "Shader1");

    Renderer::Texture2D texture = Core::ResourceManager::LoadTexture("Resources/Textures/Debugempty.png", false, "wallTest");
    Othershader.setInteger("testTex", texture.ID);


    //Temporarily here, until I have a slightly more elegant way of writing individual scripts for each scriptable entity

    //Player Camera Controller Script.
    class CameraController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {


        }

        void OnDestroy()
        {

        }

        void OnUpdate(float ts)
        {
            GLFWwindow* window = Core::Application::Get().GetWindow()->GetHandle();
            auto& tc = this->GetComponent<TransformComponent>();
            auto& cc = this->GetComponent<CameraComponent>();


            if(Input::IsKeyPressed(GLFW_KEY_ESCAPE))
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                cc.p_Camera.SetCameraLock(false);
            }

            if(Input::IsKeyPressed(GLFW_KEY_O))
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                cc.p_Camera.SetCameraLock(true);
            }

            //Mouse and Keyboard camera input handling
            glm::vec2 mousePos = Input::GetMousePosition();


            float speed = 7.0f;

            glm::vec3 Front = cc.p_Camera.GetCameraFront();
            glm::vec3 Right = cc.p_Camera.GetCameraRight();
            glm::vec3 Up = cc.p_Camera.GetCameraUp();


            if (Input::IsKeyPressed(GLFW_KEY_W))
                tc.Translation += Front * speed * ts;
            if (Input::IsKeyPressed(GLFW_KEY_S))
                tc.Translation -= Front * speed * ts;
            if (Input::IsKeyPressed(GLFW_KEY_A))
                tc.Translation -= Right * speed * ts;
            if (Input::IsKeyPressed(GLFW_KEY_D))
                tc.Translation += Right * speed * ts;
            tc.Translation.y = 3.0f;

            cc.p_Camera.SetPosition(tc.Translation);

            cc.p_Camera.ProcessMouseMovement(mousePos.x, mousePos.y);

        }
    };

    m_PlayerEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();


    Renderer::Texture2D tex = Core::ResourceManager::GetTexture("wall");
    shader.setInteger("texture1", tex.ID);
}

AppLayer::~AppLayer()
{
    m_Framebuffer->Shutdown();
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

    m_ActiveScene->OnUpdate(ts);

}

void AppLayer::OnRender()
{

    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer->GetFBO());

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    //Scene rendering call
    m_ActiveScene->OnRender();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);

    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );

}



//The rest of these can be used for UI or Engine related handling

bool AppLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{

    //Mouse button press handling.
    //Double check with TheCherno github to see how its used.




    return false;
}

bool AppLayer::OnMouseMoved(Core::MouseMovedEvent& event)
{
    //Mouse movement handling.
    //m_MousePosition = {static_cast<float>(event.GetX()), static_cast<float>(event.GetY()) };
    return false;
}


bool AppLayer::OnWindowClosed(Core::WindowClosedEvent& event)
{
    //Window closing Handling.
    std::println("Window Closed!\n");
    AppLayer::~AppLayer();
    return false;
}

bool AppLayer::OnKeyPressed(Core::KeyPressedEvent& event)
{


    return false;
}




