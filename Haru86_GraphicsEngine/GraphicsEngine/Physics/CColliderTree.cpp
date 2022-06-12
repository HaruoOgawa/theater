#include "CColliderTree.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/Primitive.h"
#include "CSphereCollider.h"
#include "CBoxCollider.h"
#include "CColliderCell.h"
#include "CCellContents.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "../Message/Console.h"
#include "GraphicsEngine/Object/CNode.h"

namespace physics {
	CColliderTree::CColliderTree():
		MAXCELLLEVEL(3),
		m_TreeMin(-50.0f),
		m_TreeMax(50.0f),
		m_CellNum(0)
	{
		int SideCellNum = glm::pow(2.0f, static_cast<float>(MAXCELLLEVEL));
		m_CellSize = (m_TreeMax - m_TreeMin) / SideCellNum;
	}

	CColliderTree::~CColliderTree() {

	}

	bool CColliderTree::AddCollider(EColliderType ColliderType, EObjectType ObjectType, GameObject* Obj) {
		const auto& PrimList = Obj->GetRootNode()->GetComponent<MeshRendererComponent>()->GetMesh()->GetPrimitiveList();
		
		switch (ColliderType)
		{
			case physics::EColliderType::Sphere:
				if (!CreateSphereCollider(PrimList, Obj, ObjectType, ColliderType))return false;
				break;

			case physics::EColliderType::Box:
				if (!CreateSimpleBoxCollider(PrimList, Obj, ObjectType, ColliderType))return false;
				break;

			default:
				break;
		}

		for (auto& Prim :PrimList) {
			Prim->Release();
		}

		return true;
	}

	bool CColliderTree::MakeColliderTree() {
		// ツリーの構築
		if (!MakeTree())return false;

		// ツリーの初期化(最初のオブジェクト登録)
		for (const auto& CellContents : m_CellContentsList) {
			RegistCellContents2Cell(CellContents);
		}

		return true;
	}

	bool CColliderTree::MakeTree() {
		// セルを作成
		int CellNum = 0;
		for (int n = 0; n < MAXCELLLEVEL+1;n++) {
			m_CellNumPerLayer.push_back(CellNum);
			CellNum += glm::pow(8.0f, n);
		}

		m_CellList.resize(CellNum);

		// ルートセルを生成
		if (!CreateCell(0))return false;

		m_CellNum = CellNum;
		return true;
	}

	//
	bool CColliderTree::CreateCell(unsigned int CreateCellIndex) {
		//
		m_CellList[CreateCellIndex]= std::make_shared<CColliderCell>();

		// 親要素が存在するか確認する。なければ親要素も生成する
		unsigned int ParentCellIndex = static_cast<unsigned int>((CreateCellIndex - 1) / 8);
		if (ParentCellIndex < m_CellNum && !m_CellList[ParentCellIndex]) {
			CreateCell(ParentCellIndex);
		}

		return true;
	}

	bool CColliderTree::RegistCellContents2Cell(const std::shared_ptr<CCellContents>& CellContents) {
		//
		const auto& Collider = CellContents->GetCollider();
		const auto& Bounds = Collider->GetBounds();
		const auto WorldMatrix = Collider->GetTransform()->GetMMatrix();
		glm::vec3 WorldMinPos = WorldMatrix * glm::vec4(Bounds->m_MinPos, 1.0f);
		glm::vec3 WorldMaxPos = WorldMatrix * glm::vec4(Bounds->m_MaxPos, 1.0f);

		// モートン番号からコライダーを格納するセルのインデックスを取得する
		unsigned int CellIndex = GetCellIndexFromMortonNumber(WorldMinPos, WorldMaxPos);
		
		if (CellIndex < m_CellList.size()) {
			// もしセルがまだ空なら生成する
			if (!m_CellList[CellIndex]) {
				if (!CreateCell(CellIndex))return false;
			}

			// セルにコライダー(セルコンテンツ)を登録する
			m_CellList[CellIndex]->SetCellContents(CellContents);
			
			return true;
		}
		else {
			return false;
		}
	}

	//
	unsigned int CColliderTree::GetCellIndexFromMortonNumber(const glm::vec3& MinPos, const glm::vec3& MaxPos)const {
		unsigned int MinMortonNum = GetMortonNumber(
			static_cast<unsigned int>((MinPos.x - m_TreeMin) / m_CellSize),
			static_cast<unsigned int>((MinPos.y - m_TreeMin) / m_CellSize),
			static_cast<unsigned int>((MinPos.z - m_TreeMin) / m_CellSize)
		);
		
		unsigned int MaxMortonNum = GetMortonNumber(
			static_cast<unsigned int>((MaxPos.x - m_TreeMin) / m_CellSize),
			static_cast<unsigned int>((MaxPos.y - m_TreeMin) / m_CellSize),
			static_cast<unsigned int>((MaxPos.z - m_TreeMin) / m_CellSize)
		);

		// モートン番号が同じだとxor演算で0になってしまう。0になるということはそのセルにすっぽりハマっているということなのでインデックスをそのまま渡す
		unsigned int ParentMorton = (MinMortonNum!= MaxMortonNum)? MinMortonNum ^ MaxMortonNum: MinMortonNum;
		
		unsigned int CellIndex = ParentMorton;
		int L = 0;

		// 親階層までのビットシフト数を計算する
		for (int n = 0; n < MAXCELLLEVEL; n++) {
			unsigned int Check = (ParentMorton >> (3 * n)) & 0x7;
			if (Check != 0)L=n;
		}

		//  セル全体でのインデックスを取得する
		CellIndex = (MaxMortonNum >> 3 * L);
		unsigned int AddNum = (m_CellNumPerLayer[MAXCELLLEVEL - L] - 1)/7;
		CellIndex += AddNum;

		// 
		if (CellIndex > static_cast<unsigned int>(m_CellNum))CellIndex = 0;

		return CellIndex;
	}

	unsigned int CColliderTree::GetMortonNumber(unsigned int x, unsigned int y, unsigned int z)const {
		return SeparateBitForMorton(x) | SeparateBitForMorton(y) << 1 | SeparateBitForMorton(z) << 2;
	}

	unsigned int CColliderTree::SeparateBitForMorton(unsigned int val)const {
		unsigned int s = val;
		s = (s | s << 8) & 0x0000f00f;
		s = (s | s << 4) & 0x000c30c3;
		s = (s | s << 2) & 0x00249249;
		return s;
	}

	std::vector<std::shared_ptr<ACollider>> CColliderTree::GetPassiveColliderListFromTree(std::shared_ptr<ACollider> Collider) {
		std::vector<std::shared_ptr<ACollider>> PassiveColliderList;
		
		// ひとまず仮の実装(まだツリーを実装していない)
		for (const auto& PassiveCollider : m_ColliderList) {
			if (Collider!=PassiveCollider) {
				PassiveColliderList.push_back(PassiveCollider);
			}
		}

		return PassiveColliderList;
	}

	//
	bool CColliderTree::GetCollisionAllMap(std::unordered_map<ACollider*, ACollider*>& CollisionMap) {
		// 初期化
		CollisionMap.clear();

		// ルート空間から開始する
		std::vector<ACollider*> ColliderStack;
		if (!GetCollisionMap(0, CollisionMap, ColliderStack))return false;
		ColliderStack.clear();

		return true;
	}

	bool CColliderTree::GetCollisionMap(unsigned int CellIndex, std::unordered_map<ACollider*, ACollider*>& CollisionList, std::vector<ACollider*>& ColliderStack) {
		//
		if (!m_CellList[CellIndex]) {
			if (!CreateCell(CellIndex))return false;
		}
		CCellContents* CellContents1 = m_CellList[CellIndex]->GetLatestCellContents();

		bool IsHaveChild = false;
		int StackObjNum = 0;
		
		//
		while (CellContents1 != nullptr)
		{
			// 動的オブジェクトでないのなら外す
			/*if (CellContents1->GetCollider()->GetObjectType() != EObjectType::DYNAMIC) {
				CellContents1 = CellContents1->GetNextCellContents();
				continue;
			}*/

			//
			CCellContents* CellContents2 = CellContents1->GetNextCellContents();

			// まず同じセルのコライダーとの衝突リストを作成する
			while (CellContents2 != nullptr) {
				// 衝突リスト => 衝突する側 | 衝突される側
				CollisionList.emplace(std::make_pair(CellContents1->GetCollider().get(), CellContents2->GetCollider().get()));

				//
				CellContents2 = CellContents2->GetNextCellContents();
			}

			// 次に上位レベルのセルのコライダーとの衝突リストを作成する
			for (int i = 0; i < static_cast<int>(ColliderStack.size()); i++) {
				// 衝突リスト => 衝突する側 | 衝突される側
				CollisionList.emplace(std::make_pair(CellContents1->GetCollider().get(), ColliderStack[i]));
			}

			// 衝突スタックを登録する
			ColliderStack.push_back(CellContents1->GetCollider().get());
			IsHaveChild = true;
			StackObjNum++;

			//
			CellContents1 = CellContents1->GetNextCellContents();
		}

		//
		for (int n = 0; n < 8;n++) {
			unsigned int ChildCellIndex = CellIndex * 8 + 1 + n;
			if (ChildCellIndex < m_CellNum && m_CellList[ChildCellIndex]) {
				// 子空間に移動し衝突リストを作成する
				if (!GetCollisionMap(ChildCellIndex, CollisionList, ColliderStack))return false;
			}
		}
		
		// スタックのオブジェクトを後ろから削除する
		if (IsHaveChild) {
			for (int i = 0; i < StackObjNum;i++) {
				ColliderStack.pop_back();
			}
		}

		return true;
	}

	//
	const std::vector<std::shared_ptr<ACollider>>& CColliderTree::GetColliderList()const {
		return m_ColliderList;
	}

	//
	std::vector<std::shared_ptr<CCellContents>>& CColliderTree::GetCellContentsList(){
		return m_CellContentsList;
	}

	bool CColliderTree::CreateSphereCollider(const std::vector<std::shared_ptr<Primitive>>& PrimitiveList, GameObject* Obj, EObjectType ObjectType, EColliderType ColliderType) {
		glm::vec3 CenterPos;
		float Radius = 0.0f;

		for (auto& Prim : PrimitiveList) {
			glm::vec3 MaxCenterPos;
			float MaxRadius;
			if (!CalculateSphereVolumeFromAABB(Prim->GetVertices()[0], MaxCenterPos, MaxRadius))return false;

			if (MaxRadius > Radius) {
				CenterPos = MaxCenterPos;
				Radius = MaxRadius;
			}
		}

		//
		m_ColliderList.push_back(std::make_shared<CSphereCollider>(ObjectType, Obj->GetRootNode()->GetComponent<TransformComponent>(), CenterPos, Radius));
		m_CellContentsList.push_back(std::make_shared<CCellContents>(m_ColliderList[m_ColliderList.size()-1]));

		return true;
	}

	bool CColliderTree::CreateSimpleBoxCollider(const std::vector<std::shared_ptr<Primitive>>& PrimitiveList, GameObject* Obj, EObjectType ObjectType, EColliderType ColliderType) {
		
		glm::vec3 CenterPos = glm::vec3(0.0f, 0.0f, 0.0f);
		
		std::vector<glm::vec3> LocalAxis;
		LocalAxis.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
		LocalAxis.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		LocalAxis.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

		glm::vec3 LocalAxisSize = glm::vec3(1.0f, 1.0f, 1.0f)*0.5f;

		m_ColliderList.push_back(std::make_shared<CBoxCollider>(ObjectType, Obj->GetRootNode()->GetComponent<TransformComponent>(), CenterPos, LocalAxis, LocalAxisSize));
		m_CellContentsList.push_back(std::make_shared<CCellContents>(m_ColliderList[m_ColliderList.size() - 1]));

		return true;
	}

	bool CColliderTree::CreateBoxCollider(const std::vector<std::shared_ptr<Primitive>>& PrimitiveList, GameObject* Obj, EObjectType ObjectType, EColliderType ColliderType) {
		return true;
	}

	bool CColliderTree::CalculateAABB(const std::vector<float>& Vertices, glm::vec3& CenterPos, glm::vec3& MaxPos, glm::vec3& MinPos) {
		int MinX = 0, MaxX = 0, MinY = 0, MaxY = 0, MinZ = 0, MaxZ = 0;

		for (int i = 0; i < static_cast<int>(Vertices.size());i+=3) {
			if (Vertices[i + 0] < Vertices[MinX + 0])MinX = i;
			if (Vertices[i + 0] > Vertices[MaxX + 0])MaxX = i;

			if (Vertices[i + 1] < Vertices[MinY + 1])MinY = i;
			if (Vertices[i + 1] > Vertices[MaxY + 1])MaxY = i;

			if (Vertices[i + 2] < Vertices[MinZ + 2])MinZ = i;
			if (Vertices[i + 2] > Vertices[MaxZ + 2])MaxZ = i;
		}

		float MinDist = 0.0f, MaxDist = 0.0f;

		glm::vec3 xvec = glm::vec3(Vertices[MaxX + 0], Vertices[MaxX + 1], Vertices[MaxX + 2]) - glm::vec3(Vertices[MinX + 0], Vertices[MinX + 1], Vertices[MinX + 2]);
		float distx = glm::dot(xvec, xvec);

		glm::vec3 yvec = glm::vec3(Vertices[MaxY + 0], Vertices[MaxY + 1], Vertices[MaxY + 2]) - glm::vec3(Vertices[MinY + 0], Vertices[MinY + 1], Vertices[MinY + 2]);
		float disty = glm::dot(yvec, yvec);

		glm::vec3 zvec = glm::vec3(Vertices[MaxZ + 0], Vertices[MaxZ + 1], Vertices[MaxZ + 2]) - glm::vec3(Vertices[MinZ + 0], Vertices[MinZ + 1], Vertices[MinZ + 2]);
		float distz = glm::dot(zvec, zvec);

		// 一番離れたペアを選択する
		MaxPos = glm::vec3(Vertices[MaxX + 0], Vertices[MaxX + 1], Vertices[MaxX + 2]);
		MinPos = glm::vec3(Vertices[MinX + 0], Vertices[MinX + 1], Vertices[MinX + 2]);

		if (disty > distx && disty > distz) {
			MaxPos = glm::vec3(Vertices[MaxY + 0], Vertices[MaxY + 1], Vertices[MaxY + 2]);
			MinPos = glm::vec3(Vertices[MinY + 0], Vertices[MinY + 1], Vertices[MinY + 2]);
		}
		else if (distz > distx && distz > disty) {
			MaxPos = glm::vec3(Vertices[MaxZ + 0], Vertices[MaxZ + 1], Vertices[MaxZ + 2]);
			MinPos = glm::vec3(Vertices[MinZ + 0], Vertices[MinZ + 1], Vertices[MinZ + 2]);
		}

		CenterPos = (MaxPos + MinPos) * 0.5f;

		return true;
	}

	bool CColliderTree::CalculateSphereVolumeFromAABB(const std::vector<float>& Vertices, glm::vec3& CenterPos, float& Radius) {
		glm::vec3 MaxPos, MinPos;

		if (!CalculateAABB(Vertices,CenterPos,MaxPos,MinPos))return false;

		// 半径を計算
		Radius = glm::sqrt(glm::dot(MaxPos-CenterPos, MaxPos - CenterPos));

		return true;
	}

	bool CColliderTree::OptimizeSphereVolume() {
		return true;
	}

	bool CColliderTree::CalculateOBB() {
		return true;
	}

	bool CColliderTree::OptimizeOBB() {
		return true;
	}

}