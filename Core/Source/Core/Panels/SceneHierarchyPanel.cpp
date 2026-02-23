
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





        }

        ImGui::End();

    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;
        //ImGui::Text("%s", tag.c_str());

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
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
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
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
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];


        ImGui::PushID(label.c_str());

        ImGui::Columns(2);

        //Label Column
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});
        //FontSize not found, using Scale
        float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight};


        //X button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});

        ImGui::PushFont(boldFont);
        if (ImGui::Button("X",buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();


        //Y button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y",buttonSize))
            values.y = resetValue;
        ImGui::PopFont();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();


        //Z button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z",buttonSize))
            values.z = resetValue;
        ImGui::PopFont();

        ImGui::PopStyleColor(3);


        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }


    //Template and lambda function for component UI drawing.
    //This allows each component to re-use the same code, dynamically filling what type of component it is
    //while also allowing us to use a lambda function to pass in any kind of "unique" function code that is needed for each different component.
    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
    {
        if(entity.HasComponent<T>())
        {
            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

            const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4,4});
            float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            if(ImGui::Button("+", ImVec2 {lineHeight,lineHeight}))
            {
                ImGui::OpenPopup("ComponentSettings");
            }


            bool removeComponent = false;
            if(ImGui::BeginPopup("ComponentSettings"))
            {
                if(ImGui::MenuItem("Remove Component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if(open)
            {

                uiFunction(component);
                ImGui::TreePop();
            }

            if(removeComponent)
                entity.RemoveComponent<T>();
        }

    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        //Cannot be removed from entity
        if(entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            //Could lead to buffer overflow, if the input string is greater than 256 bytes
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            if(ImGui::InputText("##Tag",buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if(ImGui::BeginPopup("AddComponent"))
        {

            //TODO: Move each of the things in here into their templated functions we created.
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
                //TODO: Maybe make a "Scripting Components" file with all of the script classe
                m_SelectionContext.AddComponent<NativeScriptComponent>();
                ImGui::CloseCurrentPopup();
            }



            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();




        //Cannot be removed from entity
        DrawComponent<TransformComponent>("Transform", entity, [](auto& component){
            auto& tc = component;
            DrawVec3Control("Translation", tc.Translation);
            glm::vec3 rotation = glm::degrees(tc.Rotation);
            DrawVec3Control("Rotation", rotation);
            tc.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", tc.Scale, 1.0f);
        });



        DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
        {
            //TODO: Add actual Settings for CameraComponent
            //TODO: Rework editor camera

            auto& cameraComponent = component;

            //Add the new reworked camera components here in place of the commented options
            glm::vec3 position = cameraComponent.p_Camera.GetPosition();
            glm::vec3 rotation = cameraComponent.p_Camera.GetRotation();
            DrawVec3Control("Position", position);

            glm::vec3 localrot = glm::degrees(rotation);
            DrawVec3Control("LocalRotation", localrot);
            cameraComponent.p_Camera.SetRotation(glm::radians(localrot));

            glm::vec3 camFront = cameraComponent.p_Camera.GetCameraFront();
            DrawVec3Control("CameraFront", camFront);
            glm::vec3 camUp = cameraComponent.p_Camera.GetCameraUp();
            DrawVec3Control("CameraUp", camUp);
            glm::vec3 camRight = cameraComponent.p_Camera.GetCameraRight();
            DrawVec3Control("CameraRight", camRight);

            glm::vec3 worldUp = cameraComponent.p_Camera.GetWorldUp();
            DrawVec3Control("WorldUp", worldUp);
        });


        DrawComponent<ModelComponent>("Model", entity, [](auto& component)
        {
            ImGui::Text("Model Component Placeholder");
        });

        DrawComponent<NativeScriptComponent>("Script", entity, [](auto& component)
        {
            ImGui::Text("NativeScriptComponent Placeholder");
        });

    }

}
