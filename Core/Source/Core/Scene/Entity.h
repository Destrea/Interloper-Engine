#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Core {

    class Entity
    {
    public:
        Entity() = default;

        Entity(entt::entity handle, Scene* scene);  //Constructor
        Entity(const Entity& other) = default;      //Copy Constructor

        //Variadic templating. Used for taking in a variable number of arguments.
        //Entt uses it too, so we just pass it along directly to Entt, for ease of use.
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            //Add asserts here, for easier debugging :)

            return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent()
        {
            //Add asserts here, for easier debugging :)

            return m_Scene->m_Registry.get<T>(m_EntityHandle);
        }

        template<typename T>
        bool HasComponent()
        {
            //Returns whether or not the registry has a component
            return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
        }

        template<typename T>
        void RemoveComponent()
        {
            //Add asserts here, for easier debugging :)
            m_Scene->m_Registry.remove<T>(m_EntityHandle);
        }

        operator bool() const { return m_EntityHandle != entt::null; }

    private:
        entt::entity m_EntityHandle = entt::null;

        Scene* m_Scene = nullptr;
    };

}
