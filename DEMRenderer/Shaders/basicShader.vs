#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;
uniform mat4 Normal;

in vec3 position;
in vec3 normal;

varying vec3 normal0;

void main() {
	gl_Position = MVP * vec4(position, 1.0);
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
}