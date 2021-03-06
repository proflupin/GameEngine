#version 420

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;
in vec4 shadowMapCoords0;
in vec3 T;
in vec3 B;
in vec3 N;

out vec4 color;

struct BaseLight
{
	vec3 color;
	float intensity;
};
struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

uniform sampler2D diffuse;
uniform sampler2D normalMap;
uniform sampler2D dispMap;
uniform sampler2D shadowMap;
uniform float shadowBias;
uniform float dispMapScale;
uniform float dispMapBias;
uniform vec3 eyePos;
uniform vec3 shadowTexelSize; 
uniform float specularPower;
uniform float specularIntensity;
uniform DirectionalLight directionalLight;
uniform vec4 fogColor;

const float density = 0.01f;
const float gradient = 1.5f;


vec4 calcLightDiffuse(BaseLight base , vec3 direction , vec3 normal)
{
	float diffuseFactor = dot(normalize(normal) , -normalize(direction));
	vec4 diffuseColor = vec4(0,0,0,0);
	
	if(diffuseFactor > 0)
	{
		diffuseColor = vec4(base.color,1)*base.intensity*diffuseFactor;
	}
	return diffuseColor;
}
vec4 calcLightSpec(BaseLight base , vec3 direction , vec3 normal)
{
	vec3 directionToEye = normalize(eyePos-worldPos0);
	vec3 reflectDirection = normalize(reflect(direction , normal));
	vec4 specularColor = vec4(0,0,0,0);

	float specularFactor = dot(directionToEye , reflectDirection);
	if(specularFactor > 0 )
	{
		specularFactor = pow(specularFactor , specularPower);
		specularColor = vec4(base.color , 1.0)*specularFactor*specularIntensity;
	}
	return specularColor;
}
vec4 calcLightDirectionalDiffuse(DirectionalLight dirlight , vec3 normal)
{
	return calcLightDiffuse(dirlight.base , dirlight.direction , normal);
}
vec4 calcLightDirectionalSpec(DirectionalLight dirlight , vec3 normal)
{
	return calcLightSpec(dirlight.base , dirlight.direction , normal);
}
vec2 calcParallaxTexCoords(sampler2D dMap , mat3 matrix , vec3 directionToEye , vec2 texCoords , float scale,
float bias)
{
	vec2 offset = (directionToEye * matrix).xy * (texture2D(dispMap, texCoords.xy).r * scale + bias);
	vec2 texNew = texCoords.xy;
	texNew.x += offset.x;
	texNew.y -= offset.y;
	return texNew;
}
float sampleShadowMap(sampler2D map , vec2 coords , float compare)
{
	if(coords.x < 0 || coords.y>1 || coords.x>1 || coords.y<0)
	return 1.0f;
 
	return step(compare , texture2D(map , coords.xy).r);
}
float sampleShadowMapLinear(sampler2D map , vec2 coords , float compare , vec2 texelSize)
{
	vec2 pixelPos = coords/texelSize + vec2(0.5);
	vec2 fracPart = fract(pixelPos);
	vec2 startTexel = (pixelPos - fracPart)*texelSize;
	
	float blTexel = sampleShadowMap(map , startTexel , compare);
	float brTexel = sampleShadowMap(map , startTexel + vec2(texelSize.x ,0.0) , compare);
	float tlTexel = sampleShadowMap(map , startTexel + vec2(0.0 , texelSize.y) , compare);
	float trTexel = sampleShadowMap(map , startTexel + texelSize , compare);

	float mixA = mix(blTexel , tlTexel , fracPart.y);
	float mixB = mix(brTexel , trTexel , fracPart.y);
	
	return mix(mixA , mixB , fracPart.x);
}
float sampleShadowMapPCF(sampler2D map , vec2 coords , float compare , vec2 texelSize)
{
	float result = 0.0f;
	for(float y = -1.0f ; y<= 1.0f ; y += 1.0f)
	{
		for(float x = -1.0f ; x<= 1.0f ; x += 1.0f)
		{
			vec2 coordsOffset = vec2(x,y)*texelSize;
			result += sampleShadowMapLinear(map , coords + coordsOffset , compare , texelSize);
		}
	}
	return result/9.0f;
}
float calcShadowMapEffect(sampler2D map , vec4 coords )
{
	vec3 finalCoords = (coords.xyz/coords.w)*vec3(0.5) + vec3(0.5);
	return sampleShadowMapPCF(map , finalCoords.xy , finalCoords.z - shadowBias , shadowTexelSize.xy);	
}
float calcVisibility(float d)
{
	float v =  exp(-pow((d*density) , gradient));
	return clamp(v , 0.0f , 1.0f);
}
void main()
{

	vec4 tspec = vec4(0.0,0.0,0.0,1.0);
	vec4 tdiff = vec4(0.0,0.0,0.0,1.0);
	mat3 Matrix = mat3(T,B,N);
	vec3 directionToEye = normalize(eyePos - worldPos0);
	vec2 texcoords = calcParallaxTexCoords(dispMap,Matrix,directionToEye,texCoord0,dispMapScale,dispMapBias);
	vec3 normal = normalize(Matrix*(255.0/128.0*texture2D(normalMap , texcoords.xy).xyz - 1));
	vec4 dColor = calcLightDiffuse(directionalLight.base , directionalLight.direction , normal);
	if(dColor.w>0){
		tdiff+= dColor;
		tspec += calcLightSpec(directionalLight.base , directionalLight.direction , normal);
	}
	//tspec = tspec*calcShadowMapEffect(shadowMap ,shadowMapCoords0);
	
	color = texture2D(diffuse, texcoords.xy);
	if(color.a<0.5)
		discard;
	color = (color*(tdiff+ tspec))*calcShadowMapEffect(shadowMap ,shadowMapCoords0) ;
	color = mix(vec4(0,0,0,1) , color , calcVisibility(length(worldPos0-eyePos)));
}
