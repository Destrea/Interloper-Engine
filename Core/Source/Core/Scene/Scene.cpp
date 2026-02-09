
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
        m_Registry.emplace<TransformComponent>(entity, glm::vec3(0.0f,0.0f,0.0f));

    }

    Scene::~Scene()
    {

    }

    void Scene::OnUpdate(float ts)
    {
        //Update scripts
        {
            //Updated from capturing [=] to [this,ts] to ensure that both "this" and the timestep are captured, while complying with C++20 standards
            m_Registry.view<NativeScriptComponent>().each([this, ts](auto entity, auto& nsc)
            {
                //TODO: Move to Scene::OnScenePlay
                if(!nsc.Instance)
                {
                    nsc.Instance = nsc.InstantiateScript();
                    nsc.Instance->m_Entity = Entity{entity, this};    //Setting the Scriptable entity's "entity" to be the current one
                    nsc.Instance->OnCreate();
                }

                nsc.Instance->OnUpdate(ts);

            });
        }

        //Scene Camera and rendering code here

    }



    //TODO: Add Scene::OnUpdate() and update the camera class
            //This will allow you to create a "scene" camera, independant from the player camera, and handle all the rendering, per-scene, rather than having all of the currently loaded scenes be rendered every frame.

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();

        auto& tag = entity.AddComponent<TagComponent>();
        // Make a way to dynamically name entities as "UnnamedEntity #"
        tag.Tag = name.empty() ? "UnnamedEntity" : name;


        return entity;
    }

    void Scene::DestroyEntity(Entity entity)
    {
        m_Registry.destroy(entity);
    }





}
