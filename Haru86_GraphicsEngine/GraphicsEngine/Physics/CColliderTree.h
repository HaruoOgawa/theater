#pragma once
#include <vector>
#include <memory>
#include "EColliderType.h"
#include "EObjectType.h"
#include <glm/glm.hpp>
#include <unordered_map>
class GameObject;
class Primitive;

namespace physics {
	class ACollider;
	class CColliderCell;
	class CCellContents;

	class CColliderTree
	{
		//
		std::vector<std::shared_ptr<ACollider>> m_ColliderList;
		std::vector<std::shared_ptr<CCellContents>> m_CellContentsList;

		//
		int MAXCELLLEVEL;

		float m_TreeMin;
		float m_TreeMax;

		float m_CellSize;
		int m_CellNum;
		std::vector<int> m_CellNumPerLayer;

		std::vector<int> m_CellCountPerLevel;
		std::vector<std::shared_ptr<CColliderCell>> m_CellList;
		
	public:
		CColliderTree();
		virtual ~CColliderTree();

		bool AddCollider(EColliderType ColliderType, EObjectType ObjectType, GameObject* Obj);
		bool MakeColliderTree();

		std::vector<std::shared_ptr<ACollider>> GetPassiveColliderListFromTree(std::shared_ptr<ACollider> Collider);
		
		//
		bool GetCollisionAllMap(std::unordered_map<ACollider*, ACollider*>& CollisionMap);
		
		const std::vector<std::shared_ptr<ACollider>>& GetColliderList()const;

		//
		std::vector<std::shared_ptr<CCellContents>>& GetCellContentsList();

		//
		bool RegistCellContents2Cell(const std::shared_ptr<CCellContents>& CellContents);
	private:
		bool CreateSphereCollider(const std::vector<std::shared_ptr<Primitive>>& PrimitiveList, GameObject* Obj, EObjectType ObjectType, EColliderType ColliderType);
		bool CreateSimpleBoxCollider(const std::vector<std::shared_ptr<Primitive>>& PrimitiveList, GameObject* Obj, EObjectType ObjectType, EColliderType ColliderType);
		bool CreateBoxCollider(const std::vector<std::shared_ptr<Primitive>>& PrimitiveList, GameObject* Obj, EObjectType ObjectType, EColliderType ColliderType);

		bool CalculateAABB(const std::vector<float>& Vertices,glm::vec3& CenterPos, glm::vec3& MaxPos, glm::vec3& MinPos);

		bool CalculateSphereVolumeFromAABB(const std::vector<float>& Vertices, glm::vec3& CenterPos , float& Radius);
		bool OptimizeSphereVolume();

		bool CalculateOBB();
		bool OptimizeOBB();

		//
		bool MakeTree();
		
		//
		unsigned int GetCellIndexFromMortonNumber(const glm::vec3& MinPos, const glm::vec3& MaxPos)const;
		unsigned int GetMortonNumber(unsigned int x, unsigned int y, unsigned int z)const;
		unsigned int SeparateBitForMorton(unsigned int val)const;

		//
		bool GetCollisionMap(unsigned int CellIndex, std::unordered_map<ACollider*, ACollider*>& CollisionList, std::vector<ACollider*>& ColliderStack);

		//
		bool CreateCell(unsigned int CreateCellIndex);
	};
}



