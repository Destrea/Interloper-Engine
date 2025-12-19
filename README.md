# Interloper Game Engine
This is my custom C++ and OpenGL game engine.
This engine was made for my own personal learning and growth as a programmer. Leveraging my interest in Graphics APIs and my knowledge foundation of C and C++ I decided that I'd like to learn how game engines work at a foundational level. I decided this because I've found that I learn better and enjoy creating things when I'm forced to solve problems, and I figured I could also create some cool features of my own along the way.

As this is my first foray into Engine development, I don't expect this to be the "pinnacle" of organization, nor will it be the most optimized and feature-rich engine. That said, while it's developed on Linux, it's built and tested to work on both Linux and Windows, and will be going forward.


## Libraries, Frameworks and Tools used: <br>
GLFW 3.4: https://www.glfw.org <br>
GLAD 1.0.1: https://glad.dav1d.de <br>
Assimp: https://www.assimp.org <br>
stb_image: https://github.com/nothings/stb <br>
Dear ImGui: https://github.com/ocornut/imgui <br>
Cmake: https://cmake.org <br>
EnTT: https://github.com/skypjack/entt <br>

## References & Resources used: <br>
LearnOpenGL: https://learnopengl.com <br>
The Cherno's GameEngine Series: https://www.youtube.com/@TheCherno <br>


# Build Instructions
<pre>
  mkdir build
  cd build
  cmake ..
  make
</pre>

On Windows, you can additionally open the CMakeLists.txt file in the root directory as a project, and build it with Visual Studio.
