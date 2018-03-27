#version 330 core

in vec2 Texcoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;

void main() {
	color = vec4(spriteColor , 1.f) * texture(image, Texcoords);
}