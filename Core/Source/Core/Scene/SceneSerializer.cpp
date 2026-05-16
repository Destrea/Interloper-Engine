
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
#include "Core/Application.h"
#include "Core/InputManager.h"
#include "Core/ResourceManager.h"

#include <yaml-cpp/yaml.h>
#include <fstream>


namespace YAML
{
    //Encode and decode functions for glm::vec3
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            //Escape if there aren't enough values for a vec3
            if(!node.IsSequence() || node.size() != 3)
               return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }


    };

    //Encode and decode functions for glm::vec4
    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            //Escape if there aren't enough values for a vec4
            if(!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

}




namespace Core
{
    //Vector3 operator
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }

    //Vector4 operator
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }



    SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene)
    {

    }

    static void SerializeEntity(YAML::Emitter& out, Entity entity)
    {
        out << YAML::BeginMap;  //Entity
        out << YAML::Key << "Entity";
        out << YAML::Value << "1234567890"; //TODO: Entity UUID goes here


        //Tag Component Serialization
        if(entity.HasComponent<TagComponent>())
        {
            out << YAML::Key << "TagComponent";
            out << YAML::BeginMap;  //TagComponent

            auto& tag = entity.GetComponent<TagComponent>().Tag;
            out << YAML::Key << "Tag";
            out << YAML::Value << tag;

            out << YAML::EndMap;    //TagComponent
        }

        //Transform Component Serialization
        if(entity.HasComponent<TransformComponent>())
        {
            out << YAML::Key << "TransformComponent";
            out << YAML::BeginMap;  //TransformComponent

            auto& tc = entity.GetComponent<TransformComponent>();
            out << YAML::Key << "Translation";
            out << YAML::Value << tc.Translation;
            out << YAML::Key << "Rotation";
            out << YAML::Value << tc.Rotation;
            out << YAML::Key << "Scale";
            out << YAML::Value << tc.Scale;

            out << YAML::EndMap;    //TransformComponent
        }

        if(entity.HasComponent<ModelComponent>())
        {
            out << YAML::Key << "ModelComponent";
            out << YAML::BeginMap;  //ModelComponent

            //Serialize all fields of model
            //Serialize the path for the chosen shader file
            //Serialize any relevant texture data, that would be tied to the shader file

            auto& mc = entity.GetComponent<ModelComponent>();
            out << YAML::Key << "ModelPath";
            out << YAML::Value << mc.modelPath;

            out << YAML::Key << "ShaderName";
            out << YAML::Value << mc.ShaderName;
            out << YAML::Key << "fragPath";
            out << YAML::Value << mc.fragPath;
            out << YAML::Key << "vertPath";
            out << YAML::Value << mc.vertPath;

            out << YAML::Key << "TexName";
            out << YAML::Value << mc.TexName;
            out << YAML::Key << "texPath";
            out << YAML::Value << mc.texPath;


            out << YAML::EndMap;    //ModelComponent
        }

        if(entity.HasComponent<MapDataComponent>())
        {

            //TODO: Decide if this component will stay, since we can serialize scenes now.
            out << YAML::Key << "MapDataComponent";
            out << YAML::BeginMap;  //MapDataComponent

            out << YAML::EndMap;    //MapDataComponent
        }

        if(entity.HasComponent<CameraComponent>())
        {
            out << YAML::Key << "CameraComponent";
            out << YAML::BeginMap;  //CameraComponent

            out << YAML::EndMap;    //CameraComponent
        }

        if(entity.HasComponent<NativeScriptComponent>())
        {

            //TODO: No idea how to serialize this, if needed at all.
            out << YAML::Key << "NativeScriptComponent";
            out << YAML::BeginMap;  //NativeScriptComponent

            out << YAML::EndMap;    //NativeScriptComponent
        }




        out << YAML::EndMap; //Entity
    }

    //Serializes the scene to a text file, for readability
    void SceneSerializer::Serialize(const std::string& path)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << "Untitled";


        //Entities are serialized with YAML as a sequence
        out << YAML::Key << "Entities";
        out << YAML::Value << YAML::BeginSeq;

        for(auto entity: m_Scene->m_Registry.view<entt::entity>())
        {
            Entity curEntity = {entity, m_Scene.get()};
            if(!curEntity)
                return;

            SerializeEntity(out, curEntity);
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(path);
        fout << out.c_str();

    }

    //Serializes the scene to a binary file, for runtime use
    void SceneSerializer::SerializeRuntime(const std::string& path)
    {

        printf("Scene Runtime Serializer Not implemented yet!");
    }

    //Deserializes the scene from a text file, for readability
    bool SceneSerializer::Deserialize(const std::string& path)
    {

        std::ifstream stream(path);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());

        //Escapes if the file being deserialized doesn't contain a scene.
        if(!data["Scene"])
            return false;

        std::string sceneName = data["Scene"].as<std::string>();
        printf("Deserializing scene: %s", sceneName.c_str());


        auto entities = data["Entities"];
        if(entities)
        {
            //Loops through each entity that is found in the text file, and checks if it contains an entry for each possible component, deserializing as true.
            for( auto entity : entities)
            {
                uint64_t uuid = entity["Entity"].as<uint64_t>(); // Placeholder for Entity uuid system

                std::string name;
                auto tagComponent = entity["TagComponent"];
                if(tagComponent)
                    name = tagComponent["Tag"].as<std::string>();
                printf("TagComponent done\n");
                //Create our new entity with the deserialized name
                Entity deserializedEntity = m_Scene->CreateEntity(name);

                //Deserialize components, add them to the created entity, and fill with deserialized data.
                auto transformComponent = entity["TransformComponent"];
                if(transformComponent)
                {
                    //This should always be true, because entities have transforms upon creation, as such we Get the component instead of Add.
                    auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                    tc.Translation = transformComponent["Translation"].as<glm::vec3>();
                    tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
                    tc.Scale = transformComponent["Scale"].as<glm::vec3>();
                    printf("TransformComponent done\n");
                }


                auto modelComponent = entity["ModelComponent"];
                if(modelComponent)
                {
                    //Gets the model's path, adds the model component, and loads the model.
                    std::string modelPath = modelComponent["ModelPath"].as<std::string>();
                    auto& mc = deserializedEntity.AddComponent<ModelComponent>(modelPath);
                    mc.modelPath = modelPath;

                    //Shader and Texture deserializing
                    mc.ShaderName = modelComponent["ShaderName"].as<std::string>();
                    mc.fragPath = modelComponent["fragPath"].as<std::string>();
                    mc.vertPath = modelComponent["vertPath"].as<std::string>();

                    mc.TexName = modelComponent["TexName"].as<std::string>();
                    mc.texPath = modelComponent["texPath"].as<std::string>();

                    //Loads the deserialized shader and texture, so that the model displays correctly.

                    Core::ResourceManager::LoadTexture(mc.texPath.c_str(), false, mc.TexName);
                    Core::ResourceManager::LoadShader(mc.vertPath.c_str(), mc.fragPath.c_str(), mc.ShaderName);
                    Renderer::Texture2D EntityTexture = Core::ResourceManager::GetTexture(mc.TexName);
                    printf("TexID = %d", EntityTexture.ID);

                    printf("ModelComponent done\n");
                }

                auto mapDataComponent = entity["MapDataComponent"];
                if(mapDataComponent)
                {
                    //auto& mdc = deserializedEntity.AddComponent<MapDataComponent>();
                    printf("MapDataComponent done\n");
                }

                auto cameraComponent = entity["CameraComponent"];
                if(cameraComponent)
                {
                    //TODO: Initialize camera with decoded data.
                    auto& tc = deserializedEntity.GetComponent<TransformComponent>();
                    auto& cc = deserializedEntity.AddComponent<CameraComponent>(tc.Translation, tc.Rotation, tc.Scale);
                    cc.p_Camera.SetAspectRatio(1920, 1080);
                    printf("CameraComponent done\n");
                }

                auto nativeScriptComponent = entity["NativeScriptComponent"];
                if(nativeScriptComponent)
                {
                    //TODO: Initialize nsc with decoded data.


                    class CameraController : public ScriptableEntity
                    {
                    public:
                        void OnCreate()
                        {

                        }

                        void OnDestroy()
                        {

                        }

                        void OnUpdate(float ts)
                        {
                            GLFWwindow* window = Core::Application::Get().GetWindow()->GetHandle();
                            auto& tc = this->GetComponent<TransformComponent>();
                            auto& cc = this->GetComponent<CameraComponent>();


                            if(Input::IsKeyPressed(GLFW_KEY_ESCAPE))
                            {
                                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                                cc.p_Camera.SetCameraLock(false);
                            }

                            if(Input::IsKeyPressed(GLFW_KEY_O))
                            {
                                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                                cc.p_Camera.SetCameraLock(true);
                            }

                            //Mouse and Keyboard camera input handling
                            glm::vec2 mousePos = Input::GetMousePosition();


                            float speed = 7.0f;

                            glm::vec3 Front = cc.p_Camera.GetCameraFront();
                            glm::vec3 Right = cc.p_Camera.GetCameraRight();
                            glm::vec3 Up = cc.p_Camera.GetCameraUp();


                            if (Input::IsKeyPressed(GLFW_KEY_W))
                                tc.Translation += Front * speed * ts;
                            if (Input::IsKeyPressed(GLFW_KEY_S))
                                tc.Translation -= Front * speed * ts;
                            if (Input::IsKeyPressed(GLFW_KEY_A))
                                tc.Translation -= Right * speed * ts;
                            if (Input::IsKeyPressed(GLFW_KEY_D))
                                tc.Translation += Right * speed * ts;
                            tc.Translation.y = 3.0f;

                            cc.p_Camera.SetPosition(tc.Translation);

                            cc.p_Camera.ProcessMouseMovement(mousePos.x, mousePos.y);

                        }
                    };

                    deserializedEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
                    printf("NativeScriptComponent done\n");
                }

            }
        }


        return true;
    }

    //Deserializes the scene from a binary file, for runtime use
    bool SceneSerializer::DeserializeRuntime(const std::string& path)
    {

        printf("Scene Runtime Deserializer Not implemented yet!");
        return false;
    }

}


