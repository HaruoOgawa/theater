#pragma once
#include <vector>
#include <memory>

namespace physics {
	class CCellContents;
	
	class CColliderCell
	{
		std::vector<std::shared_ptr<CCellContents>> m_CellContentsList;

		CCellContents* m_LatestCellContents;
	public:
		CColliderCell();
		virtual ~CColliderCell();

		bool SetCellContents(const std::shared_ptr<CCellContents>& CellContents);
		const std::vector<std::shared_ptr<CCellContents>>& GetCellContentsList()const;

		bool OnRemove(CCellContents* CellContents);

		//
		CCellContents* GetLatestCellContents();
	};
}