#version 420

in vec2 texCoord0;
in vec3 worldPos0;
in vec3 normal0;
in mat3 tbnMatrix;

out vec4 color;

uniform sampler2D diffuse;
uniform vec4 MaterialAmbientColor;
uniform sampler2D normalMap;
uniform sampler2D dispMap;
uniform float dispMapScale;
uniform float dispMapBias;
uniform vec3 eyePos;

void main()
{
	vec3 directionToEye = normalize(eyePos - worldPos0);
	vec2 texcoords = texCoord0.xy + (directionToEye*tbnMatrix).xy*(texture2D(dispMap , texCoord0.xy).r*
			dispMapScale + dispMapBias);
	color = texture2D(diffuse, texcoords)*(MaterialAmbientColor);	
}
