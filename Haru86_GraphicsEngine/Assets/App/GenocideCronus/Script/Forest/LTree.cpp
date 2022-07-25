#include "LTree.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Math/mymath_withGLM.h"

namespace myapp {
	LTreeNode::LTreeNode(int inum) :
		m_LAction(""),
		m_DebugIndentNum(inum)
	{
	}

	LTree::LTree():
		m_TreeMesh(nullptr),
		m_TreeMaterial(nullptr),
		m_TreeTRS(std::make_shared<TransformComponent>()),
		m_LStep(-1),
		m_StartStructure(""),
		m_LStructure(""),
		m_LRootNode(nullptr)
	{
		Start();
	}

	void LTree::Start()
	{
		// L-System
		PrepareLSystem();
		GenerateLStructure();
		AnalyseLStructure();
		RunLSystem();
		BuildLTreeMesh();

		// オブジェクト(Mesh(RunLSystemで作る)/Mat/TRS/CS)など
		CreateBaseData();
	}

	void LTree::PrepareLSystem() 
	{
		// 基礎情報
		//m_LStep = 5;
		m_LStep = 1;
		m_StartStructure = "A[B]";
		
		// 書き換えルール
		m_LRule.push_back(LRule('A', "F+B"));
		m_LRule.push_back(LRule('B', "[+F][-F]A"));
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
		
		Console::Log("m_LStructure: %s\n", m_LStructure.c_str());

		//
		for (const auto& LWord : m_LStructure)
		{
			if (LWord == '[')
			{
				//
				if (CurrentParentLNode) PreviousParentLNodeList.push_back(CurrentParentLNode);
				PreviousLNodeList.push_back(CurrentLNode);

				//
				CurrentIndent++;
				CurrentParentLNode = CurrentLNode;
				CurrentLNode = std::make_shared<LTreeNode>(CurrentIndent);
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
			else if (LWord == 'A' || LWord == 'B')
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

	void LTreeNode::BuildLNode()
	{
		// Debug
		std::string DebugStr = m_LAction;
		for (int n = -1; n < m_DebugIndentNum; n++)
		{
			DebugStr = ">" + DebugStr;
		}

		Console::Log("%d %s\n", m_LNodeList.size(), DebugStr.c_str());

		// LTreeNodeをもとに木のメッシュを作成
		for (const auto& LWord : m_LAction)
		{
			// LWordをもとにL-Systemアクションを決定する
			if (LWord == 'F') // 先に進んで線を引く
			{
				//Console::Log("%c Go Forward and Draw Line\n", LWord);
			}
			else if (LWord == '+') // 時計回りに回転
			{
				//Console::Log("%c Rotate Positive\n", LWord);
			}
			else if (LWord == '-') // 半時計周りに回転
			{
				//Console::Log("%c Rotate Negative\n", LWord);
			}
			else // 無効な文字列 
			{
				//Console::Log("Invalid char ERROR >>>>>>>>>>>>>>>>>>>>>>>>>> %c\n", LWord); // A Bは素材なため無効な文字列としてきてもおｋ
			}
		}

		// 子要素のBuild
		for (auto& Node : m_LNodeList)
		{
			Node->BuildLNode();
		}
	}

	void LTree::RunLSystem()
	{
		// LTreeNodeをもとに木のメッシュを作成
		m_LRootNode->BuildLNode();
	}

	void LTree::BuildLTreeMesh()
	{
		// LStructureから頂点データを作成
		std::vector<glm::vec3> LTree_Vertices;
		std::vector<glm::vec3> LTree_Normals;
		std::vector<unsigned short> LTree_Indices;

		// test data
		LTree_Vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		LTree_Vertices.push_back(glm::vec3(0.0f, 4.0f, 0.0f));
		
		LTree_Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		LTree_Normals.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

		LTree_Indices.push_back(0);
		LTree_Indices.push_back(1);

		// メッシュオブジェクトの構築
		std::vector<std::vector<float>> VertexData;
		std::vector<int> Dimentions;
		std::vector<unsigned short> Indices;

		VertexData.push_back(mymath::CastVec3ToLine_float(LTree_Vertices));
		VertexData.push_back(mymath::CastVec3ToLine_float(LTree_Normals));
		Dimentions.push_back(3);
		Dimentions.push_back(3);
		Indices = LTree_Indices;

		m_TreeMesh = std::make_shared<Mesh>(VertexData,Dimentions,Indices);
	}

	void LTree::CreateBaseData()
	{
		// マテリアル
		std::string LTree_vert = {
			#include "../../Shader/Forest/LTree.vert"
		};
		
		std::string LTree_geom = {
			#include "../../Shader/Forest/LTree.geom"
		};

		m_TreeMaterial = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER,LTree_vert,shaderlib::ShaderLib::Standard_frag,LTree_geom);
	}

	void LTree::Update()
	{

	}

	void LTree::Draw() 
	{
		m_TreeMaterial->SetActive();
		m_TreeTRS->CalMatrix();
		m_TreeMaterial->SetMatrixUniform("MVPMatrix", m_TreeTRS->m_pMatrix * m_TreeTRS->m_vMatrix * m_TreeTRS->m_mMatrix);
		m_TreeMaterial->SetMatrixUniform("MMatrix", m_TreeTRS->m_mMatrix);
		m_TreeMaterial->SetMatrixUniform("VMatrix", m_TreeTRS->m_vMatrix);
		m_TreeMaterial->SetMatrixUniform("PMatrix", m_TreeTRS->m_pMatrix);
		m_TreeMaterial->SetIntUniform("_UseLighting", 1);

		m_TreeMaterial->SetFloatUniform("_TreeMaxRadius", 0.05f);
		m_TreeMaterial->SetIntUniform("_TreeSegment", 12);

		m_TreeMaterial->SetVec3Uniform("_LightDir", glm::vec3(1.0, 1.0, -1.0));

		m_TreeMesh->Draw(GL_LINES);
	}
}