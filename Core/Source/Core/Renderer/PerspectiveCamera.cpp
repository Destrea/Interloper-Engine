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
        m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        RecalculateDirectionalVectors();
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
        m_Rotation.y = -90.0f;
        m_Scale = scale;

        RecalculateViewMatrix();
    }

    void PerspectiveCamera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        float MouseSensitivity = 0.10f;
        glm::vec3 LocalRotation = glm::degrees(m_Rotation);
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        //TODO: Figure out why the hell these are switched wrong.
        // Maybe because of the way I'm composing the view matrix?

        //Yaw, y axis rotation, left/right camera
        LocalRotation.y += xoffset;
        //Pitch, x axis rotation, up/down camera
        LocalRotation.x += yoffset;

        if(constrainPitch)
        {
            if(LocalRotation.x > 89.0f)
                LocalRotation.x = 89.0f;
            if(LocalRotation.x < -89.0f)
                LocalRotation.x = -89.0f;
        }
        m_Rotation = glm::radians(LocalRotation);

        RecalculateDirectionalVectors();
    }


    void PerspectiveCamera::RecalculateViewMatrix()
    {
        /*
        //Rotation calculation, for the transform mat4
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_Rotation.x, {1,0,0})
        * glm::rotate(glm::mat4(1.0f), m_Rotation.y, {0,1,0})
        * glm::rotate(glm::mat4(1.0f), m_Rotation.z, {0,0,1});

        //Multiplying together the translation, rotation and scale components, and returning it as our final Transform.
        glm::mat4 transform =  glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);


        //Transform is then inverted to give us the camera's view matrix
        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        */

         m_ViewMatrix2 = glm::lookAt(m_Position, m_Position + m_CameraFront, m_WorldUp);

    }

    void PerspectiveCamera::RecalculateDirectionalVectors()
    {
        //Temporary var for front, for calculating the directional vectors
        glm::vec3 front;
        front.x = cos(m_Rotation.y) * cos(m_Rotation.x);
        front.y = sin(m_Rotation.x);
        front.z = sin(m_Rotation.y) * cos(m_Rotation.x);

        //Recalculate the Front, Right and Up vectors, normalizing them

        //CameraFront and WorldUp entirely cancel eachother out when looking up or down, because camera front will be (0,-1,0), and up will be (0,-1,0)
        //So doing the cross product gives a vector of like (0,0,0)
        //TODO: Fix this!!

        m_CameraFront = glm::normalize(front);
        m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
        m_CameraUp = glm::normalize(glm::cross(m_CameraFront, m_CameraRight));

    }


}


