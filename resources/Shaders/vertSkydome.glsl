#version 440

layout(location = 0) in vec3 aPos;
layout(location = 4) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 uMVP;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = uMVP * vec4(aPos, 1.0);
    gl_Position.z = gl_Position.w;  
}