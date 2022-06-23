#pragma once
#include "GraphicsEngine/Component/Component.h"
#include "../Object/Object.h"
#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include <string>
#include "../Graphics/Primitive.h"
#include "../GraphicsMain/GraphicsMain.h"
#include <functional>
#include <memory>
#include "GraphicsEngine/Graphics/EGraphicsParam.h"

class Object;
class Mesh;
class Material;
class TimelineAnimationClip;

class MeshRendererComponent :
    public Component
{
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
    class Object* myowner;
    bool useZTest;
public:
    MeshRendererComponent(class Object* o, PrimitiveType primType, RenderingSurfaceType SurfaceType,
        const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tc, const std::string& tv);
    ~MeshRendererComponent();

    void Update() override;
    void Draw();
    void DrawBoard();
    static void DrawInstancedWithMesh(std::shared_ptr<class Mesh> mesh, int count, std::shared_ptr<class Material> material, GLenum rendermode);
   
    const std::shared_ptr<Mesh>& GetMesh()const;
    const std::shared_ptr<Material>& GetMaterial()const;
    void SetUseZTest(bool use);
    bool GetUseZTest()const;

    std::vector<TimelineAnimationClip*> animationClips;

    friend class GraphicsRenderer;
private:
    std::unique_ptr<class Texture> primTex;
};