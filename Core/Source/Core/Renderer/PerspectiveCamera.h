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
        const glm::mat4& GetViewProjectionMatrix() const {return m_ViewProjectionMatrix; }


        //TODO: Add a class function, either for moving the camera, or for returning the directional vectors, so that other classes and functions can use them.

        //TODO: Update/remove the keyboard and mouse controls from InputManager, as needed. Clean it all up so that all of the mouse/keyboard input can be "centalized", to make it easy to keep up with what/who/where things are being controlled from.



    private:
        void RecalculateViewMatrix();

    private:
        //Matrices
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        //Transform vector 3 components
        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_Rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_Scale    = {1.0f, 1.0f, 1.0f};

        //Camera Directional Vectors
        glm::vec3 m_CameraFront;
        glm::vec3 m_CameraRight;
        glm::vec3 m_CameraUp;
        glm::vec3 m_WorldUp = {0.0f, 1.0f, 0.0f};

        //Clipping distances, near and far
        float m_PerspectiveNear = 0.01f;
        float m_PerspectiveFar = 1000.0f;

        //45 degree FOV default
        float m_PerspectiveFOV = 45.0f;
        float m_AspectRatio = 0.0f;

    };

}






#if _OLD

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
const float SENSITIVITY =   0.01f;
const float ZOOM        =  75.0f;

namespace Renderer
{
    class PerspectiveCamera
    {
    public:
        //Camera attributes
        glm::vec3& Position;
        glm::vec3& LocalRotation;
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
        /*
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
         */

        PerspectiveCamera(glm::vec3 &Translation, glm::vec3 &Rotation, glm::vec3 up = glm::vec3(0.0f,1.0f, 0.0f)) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),  Position(Translation), LocalRotation(Rotation), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
        {

            //Position = Translation
            //Position = Translation;
            //LocalRotation = Rotation;
            //Yaw = Rotation.x
            //Pitch = Rotation.y
            //Yaw = LocalRotation.x;
            //Pitch = LocalRotation.y;
            WorldUp = up;
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


            /*
            if(direction == FORWARD)
                transform[3][1] += velocity;
            if(direction == BACKWARD)
                transform[3][1] -= velocity;
            if(direction == LEFT)
                transform[3][0] -= velocity;
            if(direction == RIGHT)
                transform[3][0] += velocity;
            //Position.y = 0.0f;

            */




        }

        //Movement processing and calculation
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            LocalRotation = glm::degrees(LocalRotation);
            xoffset *= MouseSensitivity;
            yoffset *= MouseSensitivity;

            LocalRotation.x += xoffset;
            LocalRotation.y += yoffset;

            if(constrainPitch)
            {
                if(LocalRotation.y > 89.0f)
                    LocalRotation.y = 89.0f;
                if(LocalRotation.y < -89.0f)
                    LocalRotation.y = -89.0f;
            }
            LocalRotation = glm::radians(LocalRotation);

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



        //Updates the Right and Up camera vectors from the Camera's updated euler angles
        void updateCameraVectors()
        {
            LocalRotation = glm::degrees(LocalRotation);
            //Calculate the new front vector
            glm::vec3 front;
            front.x = cos(glm::radians(LocalRotation.x)) * cos(glm::radians(LocalRotation.y));
            front.y = sin(glm::radians(LocalRotation.y));
            front.z = sin(glm::radians(LocalRotation.x)) * cos(glm::radians(LocalRotation.y));


            Front = glm::normalize(front);

            //Also re-calculate Right and Up
            Right = glm::normalize(glm::cross(Front, WorldUp));
            Up = glm::normalize(glm::cross(Right, Front));

            LocalRotation = glm::radians(LocalRotation);

        }
    private:

    };

}


#endif


