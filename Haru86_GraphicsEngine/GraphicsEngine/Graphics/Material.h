#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <map>

enum RenderingSurfaceType
{
    POLYGON,
    RAYMARCHING,
};

class Material 
{
public:
    Material();
    Material(std::map<GLenum, std::string> shaders);
    Material(std::string vertexShaderName, std::string fragmentShaderName);
    Material(std::string vertexShaderName, std::string fragmentShaderName, RenderingSurfaceType renderingSurfaceType);
    Material(std::string vertexShaderName, std::string tessellationShaderName[2], std::string fragmentShaderName);
    Material(std::string vertexShaderName, std::string geometryShaderName, std::string fragmentShaderName);
    ~Material();
    void LoadShader(std::map<GLenum,std::string> shaders,GLuint& prg, GLuint& vert, GLuint& tc, GLuint& tv, GLuint& geom, GLuint& frag);
    void LoadShader(std::string vertName,std::string fragName,GLuint& prg, GLuint& vert, GLuint& tc, GLuint& tv, GLuint& geom, GLuint& frag);
    void LoadShader(std::string vertName, std::string tessellationShaderName[2],std::string fragName, GLuint& prg, GLuint& vert, GLuint& tc, GLuint& tv, GLuint& geom, GLuint& frag);
    void LoadShader(std::string vertName,std::string geomName,std::string fragName, GLuint& prg, GLuint& vert, GLuint& tc, GLuint& tv, GLuint& geom, GLuint& frag);
    bool CompileShader(std::string shadername,GLenum shaderType,GLuint& outShader);
    void SetActive();
    void UnLoadData();
    void SetMatrixUniform(std::string uniformName,glm::mat4 matrix);
    void SetMatrixVectorUniform(std::string uniformName, const std::vector<glm::mat4>& matrixVector);
    void SetFloatUniform(std::string uniformName,float val);
    void SetIntUniform(std::string uniformName,int val);
    void SetVec2Uniform(std::string uniformName, glm::vec2 val);
    void SetVec3Uniform(std::string uniformName, glm::vec3 val);
    void SetTexUniform(std::string uniformName, unsigned int val);
    void SetFloatVectorUniform(std::string uniformName, std::vector<float> val);
    void SetBuffer(std::shared_ptr<class ComputeBuffer> buffer, int bufferIndex);
    void LoadTextureList(std::vector<std::string> texPathList,std::vector<std::string> texUniformNameList);
    void SetActiveTextureList();
    void SetEactiveTextureList();
    GLuint GetCurrentShaderPrg();
private :
    //Color Render Buffer
    GLuint vertShaderData;
    GLuint tessControlShaderData;
    GLuint tessEvalShaderData;
    GLuint geometryShaderData;
    GLuint fragShaderData;
    GLuint shaderPrg;

    //Depth Render Buffer
    GLuint depthVertShaderData;
    GLuint depthTessControlShaderData;
    GLuint depthTessEvalShaderData;
    GLuint depthGeometryShaderData;
    GLuint depthFragShaderData;
    GLuint depthShaderPrg;

    std::vector<std::unique_ptr<class Texture>> m_texture_list;
};

