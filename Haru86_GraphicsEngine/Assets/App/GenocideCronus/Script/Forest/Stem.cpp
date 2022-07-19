#include "Stem.h"
#include "FlowerModel.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace myapp {
	Stem::Stem(FlowerModel* model) :
		m_FlowerModel(model),
        stemRadius(0.2f),
        stemSegments(12),
        stemLength(2.5),
        stemGrowthRange(500),
        numthreds_val(256),
        stemResult_buffer_index(0),
        stemVertex_buffer_index(1),
        stemManage_buffer_index(2),
        stemBasePosition_buffer_index(3)
	{
        // コンピュートシェーダー
        std::string GPU_Flower_Cal_Stem_comp = {
            #include "Assets/App/GenocideCronus/Shader/Forest/GPU_Flower_Cal_Stem.comp"
        };

        cal_stem_cs = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, "", "", "", "", "", GPU_Flower_Cal_Stem_comp);

        // マテリアル
        std::string GPUFlower_Stem_Renderer_vert = {
            #include "Assets/App/GenocideCronus/Shader/Forest/GPUFlower_Stem_Renderer.vert"
        };
        std::string GPUFlower_Stem_Renderer_geom = {
            #include "Assets/App/GenocideCronus/Shader/Forest/GPUFlower_Stem_Renderer.geom"
        };
        stem_mat = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, GPUFlower_Stem_Renderer_vert,shaderlib::ShaderLib::Standard_frag, GPUFlower_Stem_Renderer_geom);

        // ポイントメッシュ
        stem_point_mesh = std::make_shared<Mesh>(PrimitiveType::POINT);

        //
        Start();
	}

	void Stem::Start() {
        Init();
        Cal_Stem_Result();
        Init_Stem_Growth();

        //gPUFlower_Base.stemIsDone = true;
	}

	void Stem::Update() {
        Cal_Stem_Manage();
        Cal_Stem_Growth();
	}

	void Stem::Draw() {
        Render_Stem();
	}

	//////////////
    void Stem::Init() {

        // コンピュートバッファを初期化
        stemResult_kernel = 0; //cal_stem_cs.FindKernel("CalStemBSplineCurveResult");
        InitStemGrowth_kernel = 1; // cal_stem_cs.FindKernel("InitStemGrowth");
        CalStemManage_kernel = 2; // cal_stem_cs.FindKernel("CalStemManage");
        stemGrowth_kernel = 3; // cal_stem_cs.FindKernel("CalStemGrowth");

        std::vector<glm::vec3> controlPoints;
        controlPoints.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
        controlPoints.push_back(glm::vec3(0.0f, 1.16f, 0.0f));
        controlPoints.push_back(glm::vec3(0.0f, 2.95f, 0.0f));
        controlPoints.push_back(glm::vec3(0.0f, 3.26f, 0.0f));
        controlPoints.push_back(glm::vec3(0.0f, 5.68f, 0.0f));
        controlPoints.push_back(glm::vec3(0.0f, 9.35f, 0.0f));
        bSplineData = std::make_shared<BSplineData>(controlPoints,0.0f,1.0f, 0.015625);

        stemVertexCount = (int)((bSplineData->knotMax - bSplineData->knotMin) / bSplineData->tWidth);
        stemResult_buffer = std::make_shared<ComputeBuffer>(stemVertexCount * m_FlowerModel->count * sizeof(StemVertex) );
        stemVertex_buffer = std::make_shared<ComputeBuffer>(stemVertexCount * m_FlowerModel->count * sizeof(StemVertex));
        stemManage_buffer = std::make_shared<ComputeBuffer>(m_FlowerModel->count * sizeof(StemManage));
        stemBasePosition_buffer = std::make_shared<ComputeBuffer>(m_FlowerModel->count *sizeof(glm::vec3));

        InitBufferData();

        // マテリアルにバッファをセット
        stem_mat->SetBuffer(stemVertex_buffer,stemVertex_buffer_index); // _stemVertex_buffer
        stem_mat->SetBuffer(stemManage_buffer,stemManage_buffer_index); // _read_stemManage_buffer
    }

    void Stem::InitBufferData() {
        std::vector<std::shared_ptr<StemVertex>> initStemVertex;
        std::vector<std::shared_ptr<StemManage>> initStemManege;

        std::vector<glm::vec3> initStemBasePosition;

        std::vector<glm::mat4> initStemDebugMatrix;

        for (int i = 0; i < m_FlowerModel->count; i++) {
            initStemVertex.push_back(std::make_shared<StemVertex>(i));

            std::shared_ptr<StemManage> stemManage = std::make_shared<StemManage>(1);
            if (stemManage->stemLifeVal == 0.0f || stemManage->stemLifeVal == 1.0f) {
                stemManage->stemWaitTime = glm::gaussRand(1.0f, 3.0f);
                stemManage->manageLifeCountFlag = 0;
            }
            initStemManege.push_back(stemManage);
            
            glm::vec2 initBasePos = glm::sphericalRand(stemGrowthRange);
            initStemBasePosition.push_back(glm::vec3(initBasePos.x, 0.0f, initBasePos.y));

            initStemDebugMatrix.push_back(glm::mat4());
        }
        stemResult_buffer->SetData(initStemVertex);
        stemVertex_buffer->SetData(initStemVertex);
        stemManage_buffer->SetData(initStemManege);
        stemBasePosition_buffer->SetData(initStemBasePosition);
    }

    void Stem::Cal_Stem_Result() {
        cal_stem_cs->SetBuffer(stemResult_buffer,stemResult_buffer_index, stem_mat); // _write_stemResult_buffer
        cal_stem_cs->SetBuffer(stemBasePosition_buffer, stemBasePosition_buffer_index, stem_mat); // _read_stemBasePosition_buffer
        std::vector<glm::vec4> contPos;
        cal_stem_cs->SetIntUniform("_contPosArrayLength", bSplineData->controlPoints.size());
        for (int i = 0; i < bSplineData->controlPoints.size(); i++) {
            glm::vec3 controlPoint = bSplineData->controlPoints[i];
            contPos.push_back(glm::vec4(controlPoint.x, controlPoint.y * stemLength * glm::gaussRand(0.5f, 1.5f), controlPoint.z, 0.0f));
        }
        cal_stem_cs->SetVec4ArrayUniform("_controlPoints", contPos);
        cal_stem_cs->SetIntUniform("_stemVertexCount", stemVertexCount);
        cal_stem_cs->SetFloatUniform("_knotMin", bSplineData->knotMin);
        cal_stem_cs->SetFloatUniform("_knotMax", bSplineData->knotMax);
        cal_stem_cs->SetFloatUniform("_tWidth", bSplineData->tWidth);

        cal_stem_cs->SetIntUniform("_KernelIndex", stemResult_kernel);
        cal_stem_cs->Dispatch((stemVertexCount * m_FlowerModel->count) / numthreds_val, 1, 1);
        
    }

    void Stem::Init_Stem_Growth() {
        cal_stem_cs->SetBuffer(stemResult_buffer,stemResult_buffer_index); // _read_stemResult_buffer
        cal_stem_cs->SetBuffer(stemVertex_buffer,stemVertex_buffer_index); // _write_stemVertex_buffer
        cal_stem_cs->SetBuffer(stemManage_buffer,stemManage_buffer_index); // _read_stemManage_buffer

        cal_stem_cs->SetBuffer(stemBasePosition_buffer,stemBasePosition_buffer_index); // _read_stemBasePosition_buffer

        cal_stem_cs->SetIntUniform("_stemVertexCount", stemVertexCount);
        // 
        cal_stem_cs->SetIntUniform("_KernelIndex", InitStemGrowth_kernel);
        cal_stem_cs->Dispatch((stemVertexCount * m_FlowerModel->count) / numthreds_val, 1, 1);
    }

    void Stem::Cal_Stem_Manage() {
        cal_stem_cs->SetBuffer(stemManage_buffer,stemManage_buffer_index); // _write_stemManage_buffer

        cal_stem_cs->SetBuffer(stemBasePosition_buffer,stemBasePosition_buffer_index); // _read_stemBasePosition_buffer
        cal_stem_cs->SetFloatUniform("_stemGrowthRange", stemGrowthRange);
        cal_stem_cs->SetFloatUniform("_DTime", GraphicsMain::GetInstance()->deltaTime);

        cal_stem_cs->SetIntUniform("_KernelIndex", CalStemManage_kernel);
        cal_stem_cs->Dispatch(m_FlowerModel->count / numthreds_val, 1, 1);
    }
    void Stem::Cal_Stem_Growth() {
        cal_stem_cs->SetIntUniform("_stemVertexCount", stemVertexCount);
        cal_stem_cs->SetBuffer(stemResult_buffer,stemResult_buffer_index); // _read_stemResult_buffer
        cal_stem_cs->SetBuffer(stemVertex_buffer,stemVertex_buffer_index); // _write_stemVertex_buffer
        cal_stem_cs->SetBuffer(stemManage_buffer,stemManage_buffer_index); // _read_stemManage_buffer

        cal_stem_cs->SetBuffer(stemBasePosition_buffer,stemBasePosition_buffer_index); // _read_stemBasePosition_buffer

        cal_stem_cs->SetIntUniform("_KernelIndex", stemGrowth_kernel);
        cal_stem_cs->Dispatch((stemVertexCount * m_FlowerModel->count) / numthreds_val, 1, 1);
    }

    void Stem::Render_Stem() {
        
        stem_mat->SetActive();
        stem_mat->SetIntUniform("_stemVertexCount", stemVertexCount);
        stem_mat->SetIntUniform("_stemSegments", stemSegments);
        stem_mat->SetFloatUniform("_stemRadius", stemRadius);
        stem_mat->SetFloatUniform("_stemLength", stemLength);
        
        stem_point_mesh->DrawInstancedWithMesh(stemVertexCount * m_FlowerModel->count, GL_POINTS);
    }
	/////////////
}