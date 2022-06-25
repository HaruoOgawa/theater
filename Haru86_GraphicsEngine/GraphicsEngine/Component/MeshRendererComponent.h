#pragma once
#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include <string>
#include "../Graphics/Primitive.h"
#include "../GraphicsMain/GraphicsMain.h"
#include <functional>
#include <memory>
#include "GraphicsEngine/Graphics/EGraphicsParam.h"
#include "GraphicsEngine/Graphics/Texture.h"

class GameObject;
class Mesh;
class Material;
class TimelineAnimationClip;
//class Texture;

class MeshRendererComponent
{
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
    class GameObject* myowner;
    bool useZTest;
public:
    MeshRendererComponent(class GameObject* o, PrimitiveType primType, RenderingSurfaceType SurfaceType,
        const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tc, const std::string& tv,const std::string& cs);
    ~MeshRendererComponent()=default;

    void Draw();
    void DrawBoard();
    static void DrawInstancedWithMesh(std::shared_ptr<class Mesh> mesh, int count, std::shared_ptr<class Material> material, GLenum rendermode);
   
    const std::shared_ptr<Mesh>& GetMesh()const;
    const std::shared_ptr<Material>& GetMaterial()const;
    void SetUseZTest(bool use);
    bool GetUseZTest()const;

    std::vector<TimelineAnimationClip*> animationClips;

    friend class GraphicsRenderer;
    friend GameObject;
private:
    class GameObject* owner;
    class GraphicsMain* game;
    std::unique_ptr<Texture> primTex;
};