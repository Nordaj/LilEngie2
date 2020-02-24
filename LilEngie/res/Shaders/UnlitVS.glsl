#version 420 core
layout (location = 0) in vec4 position;
//layout (location = 1) in vec4 normal;
//layout (location = 2) in vec4 tangent;
//layout (location = 3) in vec4 texcoord;

layout(std140, binding = 1) uniform Model
{
	mat4 model;
};

void main()
{
    //mat4 m = mat4(1);
	//gl_Position = model * pos;
    gl_Position = model * position;
}
