#include "Stem.h"
#include "FlowerModel.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Math/mymath_withGLM.h"

namespace myapp {
	Stem::Stem(FlowerModel* model) :
		m_FlowerModel(model),
        stemRadius(0.2f),
        stemSegments(12),
        stemLength(2.5),
        stemGrowthRange(200),
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

        // TRS
        m_StemTRS = std::make_shared<TransformComponent>();
        m_StemTRS->m_scale = glm::vec3(0.333f);

        //
        Start();
	}

	void Stem::Start() {
        Init();
    }

    void Stem::InitializeDispatch() {
        Cal_Stem_Result();
        Init_Stem_Growth();
    }

    void Stem::LinkBufferToResources() {
        // コンピュートシェーダーにバッファをセット
        cal_stem_cs->SetBuffer(stemResult_buffer, stemResult_buffer_index, stem_mat); // _write_stemResult_buffer
        cal_stem_cs->SetBuffer(stemBasePosition_buffer, stemBasePosition_buffer_index, stem_mat); // _read_stemBasePosition_buffer
        cal_stem_cs->SetBuffer(stemVertex_buffer, stemVertex_buffer_index, stem_mat); // _write_stemVertex_buffer
        cal_stem_cs->SetBuffer(stemManage_buffer, stemManage_buffer_index, stem_mat); // _read_stemManage_buffer

        // マテリアルにバッファをセット
        stem_mat->SetBuffer(stemVertex_buffer, stemVertex_buffer_index); // _stemVertex_buffer
        stem_mat->SetBuffer(stemManage_buffer, stemManage_buffer_index); // _read_stemManage_buffer
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
        stemBasePosition_buffer = std::make_shared<ComputeBuffer>(m_FlowerModel->count *sizeof(StemBasePosition));

        InitBufferData();
    }

    void Stem::InitBufferData() {
        std::vector<StemVertex> initStemVertex;
        std::vector<StemManage> initStemManege;

        std::vector<StemBasePosition> initStemBasePosition;
        //std::vector<glm::vec3> initStemBasePosition;
        //std::vector<float> initStemBasePosition_Array;

        std::vector<glm::mat4> initStemDebugMatrix;

        for (int i = 0; i < m_FlowerModel->count; i++) {
            //
            for (int vertex = 0; vertex < stemVertexCount;vertex++) {
                initStemVertex.push_back(StemVertex(i));
            }

            //
            StemManage stemManage = StemManage(1,glm::vec2(float(i)*10.0+0.69154f),glm::vec2(777.3591f,float(i)));
            if (stemManage.stemLifeVal == 0.0f || stemManage.stemLifeVal == 1.0f) {
                stemManage.stemWaitTime = mymath::rand(glm::vec2(float(i) + 9.154f, 0.1459f)) * 2.0f + 1.0f; //glm::gaussRand(1.0f, 3.0f);
                stemManage.manageLifeCountFlag = 0;
            }
            initStemManege.push_back(stemManage);
            
            //
            //glm::vec2 initBasePos = mymath::circleRand(stemGrowthRange, glm::vec2(5.1792f, float(i) + 6.66666f), glm::vec2(float(i) + 7.771543f)); //glm::sphericalRand(stemGrowthRange);
            // ひとまず半球状に並べる
            glm::vec2 initBasePos = mymath::hemCircleRand(stemGrowthRange, glm::vec2(5.1792f, float(i) + 6.66666f), glm::vec2(float(i) + 7.771543f)); //glm::sphericalRand(stemGrowthRange);
            
            // offset
            glm::vec2 offvec = 5.0f * glm::normalize(initBasePos);
            //glm::vec2 offvec = 10.0f * glm::normalize(initBasePos);
            initBasePos += offvec;

            //
            initStemBasePosition.push_back(StemBasePosition(initBasePos.x, 0.0f, initBasePos.y,0.0f));
            initStemDebugMatrix.push_back(glm::mat4());
        }
        stemResult_buffer->SetData<std::vector<StemVertex>>(initStemVertex);
        stemVertex_buffer->SetData<std::vector<StemVertex>>(initStemVertex);
        stemManage_buffer->SetData<std::vector<StemManage>>(initStemManege);
        stemBasePosition_buffer->SetData<std::vector<StemBasePosition>>(initStemBasePosition);
    }

    void Stem::Cal_Stem_Result() {
        cal_stem_cs->SetActive();
        cal_stem_cs->SetIntUniform("_contPosArrayLength", bSplineData->controlPoints.size());
        
        std::vector<glm::vec4> contPos;
        for (int i = 0; i < bSplineData->controlPoints.size(); i++) {
            glm::vec3 controlPoint = bSplineData->controlPoints[i];
            
            //contPos.push_back(controlPoint.x);
            //contPos.push_back(stemLength * (mymath::rand(glm::vec2(float(i) + 1.785f, float(i) + 7.1259f)) + 0.5f));
            //contPos.push_back(1.0f);
            //contPos.push_back(controlPoint.z);
            //contPos.push_back(1.0f);
            contPos.push_back(glm::vec4(controlPoint.x, controlPoint.y * 
                stemLength * (mymath::rand(glm::vec2(float(i)+1.785f, float(i) + 7.1259f))+0.5f) /*glm::gaussRand(0.5f, 1.5f) */ , controlPoint.z, 0.0f));
        }
        //cal_stem_cs->SetVec4ArrayUniform("_controlPoints", contPos);

        cal_stem_cs->SetVec4Uniform("_controlPoints0", contPos[0]);
        cal_stem_cs->SetVec4Uniform("_controlPoints1", contPos[1]);
        cal_stem_cs->SetVec4Uniform("_controlPoints2", contPos[2]);
        cal_stem_cs->SetVec4Uniform("_controlPoints3", contPos[3]);
        cal_stem_cs->SetVec4Uniform("_controlPoints4", contPos[4]);
        cal_stem_cs->SetVec4Uniform("_controlPoints5", contPos[5]);

        cal_stem_cs->SetIntUniform("_stemVertexCount", stemVertexCount);
        cal_stem_cs->SetFloatUniform("_knotMin", bSplineData->knotMin);
        cal_stem_cs->SetFloatUniform("_knotMax", bSplineData->knotMax);
        cal_stem_cs->SetFloatUniform("_tWidth", bSplineData->tWidth);

        cal_stem_cs->SetIntUniform("_KernelIndex", stemResult_kernel);
        cal_stem_cs->Dispatch((stemVertexCount * m_FlowerModel->count) / numthreds_val, 1, 1);
        
    }

    void Stem::Init_Stem_Growth() {
        cal_stem_cs->SetActive();
        
        cal_stem_cs->SetIntUniform("_stemVertexCount", stemVertexCount);
        cal_stem_cs->SetFloatUniform("_testLife", 1.0f);
        // 
        cal_stem_cs->SetIntUniform("_KernelIndex", InitStemGrowth_kernel);
        cal_stem_cs->Dispatch((stemVertexCount * m_FlowerModel->count) / numthreds_val, 1, 1);
    }

    void Stem::Cal_Stem_Manage() {
        cal_stem_cs->SetActive();
       
        cal_stem_cs->SetFloatUniform("_stemGrowthRange", stemGrowthRange);
        cal_stem_cs->SetFloatUniform("_DTime", GraphicsMain::GetInstance()->m_DeltaTime);

        cal_stem_cs->SetIntUniform("_KernelIndex", CalStemManage_kernel);
        cal_stem_cs->Dispatch(m_FlowerModel->count / numthreds_val, 1, 1);
    }
    void Stem::Cal_Stem_Growth() {
        cal_stem_cs->SetActive();
        cal_stem_cs->SetIntUniform("_stemVertexCount", stemVertexCount);
        
        cal_stem_cs->SetIntUniform("_KernelIndex", stemGrowth_kernel);
        cal_stem_cs->Dispatch((stemVertexCount * m_FlowerModel->count) / numthreds_val, 1, 1);
    }

    void Stem::Render_Stem() {
        __glewMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

        stem_mat->SetActive();
        m_StemTRS->CalMatrix();
        stem_mat->SetIntUniform("_stemVertexCount", stemVertexCount);
        stem_mat->SetIntUniform("_stemSegments", stemSegments);
        stem_mat->SetFloatUniform("_stemRadius", stemRadius);
        stem_mat->SetFloatUniform("_stemLength", stemLength);

        stem_mat->SetMatrixUniform("MVPMatrix", m_StemTRS->m_pMatrix * m_StemTRS->m_vMatrix * m_StemTRS->m_mMatrix);
        stem_mat->SetMatrixUniform("MMatrix", m_StemTRS->m_mMatrix);
        stem_mat->SetMatrixUniform("VMatrix", m_StemTRS->m_vMatrix);
        stem_mat->SetMatrixUniform("PMatrix", m_StemTRS->m_pMatrix);
        stem_mat->SetFloatUniform("_time", GraphicsMain::GetInstance()->m_SecondsTime);
        stem_mat->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->m_DeltaTime);
        stem_mat->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
        stem_mat->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
        stem_mat->SetVec3Uniform("_LightDir", glm::vec3(1.0, 1.0, 1.0));
        stem_mat->SetIntUniform("_UseLighting", 1);
        
        stem_point_mesh->DrawInstancedWithMesh(stemVertexCount * m_FlowerModel->count, GL_POINTS);
    }
	/////////////

    StemManage::StemManage(int fCount, glm::vec2 seed_lifeval, glm::vec2 seed_signNum) {
        this->stemLifeVal = mymath::rand(seed_lifeval);
        this->stemWaitTime = 0.0f;
        this->signNum = mymath::rand(seed_signNum) * 2.0f - 1.0f;
        this->manageLifeCountFlag = 1;
        this->flowerCount = fCount;
        this->flowerStartIndex = 1;
        this->leafCount = 1;
        this->leafStartIndex = 1;
    }

    StemData::StemData(int i, glm::vec4 p, glm::vec4 t, glm::vec4 n, glm::vec4 b, glm::vec2 seed) :
        resampleIndex(i),
        resampleIndexInStem(-1),
        position{ p.x,p.y,p.z,p.w },
        tangent{ t.x,t.y,t.z,t.w },
        normal{ n.x,n.y,n.z ,n.w },
        bioNormal{ b.x,b.y,b.z ,b.w },
        renderFlag(0),
        lifeTime(0.0f),
        flowerSize(mymath::rand(seed) * 2.0f + 2.0f)
    {
    }
}