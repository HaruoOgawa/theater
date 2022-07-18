#include "Flower.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"

namespace myapp
{
	Flower::Flower():
        m_FlowerMesh(nullptr),
        m_FlowerMaterial(nullptr),
        m_FlowerTRS(std::make_shared<TransformComponent>())
    {
        Start();
	}

	void Flower::Start() {
        // base
        std::vector<std::vector<float>> vertices;
        std::vector<int> dimentions;
        std::vector<unsigned short> indices;

        // SplineData
        std::vector<glm::vec3> controlPoints;
        controlPoints.push_back(glm::vec3(0.0f,0.0f, -0.1389077f));
        controlPoints.push_back(glm::vec3(1.16f, 0.5f, 0.2524655f));
        controlPoints.push_back(glm::vec3(2.95f, 0.45f, -0.1389077f));
        controlPoints.push_back(glm::vec3(3.26f, 1.39f, 0.2733917f));
        controlPoints.push_back(glm::vec3(5.68f, -0.03f, 0.9092102f));
        controlPoints.push_back(glm::vec3(8.6f, -0.48f, 0.08377504f));

        float knotMin = 0.0f;
        float knotMax = 1.0f;
        float tWidth = 0.0625f;

        // Create Mesh Data
        const auto& data = Cal_BSpline_Surface(controlPoints, knotMin, knotMax, tWidth);

        // test
        vertices.push_back(CastVec3ToLine_float(data->vertices));
        vertices.push_back(CastVec3ToLine_float(data->normals));
        dimentions.push_back(3);
        dimentions.push_back(3);
        indices = data->triangles;

        // set data to mesh
        m_FlowerMesh = std::make_shared<Mesh>(vertices, dimentions, indices);

        // material
        m_FlowerMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER,shaderlib::ShaderLib::Standard_vert,shaderlib::ShaderLib::Standard_frag);
	}

	void Flower::Update() {

	}

	void Flower::Draw() {
        m_FlowerMaterial->SetActive();
        m_FlowerTRS->CalMatrix();
        m_FlowerMaterial->SetMatrixUniform("MVPMatrix", m_FlowerTRS->m_pMatrix * m_FlowerTRS->m_vMatrix * m_FlowerTRS->m_mMatrix);
        m_FlowerMaterial->SetMatrixUniform("MMatrix", m_FlowerTRS->m_mMatrix);
        m_FlowerMaterial->SetMatrixUniform("VMatrix", m_FlowerTRS->m_vMatrix);
        m_FlowerMaterial->SetMatrixUniform("PMatrix", m_FlowerTRS->m_pMatrix);
        m_FlowerMaterial->SetIntUniform("_UseLighting", 1);
        m_FlowerMaterial->SetVec3Uniform("_LightDir", glm::vec3(1.0, 1.0, -1.0) );

        m_FlowerMesh->Draw();
	}

    // Bスプライン曲線で花びらを構築
	std::shared_ptr<BaseFlower_Data> Flower::Cal_BSpline_Surface(std::vector<glm::vec3> controlPoints, float knotMin, float knotMax, float tWidth) {
        std::shared_ptr<BaseFlower_Data> baseFlower_Data =  std::make_shared<BaseFlower_Data>();
        
        std::vector<std::shared_ptr<B_Spline_Data>> data;
        data = Cal_BSplineCurve(controlPoints, knotMin, knotMax, tWidth);

        std::vector<glm::vec3> pos;
        std::vector<int> triangles;

        for (int i = 0; i < data.size(); i++) {
            glm::vec3 p = data[i]->position;
            p = glm::quat(glm::vec3(0.0f, 0.0f, 3.14f/2.0f)) * p;
            pos.push_back(p);
        }

        for (int i = 0; i < data.size(); i++) {
            glm::vec3 p = data[i]->position;
            p.y = -p.y;
            p = glm::quat(glm::vec3(0.0f, 0.0f, 3.14f / 2.0f)) * p;
            pos.push_back(p);
        }

        int posSize = pos.size() - 2;
        int rightSize = (posSize) / 2;
        int leftSize = posSize - rightSize;

        //rightTriangles 
        for (int i = 0; i < rightSize; i++) {
            triangles.push_back(i);
            triangles.push_back(i + 1);
            triangles.push_back(pos.size() - i - 1);
        }

        //lefyTriangles
        for (int i = 0; i < leftSize; i++) {
            triangles.push_back(pos.size() - i - 1);
            triangles.push_back(pos.size() - i - 2);
            triangles.push_back(i + 1);
        }

        for (int i = 0; i < pos.size(); i++) {
            baseFlower_Data->vertices.push_back(pos[i]);
        }

        //rightNormals 
        for (int i = 0; i < rightSize; i++) {
            glm::vec3 p0 = baseFlower_Data->vertices[i];
            glm::vec3 p1 = baseFlower_Data->vertices[i + 1];
            glm::vec3 p2 = baseFlower_Data->vertices[pos.size() - i - 1];

            glm::vec3 v0 = glm::normalize(p1 - p0);
            glm::vec3 v1 = glm::normalize(p2 - p0);

            glm::vec3 normal = glm::normalize(glm::cross(v0, v1));

            baseFlower_Data->normals.push_back(normal);
        }
        baseFlower_Data->normals.push_back(baseFlower_Data->normals[baseFlower_Data->normals.size() - 1]);

        //lefyNormals
        for (int i = 0; i < leftSize; i++) {
            glm::vec3 p0 = baseFlower_Data->vertices[pos.size() - i - 1];
            glm::vec3 p1 = baseFlower_Data->vertices[pos.size() - i - 2];
            glm::vec3 p2 = baseFlower_Data->vertices[i + 1];

            glm::vec3 v0 = glm::normalize(p1 - p0);
            glm::vec3 v1 = glm::normalize(p2 - p0);

            glm::vec3 normal = glm::normalize(glm::cross(v0, v1));

            baseFlower_Data->normals.push_back(normal);

        }

        baseFlower_Data->normals.push_back(baseFlower_Data->normals[baseFlower_Data->normals.size() - 1]);

        //triangles
        for (int i = 0; i < triangles.size(); i++) {
            baseFlower_Data->triangles.push_back(triangles[i]);
        }

        return baseFlower_Data;
	}

    // 曲線データの作成
    std::vector<std::shared_ptr<B_Spline_Data>> Flower::Cal_BSplineCurve(std::vector<glm::vec3> controlPoints, float knotMin, float knotMax, float tWidth)
    {
        int p = controlPoints.size();
        int n = 3;
        int m = p + n + 1;

        std::vector<float> u = GetKnotVector(m, n, knotMin, knotMax);

        std::vector<float> tDelta;
        int num = (int)(u[u.size() - 1] / tWidth);
        for (int i = 0; i < num; i++) {
            tDelta.push_back((float)(tWidth * i));
        }

        std::vector<std::shared_ptr<B_Spline_Data>> S;
        for (int i = 0; i < tDelta.size(); i++) {
            S.push_back(std::make_shared<B_Spline_Data>(glm::vec3(0.0f, 0.0f, 0.0f), i));
        }

        S[0] = std::make_shared<B_Spline_Data>(controlPoints[0], S[0]->index);

        for (int i = 1; i < tDelta.size(); i++) {
            for (int j = 0; j < p; j++) {
                float b = GetBasisFunction(u, j, n, tDelta[i]);
                S[i] = std::make_shared<B_Spline_Data>(S[i]->position + controlPoints[j] * b, S[i]->index);
            }
        }

        return S;

    }

    //
    std::vector<float> Flower::GetKnotVector(int m, int n, float knotMin, float knotMax)
    {
        std::vector<float> knotVector;
        int knotN = n + 1;

        for (int i = 0; i < m; i++) {
            if (i >= 0 && i < knotN) {
                knotVector.push_back(knotMin);
            }
            else if (i >= knotN && i < (m - knotN)) {
                int knotWidth = m - knotN * 2;
                float knotVal = (knotMax - knotMin) / (float)(knotWidth + 1);
                knotVal = knotVal * (float)(i - knotN + 1);
                knotVector.push_back(knotVal);
            }
            else if (i >= (m - knotN) && i < m) {
                knotVector.push_back(knotMax);
            }
        }

        return knotVector;
    }

    //
    float Flower::GetBasisFunction(std::vector<float> u, int j, int k, float t)
    {
        float w1 = 0.0f;
        float w2 = 0.0f;

        if (k == 0) {
            if (u[j] < t && t <= u[j + 1]) {
                return 1.0f;
            }
            else {
                return 0.0f;
            }
        }
        else {
            if (u[j + k + 1] - u[j + 1] != 0.0f) {
                w1 = GetBasisFunction(u, j + 1, k - 1, t) * (u[j + k + 1] - t) / (u[j + k + 1] - u[j + 1]);
            }

            if ((u[j + k] - u[j]) != 0.0f) {
                w2 = GetBasisFunction(u, j, k - 1, t) * (t - u[j]) / (u[j + k] - u[j]);
            }

            return w1 + w2;
        }

    }
}