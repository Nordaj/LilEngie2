#version 420 core

layout(std140, binding = 3) uniform Material 
{
	vec3 color;
};

in vec2 uv;

out vec4 col;

//use texture to sample
uniform sampler2D tex;

void main()
{
	col = vec4(color, 1);
	col = vec4(texture(tex, uv).rgb, 1.);
}
