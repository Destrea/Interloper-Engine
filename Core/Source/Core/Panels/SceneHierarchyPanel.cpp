
#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "Core/Scene/Components.h"
#include "Core/Scene/Entity.h"

#include <glm/gtc/type_ptr.hpp>

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
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
        if(ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        if(opened)
        {
            //TODO: Display child entities when the node selected has some

            ImGui::TreePop();
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

        if(entity.HasComponent<TransformComponent>())
        {
            if(ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform" ))
            {
            //TODO: Return to add more to Transform Components :)
            //TODO: Ensure that all entities are actually handled/modified using their transform components, instead of their old "local" position variables
            auto& tc = entity.GetComponent<TransformComponent>();
            DrawVec3Control("Translation", tc.Translation);
            glm::vec3 rotation = glm::degrees(tc.Rotation);
            DrawVec3Control("Rotation", rotation);
            tc.Rotation = glm::radians(rotation);
            DrawVec3Control("Scale", tc.Scale, 1.0f);
            ImGui::TreePop();
            }

        }

        if(entity.HasComponent<CameraComponent>())
        {
            if(ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera" ))
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
                glm::vec3 worldUp = cameraComponent.p_Camera.GetWorldUp();
                DrawVec3Control("WorldUp", worldUp);


                ImGui::TreePop();
            }

        }


    }

}
