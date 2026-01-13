#pragma once



#pragma once

#include <glm/glm.hpp>
#include "Core/Renderer/Model.h"
#include "Core/Renderer/PerspectiveCamera.h"
#include "Core/Renderer/Camera.h"
#include "ScriptableEntity.h"
#include "Core/MapLoader.h"
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
        glm::mat4 Transform;

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4 transform) : Transform(transform) {}

        //Operator for the component, to "implicitly cast" the data of this component to be used as a certain type,
        //instead of having to specify what "part" of it, since the "TransformComponent" is basically just a wrapper for the mat4 "Transform" data.
        operator const glm::mat4&() const {return Transform;}
    };


    struct ModelComponent
    {

        //Fix this all so that it actually loads the model specified
        Renderer::Model EntityModel;

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
        Renderer::PerspectiveCamera p_Camera;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const glm::vec3& position) : p_Camera(position) {}


        //Renderer::Camera Camera;

        /*OrthographicCamera p_Camera;
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const glm::mat4& projection) : Camera(projection) {}
        */
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
