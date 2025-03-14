#version 460 core

out vec3 WorldPos;

uniform vec3 cameraPos;

uniform mat4 view;
uniform mat4 projection;

uniform float gridSize = 100.0;

const vec3 Pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),
    vec3( 1.0, 0.0, -1.0),
    vec3( 1.0, 0.0,  1.0),
    vec3(-1.0, 0.0,  1.0)
);

const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

void main()
{
    int Index = Indices[gl_VertexID];
    vec3 vPos = Pos[Index] * gridSize;

    vPos.x += cameraPos.x;
    vPos.z += cameraPos.z;

    gl_Position = projection * view * vec4(vPos, 1.0);

    WorldPos = vPos;
}

