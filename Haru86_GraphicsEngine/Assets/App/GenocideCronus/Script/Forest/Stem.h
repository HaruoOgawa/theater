#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

class ComputeBuffer;
class Material;
class Mesh;
class TransformComponent;

namespace myapp {
	class FlowerModel;
    struct BSplineData;

    //stem–{‘Ì‚Ìî•ñ
    struct StemVertex {
        float vertice[3];
        float tangent[3];
        float normal[3];
        float bioNormal[3];
        int index;

        StemVertex(int i):
            vertice{ 0.0f, 0.0f, 0.0f },
            tangent{ 0.0f, 0.0f, 0.0f },
            normal{ 0.0f, 0.0f, 0.0f },
            bioNormal{ 0.0f, 0.0f, 0.0f },
            index(i)
        {
        }
    };

    //‰Ô‚Ì”‚È‚Ç
    struct StemManage {
        float stemLifeVal;
        float stemWaitTime;
        float signNum;
        int manageLifeCountFlag;
        int flowerCount;
        int flowerStartIndex;
        int leafCount;
        int leafStartIndex;
        StemManage(int fCount) {
            this->stemLifeVal = glm::gaussRand(0.0f, 1.0f);
            this->stemWaitTime = 0.0f;
            this->signNum = glm::gaussRand(-1.0f, 1.0f);
            this->manageLifeCountFlag = 1;
            this->flowerCount = fCount;
            this->flowerStartIndex = 1;
            this->leafCount = 1;
            this->leafStartIndex = 1;
        }
    };

    //‰Ô‚âŒs‚ð¶¬‚·‚é‚½‚ß‚Ìî•ñ‚ðÚ‚¹‚é\‘¢‘Ì
    struct StemData {
        int resampleIndex;
        int resampleIndexInStem;
        glm::vec3 position;
        glm::vec3 tangent;
        glm::vec3 normal;
        glm::vec3 bioNormal;
        int renderFlag;
        float lifeTime;
        float flowerSize;
        StemData(int i, glm::vec3 p, glm::vec3 t, glm::vec3 n, glm::vec3 b) {
            this->resampleIndex = i;
            this->resampleIndexInStem = -1;
            this->position = p;
            this->tangent = t;
            this->normal = n;
            this->bioNormal = b;
            this->renderFlag = 0;
            this->lifeTime = 0.0f;
            this->flowerSize = glm::gaussRand(2.0f, 4.0f);
        }
    };

	class Stem
	{
        // Base Param
		FlowerModel* m_FlowerModel;
        int stemVertexCount;
        std::shared_ptr<BSplineData> bSplineData;
        float stemRadius;
        int stemSegments;
        float stemLength;
        float stemGrowthRange;
        int numthreds_val;
        std::shared_ptr<TransformComponent> m_StemTRS;

        // kernel(ComputeBuffer Uniform)
        int stemResult_kernel;
        int InitStemGrowth_kernel;
        int CalStemManage_kernel;
        int stemGrowth_kernel;

        // Compute Buffer
 		std::shared_ptr<ComputeBuffer> stemResult_buffer;
		std::shared_ptr<ComputeBuffer> stemVertex_buffer;
		std::shared_ptr<ComputeBuffer> stemManage_buffer;
		std::shared_ptr<ComputeBuffer> stemBasePosition_buffer;

        // Compute Buffer Index
        int stemResult_buffer_index;
        int stemVertex_buffer_index;
        int stemManage_buffer_index;
        int stemBasePosition_buffer_index;

        //
        std::shared_ptr<Material> cal_stem_cs;
        std::shared_ptr<Material> stem_mat;
        std::shared_ptr<Mesh> stem_point_mesh;

	public:
		Stem(FlowerModel* model);
		~Stem() = default;
		void Start();
		void Update();
		void Draw();

    private:
        void Init();
        void InitBufferData();
        void Cal_Stem_Result();
        void Init_Stem_Growth();
        void Cal_Stem_Manage();
        void Cal_Stem_Growth();
        void Render_Stem();
	};
}