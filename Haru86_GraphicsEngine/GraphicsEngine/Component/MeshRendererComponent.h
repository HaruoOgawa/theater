#pragma once
#include "../Graphics/Mesh.h"
#include "../Graphics/Material.h"
#include "../Graphics/Primitive.h"
#include "GraphicsEngine/Graphics/Texture.h"

class GameObject;
class Mesh;
class Material;
class TimelineAnimationClip;

class MeshRendererComponent
{
public:
    MeshRendererComponent(class GameObject* o, PrimitiveType primType, RenderingSurfaceType SurfaceType,
        const std::string& vert, const std::string& frag, const std::string& geom, const std::string& tc, const std::string& tv,const std::string& cs);
    ~MeshRendererComponent()=default;

    void Draw();
    
    std::vector<TimelineAnimationClip*> animationClips;
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Material> m_material;
    class GameObject* myowner;
    bool useZTest;

    friend class GraphicsRenderer;
    friend GameObject;

private:
    class GameObject* owner;
    class GraphicsMain* game;
    std::unique_ptr<Texture> primTex;
};