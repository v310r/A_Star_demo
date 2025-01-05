#version 460 core

uniform vec4 color;

out vec4 FragmentColor;

void main()
{
	FragmentColor = vec4(color);
}