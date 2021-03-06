#ifndef RENDERINGENGINE_H
#define RENDERINGENGINE_H

#include"GameObject.h"
#include"FreeLook.h"
#include"InputHandler.h"
#include<vector>
class DefaultFilter;
class Display;
class DirectionalLight;
class PointLight;
class SpotLight;
class Texture;
class Material;
class Mesh;
class SkyBoxManager;
class WaterRenderer;
class SunRenderer;

class RenderingEngine
{
public:
    RenderingEngine(Display* d);
    void renderScene(GameObject* object , Camera* mainCamera , Texture* target);
    void render(GameObject* object);
    void addDirectionalLight(DirectionalLight* light);
    void addPointLight(PointLight* light);
    void addSpotLight(SpotLight* light);
    void addCamera(Camera* c) { camera = c;}
    void addSkyBox(std::string filename , std::string format);
    void addWaterTile(glm::vec3 pos , glm::vec3 scale);
    void renderDepthMap(GameObject* object , Camera* mainCamera , Texture* target);
    void renderGodRaysSampler(GameObject* object , Camera* mainCamera , Texture* target);
    glm::vec4 getClipingPlane() const { return clipPlane ; }
    PointLight* getActivePointLight() { return active_point_light;}
    DirectionalLight* getActiveDirectionalLight() { return active_dir_light;}
    SpotLight* getActiveSpotLight() { return active_spot_light;}
    Texture* getDirShadowMap() { return temptarget ; }
    Texture* getPointShadowMap() { return pointshadowMap ; }
    glm::mat4 getLightMatrix() { return m_lightMatrix;}
    CoreEngine* getCoreEngine() { return core;}
    float getShadowBias() { return shadowMapBias;}
    void setShadowBias(float b) { shadowMapBias = b;}
    glm::vec3 getShadowTexelSize() { return shadowTexelSize ;}
    void setCoreEngine(CoreEngine* c) { core = c ;}
    Texture* getDepthMap() {return depthMap;}
    Camera* getCamera() { return camera;}
    void setFogColor(float r , float g , float b) { fogColor = glm::vec4(r,g,b,1);}
    glm::vec4 getFogColor() { return fogColor;}
    SunRenderer* getSunrenderer() { return sunRenderer ; }
    void setScene(Texture* sce) {scene = sce; }
    virtual ~RenderingEngine();

private:
    std::vector<DirectionalLight*>dir_lights;
    std::vector<PointLight*>point_lights;
    std::vector<SpotLight*>spot_lights;
    PointLight* active_point_light;
    DirectionalLight* active_dir_light;
    SpotLight* active_spot_light;
    Shader* ambientShader;
    Shader* dirShadowShader;
    Shader* pointShadowShader;
    Shader* depthShader;
    Shader* ssaoShader;
    Shader* godRaysFirstPass;
    Shader* doNothing;
    DefaultFilter * ssaoFilter;
    Camera* camera;
    Camera* altCamera;
    FreeLook* tempCamera;
    Texture* temptarget;
    Texture* pointshadowMap;
    Texture* depthMap;
    Texture* scene;
    Texture* noise;
    glm::vec4 clipPlane;
    glm::vec4 fogColor;
    float shadowMapBias ;
    glm::vec3 shadowTexelSize;
    Mesh * mesh ;
    Transform temp_transform;
    Material* temp_material;
    GameObject* cameraObject1;
    GameObject* cameraObject2;
    glm::mat4 m_lightMatrix;
    Display* display;
    CoreEngine* core;
    SkyBoxManager* skyBoxManager;
    WaterRenderer* waterRenderer;
    SunRenderer*  sunRenderer;
    RenderingEngine(const RenderingEngine& other) {}
    RenderingEngine& operator=(const RenderingEngine& other) {}
};

#endif // RENDERINGENGINE_H
