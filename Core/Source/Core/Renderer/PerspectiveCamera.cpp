#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Renderer
{
    /*
    PerspectiveCamera::PerspectiveCamera(glm::mat4 ProjectionMatrixIN) : m_ProjectionMatrix(ProjectionMatrixIN), m_ViewMatrix(1.0f)
    {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
    */

    PerspectiveCamera::PerspectiveCamera()
    {
        m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);

    }

    PerspectiveCamera::PerspectiveCamera(glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
    {
        SetTransform(translation, rotation, scale);
        m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
    }



    void PerspectiveCamera::SetProjection(glm::mat4 ProjectionMatrixIN)
    {
        m_ProjectionMatrix = ProjectionMatrixIN;
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void PerspectiveCamera::RecalculateProjectionMatrix()
    {
         m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
    }

    void PerspectiveCamera::SetAspectRatio(uint32_t width, uint32_t height)
    {
        m_AspectRatio = (float)width / (float)height;
        RecalculateProjectionMatrix();
    }

    void PerspectiveCamera::SetTransform(glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
    {
        m_Position = translation;
        m_Rotation = rotation;
        m_Scale = scale;

        RecalculateViewMatrix();
    }


    void PerspectiveCamera::RecalculateViewMatrix()
    {
        //Rotation calculation, for the transform mat4
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_Rotation.x, {1,0,0})
        * glm::rotate(glm::mat4(1.0f), m_Rotation.y, {0,1,0})
        * glm::rotate(glm::mat4(1.0f), m_Rotation.z, {0,0,1});

        //Multiplying together the translation, rotation and scale components, and returning it as our final Transform.
        glm::mat4 transform =  glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);


        //Transform is then inverted to give us the camera's view matrix
        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }


}


