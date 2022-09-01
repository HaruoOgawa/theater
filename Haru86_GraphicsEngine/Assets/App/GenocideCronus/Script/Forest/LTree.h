#pragma once
#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

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

	struct LRule // 文字列の書き換え規則
	{
		char rTarget_LWord; // 書き換え対象文字列
		std::string rResult_LWord; // 書き換える内容
		//bool m_UseProbability; // 確率計算を使うかどうか

		LRule(char rtw, const std::string& rule) :
			rTarget_LWord(rtw),
			rResult_LWord(rule)
		{
		}
	};

	class LTreeNode // Treeの枝分かれをNodeで表現するもの
	{
	public:
		std::string m_LAction; // L-Systemの記号
		std::vector<std::shared_ptr<LTreeNode>> m_LNodeList;
		int m_Indent; // インデント(子要素の入子の入り具合)
		glm::vec3 m_LastVerticesData; // 子要素(m_LNodeList)との繋ぎ目の座標
		glm::vec3 m_LastGrowDir; // 子要素に渡す成長ベクトル
		unsigned short m_LastParentIndices; // 子要素に渡すIndeceis
		std::shared_ptr<LTreeNode> m_NodeParent;

		LTreeNode(int inum);
		void BuildLNode(std::vector<glm::vec3>& LTree_Vertices, std::vector<glm::vec3>& LTree_Normals,
			std::vector<float>& LTreeRadiusList,std::vector<unsigned short>& LTree_Indices,const float LTreeRadius, float& LTreeLength);
	};

	class LTree
	{
		int m_LStep;
		std::string m_StartStructure;
		std::vector<LRule> m_LRule;
		std::string m_LStructure;
		std::shared_ptr<LTreeNode> m_LRootNode;
	public:
		LTree()=default;
		virtual ~LTree()=default;

		void Generate(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices);
	
	private:
		void PrepareLSystem();
		void GenerateLStructure();

		void AnalyseLStructure();
		void RunLSystem(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices);
		void BuildLTreeMesh(std::vector<glm::vec3>& LTree_Vertices,std::vector<glm::vec3>& LTree_Normals,
			const std::vector<float>& LTreeRadiusList,std::vector<unsigned short>& LTree_Indices,
			std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices);
	};
}