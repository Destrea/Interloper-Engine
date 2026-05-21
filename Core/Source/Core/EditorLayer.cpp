
#include "EditorLayer.h"
#include "Scene/SceneSerializer.h"
#include "Utils/PlatformUtils.h"

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


    //ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();

    //Forcing minimum window size on "Dockspace" windows, for clarity in viewing
    float minWinSize = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;


    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {

            //TODO: New Scene
            if(ImGui::MenuItem("New", "Ctrl+N"))
            {
                NewScene();
            }


            //TODO: Save (As current file)
            if (ImGui::MenuItem("Save...", "Ctrl+S"))
            {

                //Check if a file has already been loaded. If so, use the same file path.
                /*
                nfdchar_t *savePath = NULL;
                nfdresult_t result = NFD_SaveDialog("sctxt", NULL, &savePath);
                if(result == NFD_OKAY)
                {
                    printf("SUCCESS!\n Path: %s\n", savePath);
                    std::string path = std::string(savePath);
                    if(!path.contains(".sctxt"))
                    {
                        path.append(".sctxt");
                    }
                    printf("Path value: %s", path.c_str());
                    SceneSerializer serializer(m_ActiveScene);
                    serializer.Serialize(savePath);
                    free(savePath);
                }
                */

            }

            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
            {
                SaveSceneAs();
            }

            if (ImGui::MenuItem("Open...", "Ctrl+O"))
            {
                OpenScene();
            }


            ImGui::MenuItem("Wowie!");
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }


    //TODO: Update dockspace code when re-doing ImGui layer code
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    m_SceneHierarchyPanel.OnImGuiRender();

    style.WindowMinSize.x = minWinSize;

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
    //TODO: Migrate all of this into its own ImGuiLayer cpp file

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    static ImGuiIO& io = ImGui::GetIO(); (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    //Adds new fonts to the io
    io.Fonts->AddFontFromFileTTF("Resources/Fonts/opensans/static/OpenSans-Bold.ttf", 18.0f);   //Font [1] is Bolded OpenSans
    io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/opensans/static/OpenSans-Regular.ttf", 18.0f);

    //Theme coloring
    SetDarkThemeColors();


    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    m_ActiveScene = Core::Application::Get().GetActiveScene();
    m_InputManager = std::make_shared<Core::InputManager>();
    m_Framebuffer = Core::Application::Get().GetFramebuffer();

    window = Core::Application::Get().GetWindow()->GetHandle();
    //ImGui window handling
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_SceneHierarchyPanel.SetContext(m_ActiveScene);





}

bool EditorLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{

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
    EditorLayer::~EditorLayer();
    return false;
}

bool EditorLayer::OnKeyPressed(Core::KeyPressedEvent& event)
{

    //New Scene "Ctrl+N"
    if((Input::IsKeyPressed(GLFW_KEY_N) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) || (Input::IsKeyPressed(GLFW_KEY_N) && Input::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL)))
    {
        NewScene();
    }


    //Save As... "Ctrl+Shift+S"
    if((Input::IsKeyPressed(GLFW_KEY_S) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) && Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT)))
    {

        SaveSceneAs();
    }

    //Open Scene "Ctrl+O"
    if(Input::IsKeyPressed(GLFW_KEY_O) && Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
    {
        OpenScene();
    }



    return false;
}


//TODO: Move to ImGuiLayer later
void EditorLayer::SetDarkThemeColors()
{
    auto& colors = ImGui::GetStyle().Colors;

    //TODO: Add Colors here. Refer to Demo project or video to see how an example line is done.

}


void EditorLayer::NewScene()
{
    m_ActiveScene = CreateRef<Scene>();
    //m_ActiveScene
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
}

void EditorLayer::SaveSceneAs()
{
    std::string filePath = FileDialogs::SaveFile("sctxt");
    if(filePath != "")
    {

        if(!filePath.contains(".sctxt"))
        {
            filePath.append(".sctxt");
        }

        SceneSerializer serializer(m_ActiveScene);
        serializer.Serialize(filePath);

    }
}

void EditorLayer::OpenScene()
{
    //Reset the selected scene, so that it deserializes onto a fresh scene.
    m_ActiveScene = CreateRef<Scene>();
    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    //File (Open) selection dialog. Should work across platforms.
    std::string filePath = FileDialogs::OpenFile("sctxt");
    if(filePath != "")
    {
        //printf("SUCCESS!]\n Path: %s\n", outPath);
        SceneSerializer serializer(m_ActiveScene);
        serializer.Deserialize(filePath);
    }
}





