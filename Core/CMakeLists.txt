#
# Core
#

set(SOURCES
#stb_image
vendor/stb/stb_image.h
vendor/stb/stb_image.cpp

#glad
vendor/glad/include/glad/glad.h
vendor/glad/src/glad.c
vendor/glad/include/KHR/khrplatform.h

#entt
vendor/entt/include/entt.hpp

#imgui Includes
vendor/ImGui/imconfig.h
vendor/ImGui/imgui.cpp
vendor/ImGui/imgui.h
vendor/ImGui/imgui_demo.cpp
vendor/ImGui/imgui_draw.cpp
vendor/ImGui/imgui_impl_glfw.cpp
vendor/ImGui/imgui_impl_glfw.h
vendor/ImGui/imgui_impl_opengl3.cpp
vendor/ImGui/imgui_impl_opengl3.h
vendor/ImGui/imgui_internal.h
vendor/ImGui/imgui_tables.cpp
vendor/ImGui/imgui_widgets.cpp
vendor/ImGui/imstb_rectpack.h
vendor/ImGui/imstb_textedit.h
vendor/ImGui/imstb_truetype.h

#application and engine
Source/Core/Application.h
Source/Core/Application.cpp
Source/Core/Window.h
Source/Core/Window.cpp
Source/Core/Layer.h
Source/Core/Event.h
Source/Core/WindowEvents.h
Source/Core/InputEvents.h
Source/Core/InputManager.cpp
Source/Core/InputManager.h
Source/Core/ResourceManager.h
Source/Core/ResourceManager.cpp
Source/Core/EditorLayer.h
Source/Core/EditorLayer.cpp

Source/Core/Renderer/Renderer.h
Source/Core/Renderer/Renderer.cpp
Source/Core/Renderer/Shader.h
Source/Core/Renderer/Shader.cpp
Source/Core/Renderer/Texture.h
Source/Core/Renderer/Texture.cpp
Source/Core/Renderer/Camera.h
Source/Core/Renderer/Model.h
Source/Core/Renderer/Mesh.h
Source/Core/Renderer/Framebuffer.h
Source/Core/Renderer/Framebuffer.cpp

Source/Core/Scene/Scene.h
Source/Core/Scene/Scene.cpp
Source/Core/Scene/Entity.h
Source/Core/Scene/Entity.cpp
Source/Core/Scene/Components.h
)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(Core glfw)
target_link_libraries(Core glm)
target_link_libraries(Core assimp)

target_include_directories(Core PUBLIC "Source" "vendor/stb" "vendor/glad/include" "vendor/ImGui" "vendor/entt/include")

