#include "glTFData.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Object/Object.h"
#include "GraphicsEngine/Object/CNode.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Primitive.h"
#include <memory>
#include <cstring>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicsEngine/Animation/TransformTrack.h"

#include "glTFMesh.h"
#include "GraphicsEngine/Component/SkinMeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Animation/AnimatorComponent.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

namespace gltf {
	glTFData::glTFData() :
		m_mesh(nullptr),
		m_Animator(nullptr),
		m_Skeleton(nullptr)
	{
	}

	glTFData::~glTFData() 
	{
	}

	//
	bool glTFData::Load(Object* obj, const std::string& gltfDataPath) {
		// glTFのロード 
		tinygltf::TinyGLTF loader;
		
		std::string err;
		std::string warn;
		bool result = loader.LoadASCIIFromFile(&m_Model, &err, &warn, gltfDataPath);

		// 
		if (!warn.empty()) {
			Console::Log("warn:%s \n", warn.c_str());
			return false;
		}

		if (!err.empty()) {
			Console::Log("err:%s \n", err.c_str());
			return false;
		}

		// 
		if (!result) {
			Console::Log("glTF Load Error...");
			return false;
		}

		// Animatorを生成する(ルートノードのみにアタッチ)
		m_Animator = std::make_shared<animation::AnimatorComponent>(obj);
		obj->GetRootNode()->AddComponent<animation::AnimatorComponent>(m_Animator);

		// 各種データのロード
		if (!LoadNode(obj))return false;
		if (!LoadAnimation(obj))return false;
		
		return true;
	}

	bool glTFData::LoadNode(Object* obj) {
		unsigned int NodeNum = m_Model.nodes.size();

		// GameObjectのノードを作成
		std::vector<std::shared_ptr<obj::CNode>> NodeList;
		for (unsigned int NodeIndex = 0; NodeIndex < NodeNum; NodeIndex++) {
			NodeList.push_back(std::make_shared<obj::CNode>());
		}

		// 親子関係とMeshの登録
		for (unsigned int NodeIndex = 0; NodeIndex < NodeNum;NodeIndex++) {
			// glTFのノード
			const auto& glTFNode = m_Model.nodes[NodeIndex];
			Console::Log("NodeIndex: %d/ glTFNode.name: %s\n", NodeIndex, glTFNode.name.c_str());
			// GameObjectのノード
			auto& GameObjNode = NodeList[NodeIndex];

			// TransformをNodeが持つものに変更
			auto NodeTransform = GetLocalTransform(glTFNode);
			GameObjNode->GetTransform()->SetPosition(NodeTransform->GetPosition());
			GameObjNode->GetTransform()->SetRotation(NodeTransform->GetRotation());
			GameObjNode->GetTransform()->SetScale(NodeTransform->GetScale());
			GameObjNode->GetTransform()->CalMatrix();

			// Meshの作成
			int MeshIndex = glTFNode.mesh;
			if (MeshIndex != -1) {
				const auto Mesh = LoadMesh(MeshIndex);
				std::shared_ptr<SkinMeshRendererComponent> SkinMeshRenderer = std::make_shared<SkinMeshRendererComponent>(obj,"",Mesh,m_Animator, MeshIndex,glTFNode.name);
				GameObjNode->AddComponent<SkinMeshRendererComponent>(SkinMeshRenderer);
			}

			// 親子関係の作成
			// 各子要素を現在のノード(親要素)に割り当てる
			for (const auto child : glTFNode.children) {
				GameObjNode->AddNode(NodeList[child]);
				NodeList[child]->SetParentIndex(NodeIndex);
			}
			// 上記の処理を経てもなおノードが親要素も持っていないのならglTFのルートノードをGameObjectのルートノードに割り当てる
			if (GameObjNode->GetParentIndex() == -1) {
				obj->GetRootNode()->AddNode(GameObjNode);
			}
		}

		return true;
	}

	std::shared_ptr<Mesh> glTFData::LoadMesh(int MeshIndex) {
		std::shared_ptr<Mesh> GameObjMesh = std::make_shared<Mesh>();

		const auto& mesh = m_Model.meshes[MeshIndex];
		auto index = 0;
		for (auto prim : mesh.primitives) {

			//get vertices
			std::unique_ptr<glTFMesh> MeshData = std::make_unique<glTFMesh>();

			//vertex
			auto AccessorsIndex = prim.attributes["POSITION"];
			auto BufferViewIndex = m_Model.accessors[AccessorsIndex].bufferView;
			auto ByteLength = m_Model.bufferViews[BufferViewIndex].byteLength;
			auto ByteOffset = m_Model.bufferViews[BufferViewIndex].byteOffset;

			MeshData->vertex = std::vector<float>(ByteLength / 4, 0.0);
			MeshData->subDataVertex = std::vector<unsigned char>(ByteLength, 0);

			std::copy(
				m_Model.buffers[0].data.begin() + ByteLength * 0 + ByteOffset,
				m_Model.buffers[0].data.begin() + ByteLength * 1 - 1 + ByteOffset,
				MeshData->subDataVertex.begin()
			);
			std::memcpy(MeshData->vertex.data(), MeshData->subDataVertex.data(), sizeof(float) * MeshData->vertex.size());

			MeshData->subDataVertex.clear();

			//normal
			AccessorsIndex = prim.attributes["NORMAL"];
			BufferViewIndex = m_Model.accessors[AccessorsIndex].bufferView;
			ByteLength = m_Model.bufferViews[BufferViewIndex].byteLength;
			ByteOffset = m_Model.bufferViews[BufferViewIndex].byteOffset;

			MeshData->normal = std::vector<float>(ByteLength / 4, 0.0f);
			MeshData->subDataNormal = std::vector<unsigned char>(ByteLength, 0);
			std::copy(
				m_Model.buffers[0].data.begin() + ByteLength * 0 + ByteOffset,
				m_Model.buffers[0].data.begin() + ByteLength * 1 - 1 + ByteOffset,
				MeshData->subDataNormal.begin()
			);
			std::memcpy(MeshData->normal.data(), MeshData->subDataNormal.data(), sizeof(float) * MeshData->normal.size());

			MeshData->subDataNormal.clear();

			//texcoord
			AccessorsIndex = prim.attributes["TEXCOORD_0"];
			BufferViewIndex = m_Model.accessors[AccessorsIndex].bufferView;
			ByteLength = m_Model.bufferViews[BufferViewIndex].byteLength;
			ByteOffset = m_Model.bufferViews[BufferViewIndex].byteOffset;

			MeshData->texcoord = std::vector<float>(ByteLength / 4, 0.0f);
			MeshData->subDataTexcoord = std::vector<unsigned char>(ByteLength, 0);
			std::copy(
				m_Model.buffers[0].data.begin() + ByteLength * 0 + ByteOffset,
				m_Model.buffers[0].data.begin() + ByteLength * 1 - 1 + ByteOffset,
				MeshData->subDataTexcoord.begin()
			);
			std::memcpy(MeshData->texcoord.data(), MeshData->subDataTexcoord.data(), sizeof(float) * MeshData->texcoord.size());

			MeshData->subDataTexcoord.clear();

			// weight
			AccessorsIndex = prim.attributes["WEIGHTS_0"];
			BufferViewIndex = m_Model.accessors[AccessorsIndex].bufferView;
			ByteLength = m_Model.bufferViews[BufferViewIndex].byteLength;
			ByteOffset = m_Model.bufferViews[BufferViewIndex].byteOffset;

			MeshData->weights = std::vector<float>(ByteLength / 4, 0.0f);
			MeshData->subDataWeights = std::vector<unsigned char>(ByteLength, 0);

			std::copy(
				m_Model.buffers[0].data.begin() + ByteLength * 0 + ByteOffset,
				m_Model.buffers[0].data.begin() + ByteLength * 1 - 1 + ByteOffset,
				MeshData->subDataWeights.begin()
			);
			std::memcpy(MeshData->weights.data(), MeshData->subDataWeights.data(), sizeof(float) * MeshData->weights.size());

			MeshData->subDataTexcoord.clear();

			// joint
			AccessorsIndex = prim.attributes["JOINTS_0"];
			BufferViewIndex = m_Model.accessors[AccessorsIndex].bufferView;
			ByteLength = m_Model.bufferViews[BufferViewIndex].byteLength;
			ByteOffset = m_Model.bufferViews[BufferViewIndex].byteOffset;

			MeshData->joints = std::vector<int>(ByteLength / 4, 0.0f);
			MeshData->subDataJoints = std::vector<unsigned char>(ByteLength, 0);

			std::copy(
				m_Model.buffers[0].data.begin() + ByteLength * 0 + ByteOffset,
				m_Model.buffers[0].data.begin() + ByteLength * 1 - 1 + ByteOffset,
				MeshData->subDataJoints.begin()
			);
			std::memcpy(MeshData->joints.data(), MeshData->subDataJoints.data(), sizeof(int) * MeshData->joints.size());

			std::vector<float> joints_float(MeshData->joints.begin(), MeshData->joints.end());
			MeshData->joints.clear();

			//set vertices data
			std::vector<std::vector<float>> vertices;

			vertices.push_back(MeshData->vertex);
			MeshData->vertex.clear();

			vertices.push_back(MeshData->normal);
			MeshData->normal.clear();

			vertices.push_back(MeshData->texcoord);
			MeshData->texcoord.clear();

			vertices.push_back(MeshData->weights);
			MeshData->weights.clear();

			vertices.push_back(joints_float);
			joints_float.clear();

			// 各要素の次元
			std::vector<int> dimention;
			dimention.push_back(3);
			dimention.push_back(3);
			dimention.push_back(2);
			dimention.push_back(4);
			dimention.push_back(4);

			////get indices///////////////////////////////////

			AccessorsIndex = prim.indices;
			BufferViewIndex = m_Model.accessors[AccessorsIndex].bufferView;
			ByteLength = m_Model.bufferViews[BufferViewIndex].byteLength;
			ByteOffset = m_Model.bufferViews[BufferViewIndex].byteOffset;

			MeshData->indices = std::vector<unsigned int>(ByteLength / 4, 0);
			MeshData->subDataIndices = std::vector<unsigned char>(ByteLength, 0);

			std::copy(
				m_Model.buffers[0].data.begin() + ByteLength * 0 + ByteOffset,
				m_Model.buffers[0].data.begin() + ByteLength * 1 - 1 + ByteOffset,
				MeshData->subDataIndices.begin()
			);
			std::memcpy(MeshData->indices.data(), MeshData->subDataIndices.data(), sizeof(unsigned int) * MeshData->indices.size());
			MeshData->subDataIndices.clear();

			MeshData->mainDataIndices = std::vector<unsigned short>(MeshData->indices.begin(), MeshData->indices.end());

			//この辺でPrimitiveにデータを渡す
			std::shared_ptr<Primitive> m_prim = std::make_shared<Primitive>(vertices, dimention, MeshData->mainDataIndices);
			GameObjMesh->SetPrimitive(m_prim);

			//すべてのメモリを一応解放
			vertices.clear();
			dimention.clear();
			MeshData->indices.clear();
			MeshData->mainDataIndices.clear();

			index++;
		}

		return GameObjMesh;
	}

	bool glTFData::LoadAnimation(Object* obj) {
		if (!LoadSkeleton(obj))return false;
		if (!LoadAnimationClips(obj))return false;

		//
		m_Animator->Set(m_Skeleton, m_AnimationClipList);

		return true;
	}

	bool glTFData::LoadSkeleton(Object* obj) {
		m_Skeleton = std::make_shared<animation::Skeleton>(
			LoadRestPose(),
			LoadBindPose(),
			LoadJointNames()
			);

		return true;
	}

	std::shared_ptr<animation::Pose> glTFData::LoadRestPose() {
		unsigned int nodeCount = static_cast<unsigned int>(m_Model.nodes.size());
		unsigned int boneCount = static_cast<unsigned int>(m_Model.nodes.size());
		std::shared_ptr<animation::Pose> result = std::make_shared<animation::Pose>(boneCount);

		for (unsigned int i = 0; i < boneCount; i++) {
			tinygltf::Node node = m_Model.nodes[i];

			result->SetLocalTransform(i, GetLocalTransform(node));

			//int parent = GetNodeIndex(node.,&m_Model.nodes[0],boneCount);
			// tinygltfのNodeがparentメンバを持たないため直接Children側に潜って自身のインデックスを渡しに行く
			int parent = i;
			for (const auto& child : node.children) {
				result->SetParent(child, parent);
			}

		}

		return result;
	}

	std::shared_ptr<animation::Pose> glTFData::LoadBindPose() {
		std::shared_ptr<animation::Pose> restPose = LoadRestPose();
		unsigned int numBones = restPose->Size();
		std::vector<std::shared_ptr<TransformComponent>> worldBindPose(numBones);

		for (unsigned int i = 0; i < numBones; i++) {
			worldBindPose[i] = restPose->GetGloabalTransform(i);
		}

		// 逆バインドポーズ行列の取得
		unsigned int numSkins = m_Model.skins.size();
		for (unsigned int i = 0; i < numSkins; i++) {
			tinygltf::Skin* skin = &m_Model.skins[i];
			std::vector<float> invBindAccessor;
			GetScalarValues(invBindAccessor, 16, m_Model.accessors[skin->inverseBindMatrices]);

			//
			unsigned int numJoints = skin->joints.size();
			for (int j = 0; j < numJoints; j++) {
				//
				float* matrix = &(invBindAccessor[j * 16]);
				glm::mat4 invBindMatrix = glm::make_mat4(matrix);

				//
				glm::mat4 bindMatrix = glm::inverse(invBindMatrix);
				std::shared_ptr<TransformComponent> bindTransform = std::make_shared<TransformComponent>(nullptr, bindMatrix);

				//
				tinygltf::Node* jointNode = &m_Model.nodes[skin->joints[j]];
				//int jointIndex = GetNodeIndex(jointNode, &m_Model.nodes[0], numBones);
				int jointIndex = skin->joints[j];
				worldBindPose[jointIndex] = bindTransform;
			}
		}

		// スキンメッシュのための計算
		std::shared_ptr<animation::Pose> bindPose = restPose;
		for (unsigned int i = 0; i < numBones; i++) {
			std::shared_ptr<TransformComponent> current = worldBindPose[i];
			int p = bindPose->GetParent(i);
			if (p >= 0) {
				std::shared_ptr<TransformComponent> parent = worldBindPose[p];
				current = TransformComponent::Combine(TransformComponent::inverse(parent), current);
			}
			bindPose->SetLocalTransform(i, current);
		}

		return bindPose;
	}

	std::vector<std::string> glTFData::LoadJointNames()const {
		unsigned int boneCount = static_cast<unsigned int>(m_Model.nodes.size());
		std::vector<std::string> result(boneCount, "Not Set");

		for (unsigned int i = 0; i < boneCount; i++) {
			tinygltf::Node node = m_Model.nodes[i];

			if (node.name.empty()) {
				result[i] = "EMPTY NODE";
			}
			else {
				result[i] = node.name;
			}
		}

		return result;
	}

	int glTFData::GetNodeIndex(tinygltf::Node* target, tinygltf::Node* allNodes, unsigned int numNodes) {
		if (target==0 || target==nullptr) {
			return -1;
		}

		for (unsigned int i = 0; i < numNodes;i++) {
			if (target==&allNodes[i]) {
				return (int)i;
			}
		}

		return -1;
	}

	std::shared_ptr<TransformComponent> glTFData::GetLocalTransform(const tinygltf::Node& node) {
		std::shared_ptr<TransformComponent> result = std::make_shared<TransformComponent>();

		// mmatrix
		/*if (node.matrix.size() == 16) {
			glm::mat4 mmatrix = glm::mat4();
			for (unsigned int n = 0; n < node.matrix.size();n++) {
				const auto& val = node.matrix[n];
				unsigned int firstIndex = static_cast<unsigned int>(n / 4.0); // 0 to 3
				int secondIndex = static_cast<unsigned int>(n - firstIndex * 4.0);

				mmatrix[firstIndex][secondIndex] = val;
			}

		}*/

		// position
		if (node.translation.size() > 0) {
			result->SetPosition(glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
		}

		// rotation
		if (node.rotation.size() > 0) {
			result->SetRotation(glm::quat(glm::vec3(node.rotation[0], node.rotation[1], node.rotation[2])));
		}

		// scale
		if (node.scale.size() > 0) {
			result->SetScale(glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
		}

		result->CalMatrix();

		return result;
	}

	void glTFData::GetScalarValues(std::vector<float>& out, unsigned int compCount, const tinygltf::Accessor& inAccessor) {
		out.resize(inAccessor.count * compCount);
		
		const auto& BufferView = m_Model.bufferViews[inAccessor.bufferView];
		int BufferIndex= BufferView.buffer;
		auto ByteOffset = BufferView.byteOffset;
		auto ByteLength = BufferView.byteLength;

		std::memcpy(&out[0], &m_Model.buffers[BufferIndex].data[0]+ ByteOffset,  ByteLength);
	}

	bool glTFData::LoadAnimationClips(Object* obj) {
		//
		unsigned int numClips = m_Model.animations.size();
		unsigned int numNodes = m_Model.nodes.size();

		m_AnimationClipList.resize(numClips);

		//
		for (unsigned int i = 0; i < numClips;i++) {
			m_AnimationClipList[i] = std::make_shared<animation::Clip>();
			m_AnimationClipList[i]->SetName(m_Model.animations[i].name);

			unsigned int numChannels = m_Model.animations[i].channels.size();

			for (unsigned int j = 0; j < numChannels;j++) {
				tinygltf::AnimationChannel& channel = m_Model.animations[i].channels[j];

				tinygltf::Node* target = &m_Model.nodes[channel.target_node];

				//int nodeId = channel.target_node;
				int nodeId = GetNodeIndex(target,&m_Model.nodes[0],numNodes);

				if (channel.target_path=="translation") {
					std::shared_ptr<animation::VectorTrack> track = m_AnimationClipList[i]->GetTrack(nodeId)->GetPositionTrack();
					TrackFromChannel<glm::vec3,3>(track, channel,m_Model.animations[i].samplers);
				}else if (channel.target_path == "scale") {
					std::shared_ptr<animation::VectorTrack> track = m_AnimationClipList[i]->GetTrack(nodeId)->GetScaleTrack();
					TrackFromChannel<glm::vec3, 3>(track, channel, m_Model.animations[i].samplers);
				}else if (channel.target_path == "rotation") {
					std::shared_ptr<animation::QuaternionTrack> track = m_AnimationClipList[i]->GetTrack(nodeId)->GetRotationTrack();
					TrackFromChannel<glm::quat, 4>(track, channel, m_Model.animations[i].samplers);
				}
			}

			//
			m_AnimationClipList[i]->RecalculateDuration();
		}

		//
		return true;
	}
	//
	std::shared_ptr<Mesh> glTFData::GetMesh()const {
		return m_mesh;
	}

	std::shared_ptr<animation::Skeleton> glTFData::GetSkeleton()const {
		return m_Skeleton;
	}
}
