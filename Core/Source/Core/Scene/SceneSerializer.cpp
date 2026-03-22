
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

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

            out << YAML::Key << "fragPath";
            out << YAML::Value << mc.fragPath;
            out << YAML::Key << "vertPath";
            out << YAML::Value << mc.vertPath;
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


        return false;
    }

    //Deserializes the scene from a binary file, for runtime use
    bool SceneSerializer::DeserializeRuntime(const std::string& path)
    {

        printf("Scene Runtime Deserializer Not implemented yet!");
        return false;
    }

}


