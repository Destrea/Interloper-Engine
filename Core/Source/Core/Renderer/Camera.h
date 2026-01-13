#pragma once

#include <glm/glm.hpp>



namespace Renderer
{
    class Camera
    {
    public:
        Camera(const glm::mat4& projection): m_Projection(projection) {}

        const glm::mat4& GetProjection() const { return m_Projection; }

        //TODO: setPerspective, setOrthographic, etc
        //To allow for more camera variety, on runtime

    private:
        glm::mat4 m_Projection;
    };


}
