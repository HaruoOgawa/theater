#include "CColliderCell.h"
#include "CCellContents.h"

namespace physics {
	CColliderCell::CColliderCell() :
		m_LatestCellContents(nullptr)
	{
	}

	CColliderCell::~CColliderCell() {
		m_CellContentsList.clear();
	}

	bool CColliderCell::SetCellContents(const std::shared_ptr<CCellContents>& CellContents) {
		//
		if (!CellContents->GetCollider())return false;
		
		// セルが同一なら登録しない
		if (CellContents->GetColliderCell() == this)return false;

		if (!m_LatestCellContents) {
			m_LatestCellContents = CellContents.get();
		}
		else {
			CellContents->SetNextCellContents(m_LatestCellContents);
			m_LatestCellContents->SetPreCellContents(CellContents.get());
			m_LatestCellContents = CellContents.get();
		}

		// セルをセルコンテンツに登録
		CellContents->SetColliderCell(this);
		
		return true;
	}

	const std::vector<std::shared_ptr<CCellContents>>& CColliderCell::GetCellContentsList()const {
		return m_CellContentsList;
	}

	bool CColliderCell::OnRemove(CCellContents* CellContents) {
		// もし最新のセルコンテンツから離脱通知がきたらその次のものに切り替える
		if (CellContents == m_LatestCellContents) {
			m_LatestCellContents = CellContents->GetNextCellContents();
		}

		return true;
	}

	//
	CCellContents* CColliderCell::GetLatestCellContents(){
		return m_LatestCellContents;
	}
}