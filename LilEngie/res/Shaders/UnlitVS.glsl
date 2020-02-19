#version 420 core
layout (location = 0) in vec3 pos;
layout (location = 3) in vec2 uv;

layout(std140, binding = 1) uniform Model
{
	mat4 model;
};

void main()
{
    //mat4 m = mat4(1);
	gl_Position = model * vec4(pos, 1);
}
