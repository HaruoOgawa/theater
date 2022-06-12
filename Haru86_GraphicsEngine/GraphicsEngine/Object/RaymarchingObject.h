#pragma once
#include "Object.h"
#include <memory>
#include <string>

class Material;
class Mesh;

class RaymarchingObject :
    public Object
{
public:
    RaymarchingObject(std::string fragmentShaderName);
    ~RaymarchingObject();
    void Start();
    void Update() override;
    void Draw();
private:
    std::shared_ptr<Material> m_material;
    std::shared_ptr<Mesh> m_mesh;
};

