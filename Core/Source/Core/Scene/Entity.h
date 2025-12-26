#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Core {

    class Entity
    {
    public:

#if PLACEHOLDER
        //For generating UUIDs
        uint32_t crc32(std::string input, uint32_t polynomial = 0xEDB88320)
        {
            const char *data = input.c_str();
            size_t len = input.length();
            uint32_t crc = -1;

            //Iterate to each char
            for(size_t ii = 0; ii < len; ++ii)
            {
                //bitwise XOR operation, using the current char
                crc ^= uint32_t(data[ii]);
                for(size_t jj = 0; jj < 8; ++jj)
                {
                    uint32_t mask = (crc & 1) ? ~(crc & 1) + 1 : 0;
                    crc = (crc >> 1) ^ (polynomial & mask);
                }
            }

            //Returning the bitwise complement of the crc value
            crc = ~crc;
            return crc;
        }

#endif

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
        entt::entity m_EntityHandle{ entt::null };

        Scene* m_Scene = nullptr;
    };

}
