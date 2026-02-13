#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D testTex;

void main()
{
    FragColor = texture(testTex, TexCoords);
}
