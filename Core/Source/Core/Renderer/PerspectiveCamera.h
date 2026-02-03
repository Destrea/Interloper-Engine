#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdint.h>

//#include "Core/Scene/Components.h"



namespace Renderer
{
    class PerspectiveCamera
    {
    public:

        //Constructor
        PerspectiveCamera();

        PerspectiveCamera(glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

        void SetProjection(glm::mat4 ProjectionMatrixIN);
        void RecalculateProjectionMatrix();

        void RecalculateDirectionalVectors();

        void SetAspectRatio(uint32_t width, uint32_t height);

        const glm::vec3& GetPosition() const {return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix();}

        const glm::vec3& GetRotation() const {return m_Rotation; }
        void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

        void SetTransform(glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);


        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

        const glm::mat4& GetProjectionMatrx() const {return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        glm::mat4& GetViewMatrix2()
        {
            m_ViewMatrix2 = glm::lookAt(m_Position, m_Position + m_CameraFront, m_WorldUp);
            return m_ViewMatrix2;
        }
        const glm::mat4& GetViewProjectionMatrix() const {return m_ViewProjectionMatrix; }


        const glm::vec3& GetCameraFront() const {return m_CameraFront; }
        const glm::vec3& GetCameraRight() const {return m_CameraRight; }
        const glm::vec3& GetCameraUp() const {return m_CameraUp; }
        const glm::vec3& GetWorldUp() const {return m_WorldUp; }



        //TODO: Add a class function, either for moving the camera, or for returning the directional vectors, so that other classes and functions can use them.

        //TODO: Update/remove the keyboard and mouse controls from InputManager, as needed. Clean it all up so that all of the mouse/keyboard input can be "centalized", to make it easy to keep up with what/who/where things are being controlled from.


        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_Rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_Scale    = {1.0f, 1.0f, 1.0f};


    private:
        void RecalculateViewMatrix();

    private:
        //Matrices
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewMatrix2;
        glm::mat4 m_ViewProjectionMatrix;

        //Transform vector 3 components


        //Camera Directional Vectors
        glm::vec3 m_CameraFront;
        glm::vec3 m_CameraRight;
        glm::vec3 m_CameraUp;
        glm::vec3 m_WorldUp;

        //Clipping distances, near and far
        float m_PerspectiveNear = 0.01f;
        float m_PerspectiveFar = 1000.0f;

        //45 degree FOV default
        float m_PerspectiveFOV = 45.0f;
        float m_AspectRatio = 0.0f;

    };

}









