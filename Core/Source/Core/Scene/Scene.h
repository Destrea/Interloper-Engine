#pragma once

#include "entt.hpp"

namespace Core
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();


        Entity CreateEntity(const std::string& name = std::string());
    private:
        entt::registry m_Registry;

        friend class Entity;
    };

}
