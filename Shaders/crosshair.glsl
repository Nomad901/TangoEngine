#version 430

in vec2 fragPos;
in vec2 fragPosTex;
in vec4 fragColor;

out vec4 finalColor;

uniform vec2 uResolution;

void main()
{
    vec2 uv = fragPosTex * 2.0f - 1.0f; 
    float aspect = uResolution.x / uResolution.y;
    uv.x *= aspect;
       
    float fade = 0.05f;
    float distance = 1.0f - length(uv);
    distance = smoothstep(0.0f, fade, distance);
 
    if(distance == 0)
        discard;
    finalColor = vec4(distance) * fragColor;
}