
#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include "Core/Scene/Components.h"
#include "Core/Scene/Entity.h"

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

}
