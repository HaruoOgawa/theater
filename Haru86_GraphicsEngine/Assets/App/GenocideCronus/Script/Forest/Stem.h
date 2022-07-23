#pragma once
#include <memory>
#include <glm/glm.hpp>

class ComputeBuffer;
class Material;
class Mesh;
class TransformComponent;

namespace myapp {
	class FlowerModel;
    struct BSplineData;

    //stem本体の情報
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

    //花の数など
    struct StemManage {
        float stemLifeVal;
        float stemWaitTime;
        float signNum;
        int manageLifeCountFlag;
        int flowerCount;
        int flowerStartIndex;
        int leafCount;
        int leafStartIndex;
        StemManage(int fCount, glm::vec2 seed_lifeval, glm::vec2 seed_signNum);
    };

    //花や茎を生成するための情報を載せる構造体
    struct StemData {
        int resampleIndex;
        int resampleIndexInStem;
        float position[3];
        float tangent[3];
        float normal[3];
        float bioNormal[3];
        int renderFlag;
        float lifeTime;
        float flowerSize;
        StemData(int i, glm::vec3 p, glm::vec3 t, glm::vec3 n, glm::vec3 b, glm::vec2 seed);
    };

    // 各花の生える基本の座標
    struct StemBasePosition {
        float position[4];
        //float position[3];

        StemBasePosition(float x, float y, float z,float w) :
            position{x,y,z,w}
        {
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