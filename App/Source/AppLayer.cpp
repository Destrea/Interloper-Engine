
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


    class CameraController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {
            //printf("CameraController::OnCreate!");
            //std::cout << "CameraController::OnCreate!" << std::endl;
            //printf("CameraController::OnCreate!");
        }

        void OnDestroy()
        {

        }

        void OnUpdate(float ts)
        {
            auto& tc = GetComponent<TransformComponent>();
            auto& cc = GetComponent<CameraComponent>();

            //TODO: Figure this out, after reworking the camera system, so that each object can be scripted independently

            float speed = 7.0f;
            GLFWwindow* window = Core::Application::Get().GetWindow()->GetHandle();
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                tc.Translation.x += speed * ts;
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                tc.Translation.x -= speed * ts;
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                tc.Translation.z -= speed * ts;
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                tc.Translation.z += speed * ts;

            //Might not even need this anymore
            cc.p_Camera.SetPosition(tc.Translation);
            //position = tc.Translation;




            //tc.Translation = glm::vec4(position,1.0f);


            //GetComponent<CameraComponent>().p_Camera.updateCameraVectors();
        }
    };

    m_PlayerEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

    //m_PlayerEntity.GetComponent<CameraComponent>().p_Camera.Transform = m_PlayerEntity.GetComponent<TransformComponent>().Transform;

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
    //Keyboard Input
    //m_InputManager->processKeyboardInput(Core::Application::Get().GetWindow()->GetHandle(), &m_PlayerEntity.GetComponent<CameraComponent>().p_Camera, ts);
    //m_InputManager->processMouseInput(&m_PlayerEntity.GetComponent<CameraComponent>().p_Camera, m_MousePosition.x, m_MousePosition.y);
    m_InputManager->processPlayerInput(m_PlayerEntity, ts, m_MousePosition, Core::Application::Get().GetWindow()->GetHandle());

    m_ActiveScene->OnUpdate(ts);

    //TODO: Rework ALLLL of this so that camera movement and positioning is handled by the entity transform natively, instead of doing this hacky shit
    //Mouse Input

}

void AppLayer::OnRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer->GetFBO());
    Renderer::Shader newShader = Core::ResourceManager::GetShader("test");

    Renderer::Texture2D tex = Core::ResourceManager::GetTexture("wall");

    //glm::mat4 projection = glm::perspective(glm::radians(m_PlayerEntity.GetComponent<CameraComponent>().p_Camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
    glm::mat4 view = m_PlayerEntity.GetComponent<CameraComponent>().p_Camera.GetViewMatrix();

    newShader.setMatrix4("projection", projection);
    newShader.setMatrix4("view", view);
    glm::mat4 model = glm::mat4(1.0f);

    auto& tc = m_CurrentLevel.GetComponent<TransformComponent>();



    model = tc.GetTransform();


    newShader.setMatrix4("model", model);



    //m_Framebuffer->Bind();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    newShader.Use();
    maps[0].GetComponent<ModelComponent>().EntityModel.Draw(newShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);




    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();

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
    m_MousePosition = {static_cast<float>(event.GetX()), static_cast<float>(event.GetY()) };
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
    //Keyboard Input handling ONLY for AppLayer

    //If ESC is pressed, we unlock the mouse from the camera, and ignore camera input.
    if(event.GetKeyCode() == GLFW_KEY_ESCAPE)
    {
        //Re-enable cursor, and signal the Input Manager that the cursor is unlocked.
        glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_InputManager->set_cursor(false);
    }
    if(event.GetKeyCode() == GLFW_KEY_O)
    {
        //Re-enable cursor, and signal the Input Manager that the cursor is unlocked.
        glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_InputManager->set_cursor(true);
    }

    return false;
}




