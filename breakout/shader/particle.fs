#version 330 core

in vec2 Texcoords;
in vec4 ParticleColor;
out vec4 color;

uniform sampler2D image;

void main() {
	color = ParticleColor * texture(image, Texcoords);
}