#version 460

out vec3 WorldPos;

uniform mat4 view;
uniform mat4 projection;

uniform float gridSize = 100.0;
uniform vec3 cameraPos;

const vec3 Pos[4] = vec3[4](
    vec3(-1.0, -0.05, -1.0),      // bottom left
    vec3( 1.0, -0.05, -1.0),      // bottom right
    vec3( 1.0, -0.05,  1.0),      // top right
    vec3(-1.0, -0.05,  1.0)       // top left
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