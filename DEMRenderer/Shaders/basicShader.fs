 #version 330 core

out vec3 color;

in vec3 normal0;

uniform vec3 lightDirection;

void main() {
	color = vec3(1,0,0) * clamp(dot(-lightDirection, normal0), 0.0, 1.0);
}