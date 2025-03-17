#version 460

flat in int isYAxis;
in vec3 WorldPos;

out vec4 FragColour;

uniform vec3 cameraPos;
uniform float gridSize = 1000.0;
uniform float gridCellSize = 10.0;
uniform vec4 gridColour = vec4(0.35, 0.35, 0.35, 1.0);
uniform vec4 xAxisColour = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 yAxisColour = vec4(0.0, 1.0, 0.0, 1.0);
uniform vec4 zAxisColour = vec4(0.0, 0.0, 1.0, 1.0);


vec2 satv(vec2 x)
{
    vec2 v = clamp(x, vec2(0.0), vec2(1.0));
    return v;
}


float max2(vec2 v)
{
    float f = max(v.x, v.y);
    return f;
}


void main()
{

    if (isYAxis == 1) { // draw vertical axis
        FragColour = yAxisColour;
        return;
    }

    vec2 dvx = vec2(dFdx(WorldPos.x), dFdy(WorldPos.x));
    vec2 dvy = vec2(dFdx(WorldPos.z), dFdy(WorldPos.z));

    float lx = length(dvx);
    float ly = length(dvy);

    vec2 dudv = vec2(lx, ly);

    dudv *= 4.0;

    vec2 mod_div_dudv = mod(WorldPos.xz, gridCellSize) / dudv;
    float Lod0a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)) );

    vec4 Colour = gridColour;

    float xAxisProximity = abs(WorldPos.z) / dudv.y;
    float zAxisProximity = abs(WorldPos.x) / dudv.x;

    if (xAxisProximity < 0.5) {
        Colour = xAxisColour;
    } 

    if (zAxisProximity < 0.5) {
        Colour = zAxisColour;
    }

    Colour.a *= Lod0a;
    FragColour = Colour;
}