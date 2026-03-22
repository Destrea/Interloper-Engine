#
# Project Dependencies
#

include(FetchContent)


# OpenGL
find_package(OpenGL REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND)
# GLFW

find_package(glfw 3.4 QUIET)
if (NOT glfw3_FOUND)
    FetchContent_Declare(
        glfw3
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    )
    FetchContent_GetProperties(glfw3)
    if (NOT glfw3_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(glfw3)
        add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
    endif()
endif()



# GLM
FetchContent_Declare(
        glm
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip
)
FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(glm)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})
endif()

set_target_properties(glm PROPERTIES FOLDER "Dependencies")


# Assimp
FetchContent_Declare(
        assimp
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        URL https://github.com/assimp/assimp/archive/refs/tags/v6.0.2.zip
)
FetchContent_GetProperties(assimp)
if (NOT assimp_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(assimp)
    add_subdirectory(${assimp_SOURCE_DIR} ${assimp_BINARY_DIR})
endif()


# YAML cpp
FetchContent_Declare(
        yaml-cpp
        DOWNLOAD_EXTRACT_TIMESTAMP OFF
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
        GIT_TAG yaml-cpp-0.9.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(yaml-cpp)
    add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()





