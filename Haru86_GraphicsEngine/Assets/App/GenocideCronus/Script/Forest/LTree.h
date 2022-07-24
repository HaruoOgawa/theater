#pragma once
#include <vector>
#include <memory>
#include <string>

class Mesh;
class Material;
class TransformComponent;

namespace myapp {

	// やっぱり試しに『char』でやってみたい
	/*enum class LWord
	{
		LG,  // L-Syetem Go Formard --> 先に進んで線を引く						--> F
		LPR, // L-Syetam Positive Rotate --> +方向(時計回り)へ回転する				--> +
		LNR, // L-System Negative Rotate --> -方向(半時計周り)へ回転する			--> -
		LDS, // L-System Devide Start --> 分岐のスタート							--> [
		LDE, // L-Syetem Devide End --> 分岐のゴール(スタートに戻って別の規則へ)	--> ]
	};*/

	struct LRule
	{
		char rTarget_LWord; // 書き換え対象文字列
		std::string rResult_LWord; // 書き換える内容

		LRule(char rtw, const std::string& rule) :
			rTarget_LWord(rtw),
			rResult_LWord(rule)
		{
		}
	};

	class LTree
	{
		// Base Info
		std::shared_ptr<Mesh> m_TreeMesh;
		std::shared_ptr<Material> m_TreeMaterial;
		std::shared_ptr<TransformComponent> m_TreeTRS;

		// L-System Info
		int m_LStep;
		std::string m_StartStructure;
		std::vector<LRule> m_LRule;
		std::string m_LStructure;
	public:
		LTree();
		virtual ~LTree()=default;

		void Start();
		void Update();
		void Draw();
	
	private:
		// Base MeThod
		void PrepareLSystem();
		void GenerateLStructure();
		void RunLSystem();
		void BuildLTreeMesh();

		// L-System Action Method

		// 
		void CreateBaseData();
	};
}