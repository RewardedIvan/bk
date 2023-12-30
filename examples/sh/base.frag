#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D tex;
uniform vec2 uresolution;
uniform float utime;

void main() {
	FragColor = texture(tex, vTexCoord);
}