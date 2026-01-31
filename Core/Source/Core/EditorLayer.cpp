
#include "EditorLayer.h"
using namespace Core;

#include <print>

EditorLayer::EditorLayer()
{
    BeginEngineUI();
    printf("Beginning EditorLayer!\n");
}

EditorLayer::~EditorLayer()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    printf("Killing EditorLayer!\n");
}


void EditorLayer::OnEvent(Core::Event& event)
{
    //Debugging console output, tied to every event.
    //std::println("{}", event.ToString());

    Core::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Core::MouseButtonPressedEvent>([this](Core::MouseButtonPressedEvent& e) {return OnMouseButtonPressed(e); });

    dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) {return OnMouseMoved(e);});

    dispatcher.Dispatch<Core::WindowClosedEvent>([this](Core::WindowClosedEvent& e) {return OnWindowClosed(e); });

    dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) {return OnKeyPressed(e); });
}



void EditorLayer::toggleHidden()
{
    m_IsHidden = !m_IsHidden;
}

void EditorLayer::OnUpdate(float ts)
{
    m_Timestep = ts;
    //m_ActiveScene->OnUpdate(ts);
}


void EditorLayer::EditorViewport()
{

}

void EditorLayer::OnRender()
{
    //TODO: Move all of this to a separate "ImGuiRender" class
    static ImGuiIO& io = ImGui::GetIO(); (void) io;
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();



    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("Wowie!");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

     m_SceneHierarchyPanel.OnImGuiRender();


    if(ImGui::Begin("Editor"))
    {
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0,0});
        bool isHovered = ImGui::IsWindowHovered();
        bool isFocused = ImGui::IsWindowFocused();

        if(isHovered)
        {
            //Mouse click to re-focus on window
        }
        if(isFocused)
        {
            //
        }

        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        if(m_ViewportSize != *((glm::vec2*)&windowSize))
        {
            m_Framebuffer->Rescale((uint32_t)windowSize.x, (uint32_t)windowSize.y);
            m_ViewportSize = {windowSize.x, windowSize.y};
            glViewport(0,0, m_Framebuffer->GetSpec().Width,m_Framebuffer->GetSpec().Height);
        }
        //glViewport(0,0, windowSize.x, windowSize.y);

        textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 centralizedCursorpos = (windowSize - pos) * 0.5f;

        ImGui::SetCursorPos((windowSize - ImVec2(m_Framebuffer->GetSpec().Width,m_Framebuffer->GetSpec().Height)) * 0.5f);
        ImVec2 size = ImVec2(m_Framebuffer->GetSpec().Width,m_Framebuffer->GetSpec().Height);
        ImGui::Image((GLuint*)(intptr_t)textureID, size, ImVec2(0,1), ImVec2(1,0));

        //ImGui::PopStyleVar();
        ImGui::End();
    }





#if Debug
    if(ImGui::Begin("Framebuffer Debug"))
    {
        ImGui::Text("Fbo Width/Height: %d / %d", m_Framebuffer->GetSpec().Width, m_Framebuffer->GetSpec().Height);
        ImGui::Text("Viewport Width/Height: %f / %f", m_ViewportSize.x, m_ViewportSize.y);
        //printf("Fbo Width/Height: %d / %d\n", m_Framebuffer->GetSpec().Width, m_Framebuffer->GetSpec().Height);
        //printf("Viewport Width/Height: %f / %f\n", m_ViewportSize.x, m_ViewportSize.y);
        ImGui::Separator();
        ImGui::Text("m_Framebuffer TextureID: %u", textureID);
        ImGui::End();
    }


    if(ImGui::Begin("Debug"))
    {
        ImGui::Text("FPS: %f", 1.0f / m_Timestep);
        ImGui::End();
    }
#endif



    //EditorViewport();


    ImGui::EndFrame();
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }


    if(!m_IsHidden)
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void EditorLayer::BeginEngineUI()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    static ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    //m_ActiveScene = std::make_shared<Core::Scene>();
    m_ActiveScene = Core::Application::Get().GetActiveScene();
    //m_EditorCamera = m_ActiveScene->CreateEntity("EditorCamera");
    //m_EditorCamera.AddComponent<CameraComponent>();

    m_Framebuffer = Core::Application::Get().GetFramebuffer();

    window = Core::Application::Get().GetWindow()->GetHandle();
    //ImGui window handling
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //Core::Entity m_Camera = m_ActiveScene->CreateEntity("EditorCamera");
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);

#if Testing
    //Editor Camera controller
    class CameraController : public ScriptableEntity
    {
    public:
        void OnCreate()
        {

            //std::cout << "CameraController::OnCreate!" << std::endl;
            //printf("CameraController::OnCreate!");
        }

        void OnDestroy()
        {

        }

        void OnUpdate(float ts)
        {


        }
    };

    //m_EditorCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
}

bool EditorLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{

    //Mouse button press handling.
    //Double check with TheCherno github to see how its used.
    /*


    if(m_InputManager->get_cursor() == false)
    {
        glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_InputManager->set_cursor(true);
    }
    */
    return false;
}

bool EditorLayer::OnMouseMoved(Core::MouseMovedEvent& event)
{
    //Mouse movement handling.
    return false;
}


bool EditorLayer::OnWindowClosed(Core::WindowClosedEvent& event)
{
    //Window closing Handling.
    std::println("Window Closed!\n");
    EditorLayer::~EditorLayer();
    return false;
}

bool EditorLayer::OnKeyPressed(Core::KeyPressedEvent& event)
{
    //Keyboard Input handling ONLY for AppLayer

    return false;
}

void EditorLayer::GetFPS()
{


}

