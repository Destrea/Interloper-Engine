
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
//#include <glm/glm.hpp>


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


        //TODO: Add Scene::OnUpdate() and update the camera class
        //This will allow you to create a "scene" camera, independant from the player camera, and handle all the rendering, per-scene, rather than having all of the currently loaded scenes be rendered every frame.

        m_Registry.view<CameraComponent>().each([&](auto entityID, auto &cc)
        {
            Entity entity{entityID, this};
            auto& EntityCamera = cc;



        });
    }

    //Scene rendering pass
    void Scene::OnRender()
    {
        bool mainCamera = false;
        glm::mat4 projection = {1.0f};
        glm::mat4 view = {1.0f};
        //When theres a primary camera in the scene, we can add values to the projection and view matrix.
        m_Registry.view<CameraComponent>().each([&](auto entityID, auto &cc)
        {
            Entity entity{entityID, this};
            auto& EntityCamera = cc;
            projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
            view = entity.GetComponent<CameraComponent>().p_Camera.GetViewMatrix2();
            mainCamera = true;
        });

        //If there is no camera in the scene, then mainCamera remains false, and no rendering is done, since we wont have any projection or view matrix calculations.
        if(mainCamera)
        {
            //Renders all Entities in the scene that contain a ModelComponent
            m_Registry.view<ModelComponent, TransformComponent>().each([&](auto entityID, auto &mc, auto &tc)
            {
                //creates an Entity object using the entityID the iterator found
                Entity entity{entityID, this};

                //Takes the shader object and passes it into the Draw function of each ModelComponent, for rendering
                auto& EntityModel = mc;
                Renderer::Shader EntityShader = Core::ResourceManager::GetShader(mc.ShaderName);

                Renderer::Texture2D EntityTexture = Core::ResourceManager::GetTexture(mc.TexName);
                printf("TexID = %d", EntityTexture.ID);

                //Activate the current entity's shader
                EntityShader.Use();

                //Pass the model/projection/view matrices to the shader
                glm::mat4 model = glm::mat4(1.0f);
                model = entity.GetComponent<TransformComponent>().GetTransform();
                EntityShader.setMatrix4("model", model);
                EntityShader.setMatrix4("projection", projection);
                EntityShader.setMatrix4("view", view);
                EntityShader.setInteger("testTex", EntityTexture.ID);

                //Draws the entity's model, using the shader
                mc.EntityModel.Draw(EntityShader);

            });
        }

    }


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


    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
        static_assert(false);
    }

    template<>
    void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<ModelComponent>(Entity entity, ModelComponent& component)
    {

        /*
        //Loads the default cube object, on creation.

        //Loads the default vertex and fragment shaders into the ModelComponent, on being added.
        component.vertPath = "Resources/Shaders/defalutVertex.glsl";
        component.fragPath = "Resources/Shaders/defalutFragment.glsl";
        component.EntityShader = component.LoadModelShader();

        //Loads the default texture, and passes it's value over to the shader
        component.texPath = "Resources/Maps/Textures/Debugempty.png";
        component.EntityTex = component.LoadModelTexture();
        component.EntityShader.setInteger("testTex", component.EntityTex.ID);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = {1.0f};
        glm::mat4 view = {1.0f};

        component.EntityShader.setMatrix4("model", model);
        component.EntityShader.setMatrix4("projection", projection);
        component.EntityShader.setMatrix4("view", view);

        */
    }

    template<>
    void Scene::OnComponentAdded<MapDataComponent>(Entity entity, MapDataComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {

    }

    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {

    }





}
