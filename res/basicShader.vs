#version 420

in vec3 position;
in vec2 texCoord;
in vec3 normal;


out vec2 texCoord0;
out vec3 normal0;

uniform mat4 MVP;
uniform mat4 Normal;

void main()
{
	gl_Position = MVP*vec4(position, 1.0);
	texCoord0 = texCoord;
	normal0 = normal;//(Normal*vec4(normal,0.0)).xyz;
}
