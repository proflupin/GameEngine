#ifndef SHADER_INCLUDED_H
#define SHADER_INCLUDED_H

#include <string>
#include<map>
#include <GL/glew.h>
#include "transform.h"
#include"../include/Light.h"
#include"Matrial.h"


class Shader
{
public:
    Shader(const std::string & filename);
    virtual void Update(const Transform& transform, const Camera& camera,const Material& m);
    void Bind() const ;

    void setUniformMatrix4f(const std::string& uName , const GLfloat* mat)
	{
        glUniformMatrix4fv(m_uniforms[uName], 1, GL_FALSE, mat);
	}
	void setUniformVector4f(const std::string& uName , float x,float y, float z , float w)
	{
       glUniform4f(m_uniforms[uName], x,y,z,w);
	}
	void setUniformVector3f(const std::string& uName , float x,float y,float z)
	{
       glUniform3f(m_uniforms[uName], x, y, z);
	}
	void setUniform1f(const std::string& uName , float x)
	{
        glUniform1f(m_uniforms[uName], x);
	}
	virtual ~Shader();
private:
    static const unsigned int NUM_SHADERS = 2;
	void operator=(const Shader& shader) {}
	Shader(const Shader& shader) {}

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);
	GLuint m_shaders[NUM_SHADERS];
protected:
	std::map<std::string , GLuint> m_uniforms;
    GLuint m_program;
};

class BasicShader : public Shader
{
public:
    BasicShader(const std::string& fileName) ;
    virtual void Update(const Transform& transform, const Camera& camera , const Material& m);
	virtual ~BasicShader();
};
class PhongShader : public BasicShader
{
public:
	PhongShader(const std::string& fileName);
	void Update(const Transform& transform, const Camera& camera , const Material& m);
	void setUniformPointLights(PointLight* pArray , int n);
	void setUniformSpotLights(SpotLight* sArray , int n);
	virtual ~PhongShader();
protected:
private:
    static const unsigned int NUM_POINT_LIGHTS = 4;
    static const unsigned int NUM_SPOT_LIGHTS = 4;
};

#endif
