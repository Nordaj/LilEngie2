#version 420 core
layout (location = 0) in vec4 position;
//layout (location = 1) in vec4 normal;
//layout (location = 2) in vec4 tangent;
layout (location = 3) in vec4 texcoord;

layout(std140, binding = 1) uniform PerCamera
{
    mat4 vp;
};

layout(std140, binding = 2) uniform PerObject
{
	mat4 model;
};

out vec2 uv;

void main()
{
    uv = texcoord.xy;
    gl_Position = (vp * model) * position;
}
