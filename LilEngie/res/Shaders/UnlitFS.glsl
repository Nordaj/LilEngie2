#version 420 core

out vec4 col;

layout(std140, binding = 0) uniform Color
{
	vec4 color;
};

void main()
{
	col = color;
}
