#include "Flower.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "FlowerModel.h"
#include "Stem.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Math/mymath_withGLM.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Message/Console.h"

namespace myapp
{
    Flower::Flower(FlowerModel* model) :
        m_FlowerModel(model),
        m_FlowerMesh(nullptr),
        m_FlowerMaterial(nullptr),
        m_FlowerTRS(std::make_shared<TransformComponent>()),
        stemDataFlower_buffer(nullptr),
        stemDataFlower_buffer_index(4),
        DEBUG_buffer(nullptr),
        DEBUG_buffer_index(5),
        cal_flower_cs(nullptr),
        isFloweringTime(0.5f)
    {
        //
        std::string GPU_Flower_Cal_Flowers_comp = {
            #include "../../Shader/Forest/GPU_Flower_Cal_Flowers.comp"
        };
        cal_flower_cs = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, "", "", "", "", "", GPU_Flower_Cal_Flowers_comp);

        m_FlowerTRS->m_scale = glm::vec3(0.333f);

        //
        Start();
	}

	void Flower::Start() {
        Init();
	}

    void Flower::InitializeDispatch() {

    }

    void Flower::LinkBufferToResources(const std::shared_ptr<Stem>& stem) {
        // コンピュートシェーダーにバッファをセット
        cal_flower_cs->SetBuffer(stemDataFlower_buffer, stemDataFlower_buffer_index, m_FlowerMaterial);
        cal_flower_cs->SetBuffer(stem->stemVertex_buffer, stem->stemVertex_buffer_index, m_FlowerMaterial);
        cal_flower_cs->SetBuffer(stem->stemManage_buffer, stem->stemManage_buffer_index, m_FlowerMaterial);
        
        cal_flower_cs->SetBuffer(DEBUG_buffer, DEBUG_buffer_index);

        // マテリアルにバッファをセット
        m_FlowerMaterial->SetBuffer(stemDataFlower_buffer, stemDataFlower_buffer_index);
    }

    void Flower::Init() {
        kernel_CalFlowerGrowth = 0;

        InitBuffer();
        SetupFlowerdata();
    }

    void Flower::InitBuffer() {
        stemDataFlower_buffer = std::make_shared<ComputeBuffer>(m_FlowerModel->count * sizeof(StemData));
        DEBUG_buffer = std::make_shared<ComputeBuffer>(m_FlowerModel->count * sizeof(float) * 4 * 4);

        std::vector<StemData> initFlowerStemData;
        std::vector<float> initStemDebugMatrix(16* m_FlowerModel->count,0.0f);

        for (int i = 0; i < m_FlowerModel->count; i++) {
            glm::vec2 initPos = mymath::circleRand(1.0f,glm::vec2(float(i)+673.123f,0.1846f),glm::vec2(float(i)+4.12f));
            StemData data = StemData(i, glm::vec4(initPos.x, 0.0f, initPos.y,0.0f), glm::vec4(0.0f), glm::vec4(0.0f), glm::vec4(0.0f),glm::vec2(1.11123f,float(i)+5.512f));

            initFlowerStemData.push_back(data);
        }

        // バッファに初期値をセット
        stemDataFlower_buffer->SetData<std::vector<StemData>>(initFlowerStemData);
        DEBUG_buffer->SetData<std::vector<float>>(initStemDebugMatrix);
    }

    void Flower::SetupFlowerdata() {
        // base
        std::vector<std::vector<float>> vertices;
        std::vector<int> dimentions;
        std::vector<unsigned short> indices;

        // SplineData
        std::vector<glm::vec3> controlPoints;
        controlPoints.push_back(glm::vec3(0.0f, 0.0f, -0.1389077f));
        controlPoints.push_back(glm::vec3(1.16f, 0.5f, 0.2524655f));
        controlPoints.push_back(glm::vec3(2.95f, 0.45f, -0.1389077f));
        controlPoints.push_back(glm::vec3(3.26f, 1.39f, 0.2733917f));
        controlPoints.push_back(glm::vec3(5.68f, -0.03f, 0.9092102f));
        controlPoints.push_back(glm::vec3(8.6f, -0.48f, 0.08377504f));

        float knotMin = 0.0f;
        float knotMax = 1.0f;
        float tWidth = 0.0625f;

        // 花びら
        const auto& data = Cal_BSpline_Surface(controlPoints, knotMin, knotMax, tWidth);

        // 花
        std::shared_ptr<Multi_Flower_Data> multi_Flower_Data = RenderMultiFlower(data, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // test
        vertices.push_back(mymath::CastVec3ToLine_float(multi_Flower_Data->vertices));
        vertices.push_back(mymath::CastVec3ToLine_float(multi_Flower_Data->normals));
        dimentions.push_back(3);
        dimentions.push_back(3);
        indices = multi_Flower_Data->triangles;

        // set data to mesh
        m_FlowerMesh = std::make_shared<Mesh>(vertices, dimentions, indices);

        // material
        std::string GPUFlower_Flowers_Renderer_vert = {
            #include "../../Shader/Forest/GPUFlower_Flowers_Renderer.vert"
        };
        m_FlowerMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, GPUFlower_Flowers_Renderer_vert, shaderlib::ShaderLib::Standard_frag);
    }

	void Flower::Update() {
        if (m_FlowerModel->flowersIsDone&&m_FlowerModel->stemIsDone&&m_FlowerModel->leafIsDone) {
            Cal_flower_growth();

            /*// DEBUG
            float initStemDebugMatrix[16];
            DEBUG_buffer->GetBufferData<float>(&initStemDebugMatrix[0], 0, 16);
            for (int n=0;n<16;n++)
            {
                const auto& DEBUG_Val = initStemDebugMatrix[n];
                Console::Log("%d DEBUG_Val: %f\n",n, DEBUG_Val);
            }*/
        }
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

        //m_FlowerMesh->Draw();
        m_FlowerMesh->DrawInstancedWithMesh(m_FlowerModel->count,GL_TRIANGLES);
	}

    // 花びらから花を構築
    std::shared_ptr<Multi_Flower_Data> Flower::RenderMultiFlower(const std::shared_ptr<BaseFlower_Data>& flower_data, glm::vec3 flowerPosition,
        glm::vec3 flowerTangent, glm::vec3 flowerBioNormal, float flowerTime, int N)
    {
        std::shared_ptr<Multi_Flower_Data> data = std::make_shared<Multi_Flower_Data>();
        
        std::vector<glm::vec3> FibonacciPosition;
        std::vector<glm::quat> FibonacciRotation;
        std::vector<glm::vec4> FibonacciGrowthData;
        CalFibonacciPosition(FibonacciPosition,FibonacciRotation,FibonacciGrowthData, N);

        std::vector<glm::vec3> fibonacciVertices;
        std::vector<int> fibonacciIndices;
        std::vector<glm::vec3> fibonacciNormals;

        for (int i = 0; i < FibonacciPosition.size(); i++) {
            glm::vec3 fibPos = FibonacciPosition[i];
            
            glm::vec4 fibGroth = FibonacciGrowthData[i];
            glm::quat fibRot = glm::angleAxis(flowerTime * fibGroth.w, glm::vec3(fibGroth.x, fibGroth.y, fibGroth.z) ) * FibonacciRotation[i];
            fibRot = glm::angleAxis(
                glm::acos(dot(flowerTangent, glm::vec3(0.0f, 0.0f, 1.0f)) / glm::abs(glm::length(flowerTangent) * glm::length(glm::vec3(0.0f, 0.0f, 1.0f)))),
                flowerBioNormal
            ) * fibRot;
            
            for (int q = 0; q < flower_data->vertices.size(); q++) {
                float size = (float)(i + 1) * 0.01f;
                fibonacciVertices.push_back((flowerTime * size * (fibRot * flower_data->vertices[q] + fibPos * (1.0f / (float)(i + 1.0f))) + flowerPosition));
            }
            
            for (int p = 0; p < flower_data->normals.size(); p++) {
                fibonacciNormals.push_back((fibRot * flower_data->normals[p]));
            }
        }

        for (int i = 0; i < FibonacciPosition.size(); i++) {
            for (int p = 0; p < flower_data->triangles.size(); p++) {
                fibonacciIndices.push_back(i * (flower_data->vertices.size()) + flower_data->triangles[p]);
            }
        }

        for (int i = 0; i < fibonacciVertices.size(); i++) {
            data->vertices.push_back(fibonacciVertices[i]);
        }

        for (int i = 0; i < fibonacciNormals.size(); i++) {
            data->normals.push_back(fibonacciNormals[i]);
        }

        for (int i = 0; i < fibonacciIndices.size(); i++) {
            data->triangles.push_back(fibonacciIndices[i]);
        }

        return data;
    }

    // フィボナッチ数列の計算(花びらはフィボナッチ数列)
    void Flower::CalFibonacciPosition(std::vector<glm::vec3>& FibonacciPosition, std::vector<glm::quat>& FibonacciRotation,
        std::vector<glm::vec4>& FibonacciGrowthData, int N)
    {
        float goldenAngle = 137.509f;
        for (int i = 1; i < N + 1; i++) {
            glm::vec3 pos = glm::vec3(0.0f);
            float r = glm::sqrt((float)i);
            float ang = (float)(i - 1) * goldenAngle * (3.14f/180.0f);
            pos.x = r * glm::sin(ang);
            pos.z = r * glm::cos(ang);

            glm::vec3 crossVec = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), glm::normalize(pos));
            glm::quat fibRot = glm::quat(glm::vec3(0.0f, ang, 0.0f));
            // 弧度法を使用する
            float angVal = glm::pow((float)(i - 1) * 0.175f, 2.0f) * (3.14f / 180.0f);

            FibonacciPosition.push_back(pos);
            FibonacciRotation.push_back(fibRot);
            FibonacciGrowthData.push_back(glm::vec4(crossVec.x, crossVec.y, crossVec.z, angVal));
        }
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

    // 更新処理
    void Flower::Cal_flower_growth() {
        cal_flower_cs->SetActive();
        cal_flower_cs->SetIntUniform("_stemVertexCount", m_FlowerModel->m_Stem->stemVertexCount);
        cal_flower_cs->SetFloatUniform("_isFloweringTime", isFloweringTime);
        cal_flower_cs->SetFloatUniform("_DTime", GraphicsMain::GetInstance()->deltaTime);

        cal_flower_cs->Dispatch(m_FlowerModel->count / m_FlowerModel->m_Stem->numthreds_val, 1, 1);
    }
}