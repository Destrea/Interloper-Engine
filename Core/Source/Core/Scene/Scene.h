#pragma once

#include "entt.hpp"
#include <glm/glm.hpp>
namespace Core
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void OnUpdate(float ts);
        void OnRender(glm::mat4 projection, glm::mat4 view);


        Entity CreateEntity(const std::string& name = std::string());
        void DestroyEntity(Entity entity);
    private:
        entt::registry m_Registry;

        friend class Entity;
        friend class SceneHierarchyPanel;
    };

}
