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
    MeshRendererComponent(class Object* o, PrimitiveType primType,
        const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tc, const std::string& tv);
    ~MeshRendererComponent();

    void Update() override;
    void Draw();
    void DrawBoard() override;
    void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) override;

    friend class GraphicsRenderer;
private:
    std::unique_ptr<class Texture> primTex;
};