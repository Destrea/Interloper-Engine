#pragma once

#include "Scene.h"

#include "Core/Base.h"

namespace Core
{
    class SceneSerializer
    {
    public:
        SceneSerializer(const Ref<Scene>& scene);

        void Serialize(const std::string& path);            //Serializes the scene to a text file, for readability
        void SerializeRuntime(const std::string& path);     //Serializes the scene to a binary file, for runtime use

        bool Deserialize(const std::string& path);            //Deserializes the scene from a text file, for readability
        bool DeserializeRuntime(const std::string& path);     //Deserializes the scene from a binary file, for runtime use

    private:
        Ref<Scene> m_Scene;
    };

}
