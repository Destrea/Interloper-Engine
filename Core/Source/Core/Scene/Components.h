#pragma once

#include <glm/glm.hpp>
#include "Core/Renderer/Model.h"
#include "Core/Renderer/Camera.h"
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
        ModelComponent(const Renderer::Model model) : EntityModel(model) {}
    };

    struct CameraComponent
    {
        Camera p_Camera;
        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const glm::vec3& position) : p_Camera(position) {}
    };

    struct CollisionComponent
    {

    };


}
