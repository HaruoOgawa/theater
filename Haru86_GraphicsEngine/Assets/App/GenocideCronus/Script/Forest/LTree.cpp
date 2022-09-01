#include "LTree.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Math/mymath_withGLM.h"
#include <glm/gtc/random.hpp>

namespace myapp {
	LTreeNode::LTreeNode(int inum) :
		m_LAction(""),
		m_Indent(inum),
		m_LastVerticesData(glm::vec3(0.0f)),
		m_LastParentIndices(0),
		m_NodeParent(nullptr),
		m_LastGrowDir(glm::vec3(0.0f, 1.0f, 0.0f))
	{
	}

	void LTree::Generate(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices)
	{
		//
		m_LStep = -1;
		m_StartStructure = "";
		m_LStructure = "";
		m_LRootNode = nullptr;

		// L-System
		PrepareLSystem();
		GenerateLStructure();
		AnalyseLStructure();
		RunLSystem(VertexData, Dimention, Indices);
	}

	void LTree::PrepareLSystem() 
	{
		// 基礎情報
		m_LStep = 6;
		m_StartStructure = "X";
		
		// 書き換えルール
		m_LRule.push_back(LRule('F', "FF"));
		m_LRule.push_back(LRule('X', "F[+X]F[-X]+X"));
	}

	void LTree::GenerateLStructure()
	{
		// 初期文字列を代入
		m_LStructure += m_StartStructure;
		
		// 書き換えルールに従い書き換える
		for (int nLStep=0; nLStep<m_LStep; nLStep++)
		{
			// ルールをチェックする
			for (const auto& Rule : m_LRule) {
				// 今のステップでの更新内容
				std::string nowStep_LStructure = "";

				//
				for (char& now_word : m_LStructure)
				{
					// ターゲットであれば書き換える(nowStep_LStructureにはrResult_LWordを足す)
					if (now_word == Rule.rTarget_LWord)
					{
						nowStep_LStructure += Rule.rResult_LWord;
					}
					else // 合致しなければそのまま(rTarget_LWordを足す-->もとの文字)
					{
						nowStep_LStructure += now_word;
					}
				}

				// 更新された内容を反映する
				m_LStructure = nowStep_LStructure;
			}
		}
	}

	void LTree::AnalyseLStructure()
	{
		//
		std::shared_ptr<LTreeNode> CurrentParentLNode = nullptr;
		std::shared_ptr<LTreeNode> CurrentLNode = nullptr;
		
		// ルートノードを作成
		int CurrentIndent = 0;
		m_LRootNode = std::make_shared<LTreeNode>(CurrentIndent);
		CurrentLNode = m_LRootNode;

		// ひとつ前に戻るためのペアレントノードやノードを一時保存しておくためのベクター
		std::vector<std::shared_ptr<LTreeNode>> PreviousParentLNodeList;
		std::vector<std::shared_ptr<LTreeNode>> PreviousLNodeList;

		// ありうるパターン
		// ***[***[***]***]***
		// ***[***[***]***[***]***]***
		// ***[***[***[***]***]***[***[***]***[***]***]***]***
		
		//Console::Log("m_LStructure: %s\n", m_LStructure.c_str());

		//
		for (const auto& LWord : m_LStructure)
		{
			if (LWord == '[')
			{
				// 分岐(子要素への移動の目印としてアスタリスクを配置)
				CurrentLNode->m_LAction += '*';

				//
				if (CurrentParentLNode) PreviousParentLNodeList.push_back(CurrentParentLNode);
				PreviousLNodeList.push_back(CurrentLNode);

				//
				CurrentIndent++;
				CurrentParentLNode = CurrentLNode;
				CurrentLNode = std::make_shared<LTreeNode>(CurrentIndent);
				CurrentLNode->m_NodeParent = CurrentParentLNode;
				if (CurrentParentLNode) CurrentParentLNode->m_LNodeList.push_back(CurrentLNode);
			}
			else if (LWord == ']')
			{
				CurrentIndent--;
				
				if (PreviousParentLNodeList.size() > 0 && PreviousLNodeList.size() > 0)
				{
					//
					CurrentParentLNode = PreviousParentLNodeList[PreviousParentLNodeList.size() - 1];
					PreviousParentLNodeList.pop_back();
					PreviousParentLNodeList.shrink_to_fit();

					//
					CurrentLNode = PreviousLNodeList[PreviousLNodeList.size() - 1];
					PreviousLNodeList.pop_back();
					PreviousLNodeList.shrink_to_fit();
				}
				else // 戻すものがないということはルートノードであるということ
				{
					CurrentLNode = m_LRootNode;
					CurrentParentLNode = nullptr;
				}
			}
			else if (LWord == 'X')
			{
				// A,Bは無効な文字列なので無視する
			}
			else
			{
				// CurrentLNodeのアクションに追加する
				CurrentLNode->m_LAction += LWord;
			}
		}
	}

	void LTreeNode::BuildLNode(std::vector<glm::vec3>& LTree_Vertices, std::vector<glm::vec3>& LTree_Normals,
		std::vector<float>& LTreeRadiusList, std::vector<unsigned short>& LTree_Indices,const float LTreeRadius, float& LTreeLength)
	{
		// Debug /////////////////////////////
		std::string DebugStr = m_LAction;
		for (int n = -1; n < m_Indent; n++)
		{
			DebugStr = ">" + DebugStr;
		}

		//Console::Log("%d (p)%p (my)%p %s\n", m_LNodeList.size(),m_NodeParent.get(),this, DebugStr.c_str());
		/////////////////////////////////////

		// スタート地点を定義親要素との繋ぎ目(Nodeの中でこれを使いまわす)
		glm::vec3 StartPosInCNode = glm::vec3(0.0f);
		if (m_NodeParent) StartPosInCNode = m_NodeParent->m_LastVerticesData;

		// 成長ベクトル(どの方向に伸びるか)。回転では、このベクトルを加工する
		glm::vec3 StartGrowDir = glm::vec3(0.0f, 1.0f, 0.0f);
		if (m_NodeParent) StartGrowDir = m_NodeParent->m_LastGrowDir;

		// インデックスデータ
		unsigned short FirstIndices = 0;
		if (m_NodeParent) FirstIndices = m_NodeParent->m_LastParentIndices;
		unsigned short SecondIndices = 1;
		if(LTree_Indices.size()>0) SecondIndices = LTree_Indices[LTree_Indices.size() - 1] + 1;

		// Fの数を事前に計算して0以下ならそのノードは子要素だけ調べてあとは無視する
		int FNum = 0;
		for (const auto& LWord : m_LAction)
		{
			if (LWord == 'F') FNum++;
		}

		if (FNum <= 0)
		{
			// 子要素のBuild
			for (auto& Node : m_LNodeList)
			{
				Node->BuildLNode(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, LTreeRadius, LTreeLength);
			}

			return;
		}

		// 現在のノードの持つ子要素のリスト --> * が来たら再帰的に子要素の中に入っていく
		std::vector<std::shared_ptr<LTreeNode>> ChildNodeList;
		ChildNodeList.resize(m_LNodeList.size());
		for (int n=0;n< m_LNodeList.size();n++)
		{
			ChildNodeList.push_back(m_LNodeList[m_LNodeList.size() - 1 - n]);
		}

		// LTreeNodeをもとに木のメッシュを作成
		for (int n=0;n< m_LAction.size();n++)
		{
			const auto& LWord = m_LAction[n];

			// LWordをもとにL-Systemアクションを決定する
			if (LWord == 'F') // 先に進んで線を引く
			{
				// 頂点
				StartPosInCNode += StartGrowDir * LTreeLength;
				LTree_Vertices.push_back(StartPosInCNode);

				// 法線(成長ベクトルとvec3(1.0,0.0,0.0)との外積)
				LTree_Normals.push_back(glm::normalize(glm::cross(StartGrowDir, glm::vec3(1.0f, 0.0f, 0.0f))));

				// 半径の計算
				float NodeRadius = LTreeRadius;

				// インデントが深いほど細くなる
				NodeRadius *= glm::pow(0.65f, float(m_Indent));

				// 半径(現在のノードの中でm_Actionの最後の方ほど細くなる)
				NodeRadius *= glm::exp(-2.0f * ( float((n)) / float(m_LAction.size())) );
				
				// 半径代入
				LTreeRadiusList.push_back(NodeRadius);

				// Debug
				//Console::Log("%d FirstIndices: %d / SecondIndices: %d\n", (LTree_Vertices.size() - 1), FirstIndices, SecondIndices);

				// インデックスデータ
				LTree_Indices.push_back(FirstIndices);
				LTree_Indices.push_back(SecondIndices);
				FirstIndices= SecondIndices;
				SecondIndices++;
			}
			else if (LWord == '+') // 時計回りに回転
			{
				// もし『+』『-』が末尾なのであればなにもしない
				if (((n + 1) < m_LAction.size()) && (m_LAction[n + 1] == 'X'))
				{
					continue;
				}

				//
				float angle =  20.0f * (3.14f/180.0f);
				float rotAxisZ = glm::linearRand(0.0f, 1.0f);
				//glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3(0.0f, 0.0f, angle))) * glm::vec4(glm::vec3(0.0f,1.0f,0.0f), 0.0);
				glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3( (rotAxisZ<0.5f)? angle :0.0f, 0.0f, (rotAxisZ >= 0.5f) ? angle : 0.0f))) * glm::vec4(StartGrowDir, 0.0);
				StartGrowDir = glm::normalize(glm::vec3(rotV.x, rotV.y, rotV.z));
			}
			else if (LWord == '-') // 半時計周りに回転
			{
				// もし『+』『-』が末尾なのであればなにもしない
				if (((n + 1) < m_LAction.size()) && (m_LAction[n + 1] == 'X'))
				{
					continue;
				}

				//
				float angle = - 20.0f * (3.14f / 180.0f);
				float rotAxisZ = glm::linearRand(0.0f, 1.0f);
				//glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3(0.0f, 0.0f, angle))) * glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 0.0);
				glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3((rotAxisZ < 0.5f) ? angle : 0.0f, 0.0f, (rotAxisZ >= 0.5f) ? angle : 0.0f))) * glm::vec4(StartGrowDir, 0.0);
				StartGrowDir = glm::normalize(glm::vec3(rotV.x, rotV.y, rotV.z));
			}
			else if (LWord=='*') // 子要素へ移動
			{
				// 最後の頂点データを現在のノードの子要素との繋ぎ目にする
				m_LastVerticesData = LTree_Vertices[LTree_Vertices.size() - 1];

				// 上記と同様に現在のノードプロセスの中でいろいろと加工した成長ベクトルを子要素に引き継ぐ
				m_LastGrowDir = StartGrowDir;

				// Indicesも最後のものをParentのものとして登録しておく
				m_LastParentIndices = LTree_Indices[LTree_Indices.size() - 1];

				// 子要素のBuild
				auto Child = ChildNodeList.back();
				ChildNodeList.pop_back();
				Child->BuildLNode(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, LTreeRadius, LTreeLength);

				// 子要素でIndicesが更新されるのでそれを考慮して再配置
				SecondIndices = LTree_Indices[LTree_Indices.size() - 1] + 1; // 現時点のIndicesの最大値+1
			}
		}

		// 最後の頂点データを現在のノードの子要素との繋ぎ目にする
		m_LastVerticesData = LTree_Vertices[LTree_Vertices.size() - 1];

		// 上記と同様に現在のノードプロセスの中でいろいろと加工した成長ベクトルを子要素に引き継ぐ
		m_LastGrowDir = StartGrowDir;

		// Indicesも最後のものをParentのものとして登録しておく
		m_LastParentIndices = LTree_Indices[LTree_Indices.size() - 1];
	}

	void LTree::RunLSystem(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices)
	{
		// LStructureから頂点データを作成
		std::vector<glm::vec3> LTree_Vertices;
		std::vector<glm::vec3> LTree_Normals;
		std::vector<float> LTreeRadiusList; // 段々縮小していく半径
		std::vector<unsigned short> LTree_Indices;

		// 基本パラメーター(初期値)
		//float LTreeRadius = 1.0f;
		float LTreeRadius = 1.0f;
		float LTreeLength = 1.0f;
		//float LTreeLength = 1.0f*0.1f;

		// 初期値を設定(原点)
		LTree_Vertices.push_back(glm::vec3(0.0f));
		LTree_Normals.push_back(glm::vec3(0.0f));
		LTreeRadiusList.push_back(LTreeRadius);

		// LTreeNodeをもとに木のメッシュを作成
		m_LRootNode->BuildLNode(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, LTreeRadius, LTreeLength);

		// メッシュオブジェクトを構築
		BuildLTreeMesh(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, VertexData, Dimention, Indices);
	}

	void LTree::BuildLTreeMesh(std::vector<glm::vec3>& LTree_Vertices, std::vector<glm::vec3>& LTree_Normals,
		const std::vector<float>& LTree_Radius, std::vector<unsigned short>& LTree_Indices,
		std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices)
	{
		VertexData.push_back(mymath::CastVec3ToLine_float(LTree_Vertices));
		VertexData.push_back(mymath::CastVec3ToLine_float(LTree_Normals));
		VertexData.push_back(LTree_Radius);
		Dimention.push_back(3);
		Dimention.push_back(3);
		Dimention.push_back(1);
		Indices = LTree_Indices;
	}
}