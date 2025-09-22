#version 430 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in DATA {
    vec3 normal;
    vec3 color;
    vec2 texCoord;
    mat4 proj;
} data_in[];

in vec3 fragPos[];  // Need to pass fragPos through!

out DATA {
    vec3 normal;
    vec3 color;
    vec2 texCoord;
    mat4 proj;
} data_out;

out vec3 fragPosOut;  // Pass fragPos to fragment shader

void main() {
    for(int i = 0; i < 3; i++) {
        gl_Position = data_in[i].proj * gl_in[i].gl_Position;
        
        data_out.normal = data_in[i].normal;
        data_out.color = data_in[i].color;
        data_out.texCoord = data_in[i].texCoord;
        data_out.proj = data_in[i].proj;
        
        fragPosOut = fragPos[i];  // Pass through fragPos
        
        EmitVertex();
    }
    EndPrimitive();
}