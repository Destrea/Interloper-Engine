#pragma once

#include <filesystem>
#include <glm/glm.hpp>
namespace Renderer {

    class Shader
    {
    public:

        //Shader State
        unsigned int ID;
        bool errors = false;
        //Shader Constructor
        Shader() {}
        // sets the current shader as active
        Shader &Use();
        // Compiles the shader from the given source code
        void Compile(const char *vetexSource, const char *fragmentSource);
        //Utility functions
        void setFloat   (const char *name, float value, bool useShader = false);
        void setInteger (const char *name, int value, bool useShader = false);
        void setVector2f(const char *name, float x, float y, bool useShader = false);
        void setVector2f(const char *name, const glm::vec2 &value, bool useShader = false);
        void setVector3f(const char *name, float x, float y, float z, bool useShader = false);
        void setVector3f(const char *name, const glm::vec3 &value, bool useShader = false);
        void setVector4f(const char *name, float x, float y, float z, float w, bool useShader = false);
        void setVector4f(const char *name, const glm::vec4 &value, bool useShader = false);
        void setMatrix4 (const char *name, const glm::mat4 &matrix, bool useShader = false);

        uint32_t CreateComputeShader(const std::filesystem::path& path);
        uint32_t ReloadComputeShader(uint32_t shaderHandle, const std::filesystem::path& path);

        uint32_t CreateGraphicsShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
        uint32_t ReloadGraphicsShader(uint32_t shaderHandle, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);

    private:
        // compile and linkage error reporting
        void checkCompileErrors(unsigned int object, std::string type);
    };

}
