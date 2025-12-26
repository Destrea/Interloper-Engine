
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include <glm/glm.hpp>


namespace Core
{

    Scene::Scene()
    {


        entt::entity entity = m_Registry.create();

        //Can be used with the arugments (entity) to just attach the entity to a Transform component, or with (entity, glm::mat4(DATA)) to initialize the transform with specific mat4 data
        m_Registry.emplace<TransformComponent>(entity, glm::mat4(1));

    }

    Scene::~Scene()
    {

    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();

        auto& tag = entity.AddComponent<TagComponent>();
        // Make a way to dynamically name entities as "UnnamedEntity #"
        tag.Tag = name.empty() ? "UnnamedEntity" : name;
/*
        std::string newName = name;
        uint32_t ID_Tag;
        if(name.empty())
        {
           //Dynamically generate unnamedentity names
           newName = "UnnamedEntity";
           ID_Tag = crc32(newName);
        }
        else
        {
            ID_Tag = crc32(name);
        }
*/

        return entity;
    }



}
