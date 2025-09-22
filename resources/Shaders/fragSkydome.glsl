#version 440

in vec2 TexCoords;
out vec4 outColor;

uniform sampler2D uTexture;

void main()
{
    outColor = texture(uTexture, TexCoords);
}