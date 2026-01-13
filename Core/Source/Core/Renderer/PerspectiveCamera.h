#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#if OLD
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//Camera values
const float YAW         = -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   7.0f;
const float SENSITIVITY =   0.1f;
const float ZOOM        =  75.0f;

namespace Renderer
{
    class PerspectiveCamera
    {
    public:
        //Camera attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        //Euler Angles
        float Yaw;
        float Pitch;

        //Camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        //Constructor with vector inputs
        PerspectiveCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }
        PerspectiveCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        const glm::vec3& GetPosition() const {return m_Position; }
        void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix();}

        float GetRotation() const {return m_Rotation; }
        void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix();}    //This scenario of rotation is ONLY used for 2D rendering rotation. Re-work this afterwards to include rotation along all 3 axes.

        const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const glm::mat4& GetViewMatrix() const {return m_ProjectionMatrix; }
        const glm::mat4& GetViewProjectionMatrix() const {return m_ViewProjectionMatrix; }

        void ProcessKeyboard(Camera_Movement direction, float deltatime);
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    private:
        void RecalculateViewMatrix() { m_ViewMatrix = glm::lookAt(Position, Position + Front, Up); }
        void updateCameraVectors()
        {
            //Calculate the new front vector
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);

            //Also re-calculate Right and Up
            Right = glm::normalize(glm::cross(Front, WorldUp));
            Up = glm::normalize(glm::cross(Right, Front));

        }

    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;       //Cache the View Proj Matrix, to avoid doing the operation multiple times;

        glm::vec3 m_Position;
        float m_Rotation = 0.0f;

    };

}

#endif



//Defines possible options for camera movement. Allows for abstraction from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//Camera values
const float YAW         = -90.0f;
const float PITCH       =   0.0f;
const float SPEED       =   7.0f;
const float SENSITIVITY =   0.1f;
const float ZOOM        =  75.0f;

namespace Renderer
{
    class PerspectiveCamera
    {
    public:
        //Camera attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        //Euler Angles
        float Yaw;
        float Pitch;

        //Camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        //Constructor with vector inputs
        PerspectiveCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = position;
            WorldUp = up;
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();
        }

        //Constructor with scalar value inputs
        PerspectiveCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {
            Position = glm::vec3(posX, posY, posZ);
            WorldUp = glm::vec3(upX, upY, upZ);
            Yaw = yaw;
            Pitch = pitch;
            updateCameraVectors();

        }

        //returns the view matrix, calculated using euler angles and the lookAt matrix
        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(Position, Position + Front, Up);
        }


        // Processes input recieved from any keyboard-style input system.
        void ProcessKeyboard(Camera_Movement direction, float deltaTime)
        {
            float velocity = MovementSpeed * deltaTime;
            if(direction == FORWARD)
                Position += Front * velocity;
            if(direction == BACKWARD)
                Position -= Front * velocity;
            if(direction == LEFT)
                Position -= Right * velocity;
            if(direction == RIGHT)
                Position += Right * velocity;
            Position.y = 0.0f;
        }

        //Movement processing and calculation
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            Yaw += xoffset;
            Pitch += yoffset;


            if(constrainPitch)
            {
                if(Pitch > 89.0f)
                    Pitch = 89.0f;
                if(Pitch < -89.0f)
                    Pitch = -89.0f;
            }

            updateCameraVectors();
        }

        //Zoom functionality
        void ProcessMouseScroll(float yoffset)
        {
            Zoom -= (float)yoffset;
            if(Zoom < 1.0f)
                Zoom = 1.0f;
            if(Zoom > 45.0f)
                Zoom = 45.0f;
        }

    private:

        //Updates the Right and Up camera vectors from the Camera's updated euler angles
        void updateCameraVectors()
        {
            //Calculate the new front vector
            glm::vec3 front;
            front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            front.y = sin(glm::radians(Pitch));
            front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
            Front = glm::normalize(front);

            //Also re-calculate Right and Up
            Right = glm::normalize(glm::cross(Front, WorldUp));
            Up = glm::normalize(glm::cross(Right, Front));
        }

    };

}


