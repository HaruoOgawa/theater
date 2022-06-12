#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

class TransformComponent;

namespace animation {
	class Pose
	{
	public:
		Pose();
		Pose(const Pose& p);
		Pose(unsigned int numJoints);
		Pose& operator=(const Pose& p);

		void Resize(unsigned int size);
		unsigned int Size()const;

		int GetParent(unsigned int index)const;
		void SetParent(unsigned int index,int parent);

		const std::shared_ptr<TransformComponent> GetLocalTransform(unsigned int index)const;
		void SetLocalTransform(unsigned int index, const std::shared_ptr<TransformComponent>& transform);

		std::shared_ptr<TransformComponent> GetGloabalTransform(unsigned int index)const;
		const std::shared_ptr<TransformComponent>& operator[](unsigned int index)const;

		void GetMatrixPalette(std::vector<glm::mat4>& out);

		bool operator==(const Pose& other);
		bool operator!=(const Pose& other);
	protected:
		std::vector<std::shared_ptr<TransformComponent>> m_Joints;
		std::vector<int> m_Parents;
	};
}