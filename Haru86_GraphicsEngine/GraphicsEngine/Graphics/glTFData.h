#pragma once

#include <string>
#include <memory>
#include <vector>

#include "tiny_gltf.h"
#include "GraphicsEngine/Animation/Pose.h"
#include "GraphicsEngine/Animation/CTrack.h"
#include "GraphicsEngine/Animation/Cframe.h"
#include "GraphicsEngine/Animation/Clip.h"
#include "GraphicsEngine/Animation/Skeleton.h"
#include "GraphicsEngine/Animation/EInterpolationType.h"

class Object;
class Mesh;
class TransformComponent;

namespace animation {
	class AnimatorComponent;
}

namespace gltf {
	class glTFData
	{
		tinygltf::Model m_Model;

		std::shared_ptr<Mesh> m_mesh;
		
		std::shared_ptr<animation::AnimatorComponent> m_Animator;
		std::shared_ptr<animation::Skeleton> m_Skeleton;
		std::vector<std::shared_ptr<animation::Clip>> m_AnimationClipList;
	public:
		glTFData();
		~glTFData();

		//
		bool Load(Object* obj, const std::string& gltfDataPath);
		
		//
		std::shared_ptr<Mesh> GetMesh()const;
		std::shared_ptr<animation::Skeleton> GetSkeleton()const;
	private:
		bool LoadNode(Object* obj);
		std::shared_ptr<Mesh> LoadMesh(int MeshIndex);

		bool LoadAnimation(Object* obj);
		bool LoadSkeleton(Object* obj);
		std::shared_ptr<animation::Pose> LoadRestPose();
		std::shared_ptr<animation::Pose> LoadBindPose();
		std::vector<std::string> LoadJointNames()const;
		bool LoadAnimationClips(Object* obj);
		
		// ヘルパー関数群
		int GetNodeIndex(tinygltf::Node* target, tinygltf::Node* allNodes, unsigned int numNodes);
		std::shared_ptr<TransformComponent> GetLocalTransform(const tinygltf::Node& node);
		void GetScalarValues(std::vector<float>& out, unsigned int compCount, const tinygltf::Accessor& inAccessor);

		template<typename T, int N>
		void TrackFromChannel(std::shared_ptr<animation::CTrack<T, N>>& result, const tinygltf::AnimationChannel& channel,const std::vector<tinygltf::AnimationSampler>& SamplerList) {
			//
			const auto& sampler = SamplerList[channel.sampler];

			// 補完タイプを取得
			animation::EInterpolationType InterpolationType = animation::EInterpolationType::Constant;

			if (sampler.interpolation == "LINEAR") {
				InterpolationType = animation::EInterpolationType::Linear;
			}
			else if (sampler.interpolation == "CUBICSPLINE") {
				InterpolationType = animation::EInterpolationType::Cubic;
			}

			bool isSamplerCubic = (InterpolationType == animation::EInterpolationType::Cubic);

			//
			result->SetInterpolationType(InterpolationType);

			//
			std::vector<float> time;
			GetScalarValues(time, 1, m_Model.accessors[sampler.input]);

			//
			std::vector<float> val;
			GetScalarValues(val, N, m_Model.accessors[sampler.output]);

			//
			unsigned int numFrames = m_Model.accessors[sampler.input].count;
			unsigned int compCount = val.size() / time.size();
			result->Resize(numFrames);

			for (unsigned int i = 0; i < numFrames;i++) {
				int baseIndex = i * compCount;
				std::shared_ptr<animation::CFrame<N>>& frame = result->GetFrame(i);
				int offset = 0;

				//
				frame->m_Time = time[i];

				for (int comp = 0; comp < N;comp++) {
					frame->m_In[comp] = (isSamplerCubic) ? val[baseIndex + offset++] : 0.0f;
				}

				for (int comp = 0; comp < N; comp++) {
					frame->m_Value[comp] = val[baseIndex + offset++];
				}

				for (int comp = 0; comp < N; comp++) {
					frame->m_Out[comp] = (isSamplerCubic) ? val[baseIndex + offset++] : 0.0f;
				}
			}
		}
	};
}