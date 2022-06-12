#pragma once
#include <memory>

namespace physics {
	class CColliderCell;
	class ACollider;
	
	class CCellContents
	{
		std::shared_ptr<ACollider> m_Collider;
		//
		CColliderCell* m_ColliderCell;

		//
		CCellContents* m_NextCellContents;
		CCellContents* m_PreCellContents;

	public:
		CCellContents(const std::shared_ptr<ACollider>& Collider);
		virtual ~CCellContents();

		const std::shared_ptr<ACollider>& GetCollider()const;
		//
		CColliderCell* GetColliderCell()const;
		void SetColliderCell(CColliderCell* ColliderCell);


		//
		void SetPreCellContents(CCellContents* CellContents);
		CCellContents* GetPreCellContents()const;
		void SetNextCellContents(CCellContents* CellContents);
		CCellContents* GetNextCellContents()const;

		//
		bool RemoveFromCell();
	};
}