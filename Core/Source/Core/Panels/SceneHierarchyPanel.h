#pragma once

#include "Core/Scene/Scene.h"
#include "Core/Base.h"
#include "Core/Scene/Entity.h"

namespace Core
{
    class SceneHierarchyPanel
    {
    public:
        //Create it with a reference to a specific scene
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);

        void SetContext(const Ref<Scene>& context);

        void OnImGuiRender();
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}
