#version 330 core
layout (location = 0) in vec4 vertex;
//[position, texcoords]

out vec2 Texcoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform vec4 color;
uniform vec2 offset;

void main() {
    float scale = 10.f;
    ParticleColor = color;
	Texcoords = vertex.zw;
	gl_Position = projection * vec4((vertex.xy * scale + offset), 0.f, 1.f);
}