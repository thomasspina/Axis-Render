#version 460

out vec3 WorldPos;
flat out int isYAxis;

uniform mat4 view;
uniform mat4 projection;

uniform float gridSize = 1000.0;
uniform vec3 cameraPos;

const vec3 gridPos[4] = vec3[4](
    vec3(-1.0, -0.005, -1.0),      // bottom left
    vec3( 1.0, -0.005, -1.0),      // bottom right
    vec3( 1.0, -0.005,  1.0),      // top right
    vec3(-1.0, -0.005,  1.0)       // top left
);

const int gridIndices[6] = int[6](0, 2, 1, 2, 0, 3);


const vec3 yAxisVertices[2] = vec3[2](
    vec3(0.0, -1.0, 0.0),
    vec3(0.0, 1.0, 0.0)
);



void main()
{   
    vec3 vPos;

    if (gl_VertexID < 6) { 
        int Index = gridIndices[gl_VertexID];
        vPos = gridPos[Index] * gridSize;
        isYAxis = 0;
        
        vPos.x += cameraPos.x;
        vPos.z += cameraPos.z;
    } else { 
        vPos = yAxisVertices[gl_VertexID - 6] * gridSize;
        isYAxis = 1;
    }

    WorldPos = vPos;
    gl_Position = projection * view * vec4(vPos, 1.0);
}