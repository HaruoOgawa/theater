#pragma once
#include "../Component/ARendererComponent.h"
#include "../Object/Object.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include <string>
#include "../Graphics/Primitive.h"
#include "../GraphicsMain/GraphicsMain.h"
#include <functional>
#include <memory>

class MeshRendererComponent :
    public ARendererComponent
{
public:
    MeshRendererComponent(class Object* o, PrimitiveType primType);
    MeshRendererComponent(class Object* o, PrimitiveType primType, std::map<GLenum, std::string> shaders);
    MeshRendererComponent(class Object* o, PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName);
    MeshRendererComponent(class Object* o, PrimitiveType primType, std::string vertexShaderName, std::string tessellationShaderName[2], std::string fragmentShaderName);
    MeshRendererComponent(class Object* o, PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName, std::string textureString);
    ~MeshRendererComponent();

    void Update() override;
    void Draw();
    void DrawBoard() override;
    void ProcessInput(const SDL_Event& e) override;

    friend class GraphicsRenderer;
private:
    std::unique_ptr<class Texture> primTex;
};