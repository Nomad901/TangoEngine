#version 430 core

layout (quads, fractional_odd_spacing, ccw) in;

uniform sampler2D uHeightMap;
uniform mat4 uMVP;
uniform mat4 uView;
uniform mat4 uModel;

in vec2 TextureCoord[];
in vec3 WorldPos[];
in float Height[];

out float height;
out vec3 normals;
out vec3 worldPos;

vec3 calculateNormals(vec2 pTexCoord, float pTexelSize)
{
    float hL = texture(uHeightMap, pTexCoord + vec2(-pTexelSize, 0.0f)).r;
    float hR = texture(uHeightMap, pTexCoord + vec2( pTexelSize, 0.0f)).r;
    float hD = texture(uHeightMap, pTexCoord + vec2( 0.0f, -pTexelSize)).r;
    float hU = texture(uHeightMap, pTexCoord + vec2( 0.0f,  pTexelSize)).r;
   
    vec3 normals = normalize(vec3(hL - hR, 2.0f * pTexelSize, hD - hU));
    return normals;
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = TextureCoord[0]; 
    vec2 t01 = TextureCoord[1]; 
    vec2 t10 = TextureCoord[2]; 
    vec2 t11 = TextureCoord[3]; 
    
    vec2 t0 = mix(t00, t01, u);
    vec2 t1 = mix(t10, t11, u);
    vec2 texCoord = mix(t0, t1, v);

    vec3 wp00 = WorldPos[0];
    vec3 wp01 = WorldPos[1];
    vec3 wp10 = WorldPos[2];
    vec3 wp11 = WorldPos[3];

    vec3 wp0 = mix(wp00, wp01, u);
    vec3 wp1 = mix(wp10, wp11, u);
    worldPos = mix(wp0, wp1, v);

    vec4 p00 = gl_in[0].gl_Position;  
    vec4 p01 = gl_in[1].gl_Position; 
    vec4 p10 = gl_in[2].gl_Position; 
    vec4 p11 = gl_in[3].gl_Position;  

    vec4 p0 = mix(p00, p01, u);
    vec4 p1 = mix(p10, p11, u);
    vec4 p = mix(p0, p1, v);

    float h00 = Height[0];
    float h01 = Height[1];
    float h10 = Height[2];
    float h11 = Height[3];
    
    float h0 = mix(h00, h01, u);
    float h1 = mix(h10, h11, u);
    height = mix(h0, h1, v);

    float texelSize = 1.0f / textureSize(uHeightMap, 0).x;
    normals = calculateNormals(texCoord, texelSize);

    p.y += height;
    vec4 worldPos = uModel * p;
    gl_Position = uMVP * worldPos;
    
    normals = mat3(uModel) * normals;
}