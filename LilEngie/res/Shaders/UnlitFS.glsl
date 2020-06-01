#version 420 core

layout(std140, binding = 3) uniform Material 
{
	vec3 color;
};

out vec4 col;

void main()
{
	col = vec4(color, 1);
}
