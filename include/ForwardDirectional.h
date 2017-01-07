#ifndef FORWARDDIRECTIONAL_H_INCLUDED
#define FORWARDDIRECTIONAL_H_INCLUDED

#include"Light.h"
#include"shader.h"

class ForwardDirectional: public Shader
{
public:
    ForwardDirectional(const std::string fileName): Shader(fileName)
    {
	m_uniforms["eyePos"] = glGetUniformLocation(m_program , "eyePos");
	m_uniforms["specularPower"] = glGetUniformLocation(m_program , "specularPower");
	m_uniforms["specularIntensity"] = glGetUniformLocation(m_program , "specularIntensity");
	std::string light = "directionalLight";
    m_uniforms[light + ".base.color"] = glGetUniformLocation(m_program ,(light+".base.color").c_str());
    m_uniforms[light + ".base.intensity"] = glGetUniformLocation(m_program ,(light+".base.intensity").c_str());
    m_uniforms[light + ".direction"] = glGetUniformLocation(m_program ,(light+".direction").c_str());

    }
    void Update(const Camera&c,const Material& material , RenderingEngine* renderingEngine)
    {
        Shader::Update(c,material,renderingEngine);
        std::string light = "directionalLight";
        DirectionalLight* dirlight = renderingEngine->getActiveDirectionalLight();
        glm::vec3 color = dirlight->getColor();
        setUniformVector3f(light+".base.color" ,color.x , color.y , color.z);
        setUniform1f(light+".base.intensity" , dirlight->getIntensity());
        glm::vec3 dir = dirlight->getDirection();
        setUniformVector3f(light+".direction" ,dir.x , dir.y , dir.z);
        setUniform1f("specularPower" , material.getSpecularPower());
        setUniform1f("specularIntensity" , material.getSpecularIntensity());
        glm::vec3 p = c.getPos();
        setUniformVector3f("eyePos" , p.x , p.y , p.z);
    }

};

#endif // FORWARDDIRECTIONAL_H_INCLUDED