#version 420 core
layout (location = 0) in vec4 position;
//layout (location = 1) in vec4 normal;
//layout (location = 2) in vec4 tangent;
//layout (location = 3) in vec4 texcoord;

layout(std140, binding = 1) uniform PerObject
{
	mat4 model;
};

layout(std140, binding = 2) uniform PerScene
{
    mat4 vp;
};

void main()
{
    //mat4 m = mat4(1);
	//gl_Position = model * pos;
    gl_Position = (vp * model) * position;
}
