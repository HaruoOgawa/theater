#include "CCellContents.h"
#include "ACollider.h"
#include "CColliderCell.h"

namespace physics {
	CCellContents::CCellContents(const std::shared_ptr<ACollider>& Collider) :
		m_Collider(Collider),
		m_ColliderCell(nullptr),
		m_NextCellContents(nullptr),
		m_PreCellContents(nullptr)
	{
	}

	CCellContents::~CCellContents() {

	}

	const std::shared_ptr<ACollider>& CCellContents::GetCollider()const {
		return m_Collider;
	}

	//
	CColliderCell* CCellContents::GetColliderCell()const {
		return m_ColliderCell;
	}

	void CCellContents::SetColliderCell(CColliderCell* ColliderCell){
		m_ColliderCell = ColliderCell;
	}

	//
	void CCellContents::SetPreCellContents(CCellContents* CellContents){
		m_PreCellContents = CellContents;
	}

	CCellContents* CCellContents::GetPreCellContents()const {
		return m_PreCellContents;
	}

	void CCellContents::SetNextCellContents(CCellContents* CellContents) {
		m_NextCellContents = CellContents;
	}

	CCellContents* CCellContents::GetNextCellContents()const {
		return m_NextCellContents;
	}

	// 所属するセルからの離脱
	bool CCellContents::RemoveFromCell() {
		// 既に空なら抜ける
		if (!m_ColliderCell)return false;

		// セルに抜けることを通知する
		if (!m_ColliderCell->OnRemove(this))return false;

		// 前後のポインターを繋ぎなおす
		if (m_PreCellContents!=nullptr) {
			m_PreCellContents->SetNextCellContents(m_NextCellContents);
		}

		if (m_NextCellContents!=nullptr) {
			m_NextCellContents->SetPreCellContents(m_PreCellContents);
		}

		// 自身の要素を空にする
		m_NextCellContents = nullptr;
		m_PreCellContents = nullptr;
		m_ColliderCell = nullptr;

		return true;
	}

}