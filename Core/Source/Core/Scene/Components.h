#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Renderer/Model.h"
#include "Core/Renderer/PerspectiveCamera.h"
#include "Core/Renderer/Camera.h"
#include "ScriptableEntity.h"
#include "Core/MapLoader.h"
#include "Core/Renderer/Shader.h"

using namespace Renderer;

namespace Core
{


    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}
    };

    struct TransformComponent
    {
        //rotation will be stored as radians here, and converted into degrees for display in UI.
        glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3 translation) : Translation(translation) {}

        glm::mat4 GetTransform() const
        {
            //Rotation calculation, for the transform mat4
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1,0,0})
            * glm::rotate(glm::mat4(1.0f), Rotation.y, {0,1,0})
            * glm::rotate(glm::mat4(1.0f), Rotation.z, {0,0,1});

            //Multiplying together the translation, rotation and scale components, and returning it as our final Transform.
            return glm::translate(glm::mat4(1.0f), Translation) * rotation * glm::scale(glm::mat4(1.0f), Scale);
        }

    };

    struct ModelComponent
    {

        //Fix this all so that it actually loads the model specified
        Renderer::Model EntityModel;
        Renderer::Shader EntityShader;

        ModelComponent() = default;
        ModelComponent(const ModelComponent&) = default;
        ModelComponent(std::string const &path) : EntityModel(path) {}
    };

    struct MapDataComponent
    {
        MapData data;

        MapDataComponent() = default;
        MapDataComponent(const MapDataComponent&) = default;
        MapDataComponent(std::string const &path) : data(path) {}
    };

    struct CameraComponent
    {
        //Edit this to be more flexible to use for both "Editor" and "scene" camera types.
        //This would allow you to resize the window and still have the scene render properly, filling the window, at the correct aspect ratio.
        PerspectiveCamera p_Camera;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) : p_Camera(translation, rotation, scale) {}

    };

    struct NativeScriptComponent
    {
        //For C++ scripting of the attached entity.
        ScriptableEntity* Instance = nullptr;

        ScriptableEntity*(*InstantiateScript)();
        void (*DestroyScript)(NativeScriptComponent*);

        template<typename T>
        void Bind()
        {
            InstantiateScript = []() { return static_cast<ScriptableEntity*>( new T() ); };
            DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
        }

    };


    struct CollisionComponent
    {

    };


}
