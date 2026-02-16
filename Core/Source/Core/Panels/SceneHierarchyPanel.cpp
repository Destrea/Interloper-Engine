
#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "Core/Scene/Components.h"
#include "Core/Scene/Entity.h"

#include <glm/gtc/type_ptr.hpp>

//Temporary
#include "Core/Renderer/Shader.h"
#include "Core/ResourceManager.h"
#include "Core/Renderer/Texture.h"
#include "Core/Application.h"

namespace Core
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        //For now only a setter, but may do more later.
        m_Context = context;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        if(ImGui::Begin("SceneHierarchy"))
        {
            //ImGui::basic_registry's "each" was deprecated, so we're instead iterating using
            //entt's "view" iterator, and a lambda, to iterate through both the entities, and the components
            //looking for the tag components, so that they can be displayed.


            m_Context->m_Registry.view<TagComponent>().each([&](auto entityID, auto &tc)
            {
                //Get the engine's Entity Wrapper and pass it to the Draw Entity Node Function
                Entity entity{entityID, m_Context.get() };
                DrawEntityNode(entity);
            });

            //Tree item de-selection
            if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            {
                m_SelectionContext = {};
            }


            //Right click on a blank space. For creating new entitites

            if(ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems)  )
            {
                //Missing "over_item" boolean input. Find an alternative
                if(ImGui::MenuItem("Create Empty Entity"))
                {
                    m_Context->CreateEntity("Empty Entity");
                }
                ImGui::EndPopup();
            }

        }
        ImGui::End();

        ImGui::Begin("Properties");
        if(m_SelectionContext)
        {
            DrawComponents(m_SelectionContext);

            if (ImGui::Button("Add Component"))
                ImGui::OpenPopup("AddComponent");

            if(ImGui::BeginPopup("AddComponent"))
            {
                if(ImGui::MenuItem("Camera"))
                {
                    m_SelectionContext.AddComponent<CameraComponent>();
                    auto selectionTC = m_SelectionContext.GetComponent<TransformComponent>();
                    auto selectionCC = m_SelectionContext.GetComponent<CameraComponent>();
                    selectionCC.p_Camera.SetTransform(selectionTC.Translation, selectionTC.Rotation, selectionTC.Scale);
                    selectionCC.p_Camera.SetAspectRatio(1920, 1080);

                    ImGui::CloseCurrentPopup();
                }

                if(ImGui::MenuItem("Model Renderer"))
                {
                    //Open A menu to select the 3D Model that you want
                    //It'll take the path to the file you selected, and pass it into the ModelComponent Constructor.
                    //If nothing is provided, It'll default to a basic Cube.

                    //TODO: Implement model/file selction UI, and Update this to use it!

                    std::string path;

                    m_SelectionContext.AddComponent<ModelComponent>("Resources/DefaultModels/DefaultCube.obj");
                    //Add a placeholder Shader to it here.

                    Renderer::Shader Othershader = Core::ResourceManager::GetShader("Shader1");
                    Renderer::Texture2D myTexture = Core::ResourceManager::GetTexture("wallTest");
                    Othershader.setInteger("testTex", myTexture.ID);

                    m_SelectionContext.GetComponent<ModelComponent>().EntityShader = Othershader;

                    ImGui::CloseCurrentPopup();
                }

                if(ImGui::MenuItem("Native Script Component"))
                {
                    //TODO: Maybe make a "Scripting Components" file with all of the script classes

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
                            /*
                            GLFWwindow* window = Core::Application::Get().GetWindow()->GetHandle();
                            //glm::vec2 mousePos = m_MousePosition;
                            auto& tc = GetComponent<TransformComponent>();
                            auto& cc = GetComponent<CameraComponent>();
                            float speed = 7.0f;


                            glm::vec3 Front = cc.p_Camera.GetCameraFront();
                            glm::vec3 Right = cc.p_Camera.GetCameraRight();
                            glm::vec3 Up = cc.p_Camera.GetCameraUp();

                            //TODO: Figure out how to dynamically add Native Scripts to entities, so that you can use this CameraController to move the player/camera

                            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                                tc.Translation += Front * speed * ts;
                            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                                tc.Translation -= Front * speed * ts;
                            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                                tc.Translation -= Right * speed * ts;
                            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                                tc.Translation += Right * speed * ts;
                            tc.Translation.y = 3.0f;

                            //m_InputManager->processMouseInput(&cc.p_Camera, m_MousePosition.x, m_MousePosition.y);

                            */
                        }
                    };

                    m_SelectionContext.AddComponent<NativeScriptComponent>(new CameraController());
                    ImGui::CloseCurrentPopup();
                }



                ImGui::EndPopup();
            }



        }

        ImGui::End();

    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;
        //ImGui::Text("%s", tag.c_str());

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if(ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }


        bool entityDeleted = false;
        if(ImGui::BeginPopupContextItem())
        {
            //Missing "over_item" boolean input. Find an alternative
            if(ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;

            ImGui::EndPopup();
        }


        if(opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
            if(opened)
                ImGui::TreePop();

            ImGui::TreePop();
        }

        //Defer deletion until the end of the frame.
        if(entityDeleted)
        {
            m_Context->DestroyEntity(entity);
            if(m_SelectionContext == entity)
                m_SelectionContext = {};
        }

    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);

        //Label Column
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});
        //FontSize not found, using Scale
        float lineHeight = GImGui->Font->Scale + GImGui->Style.FramePadding.y * 6.0f;

        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight};


        //X button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});

        if (ImGui::Button("X",buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();


        //Y button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});

        if (ImGui::Button("Y",buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();


        //Z button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});

        if (ImGui::Button("Z",buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);


        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }


    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if(entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            //Could lead to buffer overflow, if the input string is greater than 256 bytes
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            if(ImGui::InputText("Tag",buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;


        if(entity.HasComponent<TransformComponent>())
        {
            bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform" );


            if(open)
            {
                auto& tc = entity.GetComponent<TransformComponent>();
                DrawVec3Control("Translation", tc.Translation);
                glm::vec3 rotation = glm::degrees(tc.Rotation);
                DrawVec3Control("Rotation", rotation);
                tc.Rotation = glm::radians(rotation);
                DrawVec3Control("Scale", tc.Scale, 1.0f);
                ImGui::TreePop();
            }


        }

        //TODO: Add actual Settings for CameraComponent
        //TODO: Rework editor camera, fix camera deletion bug

        if(entity.HasComponent<CameraComponent>())
        {
            if(ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera" ))
            {
                auto& cameraComponent = entity.GetComponent<CameraComponent>();

                //Add the new reworked camera components here in place of the commented options


                DrawVec3Control("Position", cameraComponent.p_Camera.m_Position);

                glm::vec3 localrot = glm::degrees(cameraComponent.p_Camera.m_Rotation);
                DrawVec3Control("LocalRotation", localrot);
                cameraComponent.p_Camera.m_Rotation = glm::radians(localrot);

                glm::vec3 camFront = cameraComponent.p_Camera.GetCameraFront();
                DrawVec3Control("CameraFront", camFront);
                glm::vec3 camUp = cameraComponent.p_Camera.GetCameraUp();
                DrawVec3Control("CameraUp", camUp);
                glm::vec3 camRight = cameraComponent.p_Camera.GetCameraRight();
                DrawVec3Control("CameraRight", camRight);

                glm::vec3 worldUp = cameraComponent.p_Camera.GetWorldUp();
                DrawVec3Control("WorldUp", worldUp);


                ImGui::TreePop();
            }

        }




        if(entity.HasComponent<ModelComponent>())
        {

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4,4});
            bool open = ImGui::TreeNodeEx((void*)typeid(ModelComponent).hash_code(), treeNodeFlags, "Model Component");

            ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
            if(ImGui::Button("+", ImVec2 {20,20}))
            {
                ImGui::OpenPopup("ComponentSettings");
            }
            ImGui::PopStyleVar();

            bool removeComponent = false;
            if(ImGui::BeginPopup("ComponentSettings"))
            {
                if(ImGui::MenuItem("Remove Component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if(open)
            {
                ImGui::Text("Model Component Placeholder");
                ImGui::TreePop();
            }

            if(removeComponent)
                entity.RemoveComponent<ModelComponent>();
        }


        if(entity.HasComponent<NativeScriptComponent>())
        {
            bool open = ImGui::TreeNodeEx((void*)typeid(NativeScriptComponent).hash_code(), treeNodeFlags, "Scriptable Entity");
            if(open)
            {
               ImGui::Text("NativeScriptComponent Placeholder");
               ImGui::TreePop();
            }

        }



    }

}
