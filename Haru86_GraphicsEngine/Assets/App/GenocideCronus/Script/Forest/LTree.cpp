#include "LTree.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Math/mymath_withGLM.h"
#include <glm/gtc/random.hpp>

namespace myapp {
	LTreeNode::LTreeNode(int inum) :
		m_LAction(""),
		m_DebugIndentNum(inum),
		m_LastVerticesData(glm::vec3(0.0f)),
		m_NodeParent(nullptr),
		m_LastGrowDir(glm::vec3(0.0f, 1.0f, 0.0f))
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
		
		// オブジェクト(Mesh(RunLSystemで作る)/Mat/TRS/CS)など
		CreateBaseData();
	}

	void LTree::PrepareLSystem() 
	{
		// 基礎情報
		m_LStep = 5;
		m_StartStructure = "X";
		
		// 書き換えルール
		m_LRule.push_back(LRule('X', "F-[[X]+X]+F[[X]+X]-X",true));
		m_LRule.push_back(LRule('X', "F-[[X]+X]+F[-FX]+X", true));
		m_LRule.push_back(LRule('X', "F[+X][-X]FX", true));
		m_LRule.push_back(LRule('X', "F[+X]F[-X]+X", true));
		m_LRule.push_back(LRule('F', "FF", false));
	}

	void LTree::GenerateLStructure()
	{
		// 初期文字列を代入
		m_LStructure += m_StartStructure;
		
		// 書き換えルールに従い書き換える
		for (int nLStep=0; nLStep<m_LStep; nLStep++)
		{
			// Rule1
			int proIndex = static_cast<int>(glm::floor(glm::linearRand(0.0f, 3.9f)));
			
			{
				// 今のステップでの更新内容
				std::string nowStep_LStructure = "";

				//
				for (char& now_word : m_LStructure)
				{
					const auto& Rule = m_LRule[proIndex];
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

			// Rule2
			{
				// 今のステップでの更新内容
				std::string nowStep_LStructure = "";

				//
				for (char& now_word : m_LStructure)
				{
					const auto& Rule = m_LRule[4];
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
			}

			/*// ルールをチェックする
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
			}*/
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

	void LTreeNode::BuildLNode(std::vector<glm::vec3>& LTree_Vertices, std::vector<glm::vec3>& LTree_Normals,
		std::vector<float>& LTreeRadiusList, std::vector<unsigned short>& LTree_Indices, float& LTreeRadius, float& LTreeLength)
	{
		// Debug /////////////////////////////
		std::string DebugStr = m_LAction;
		for (int n = -1; n < m_DebugIndentNum; n++)
		{
			DebugStr = ">" + DebugStr;
		}

		//Console::Log("%d %s\n", m_LNodeList.size(), DebugStr.c_str());
		/////////////////////////////////////

		// スタート地点を定義親要素との繋ぎ目(Nodeの中でこれを使いまわす)
		glm::vec3 StartPosInCNode = glm::vec3(0.0f);
		if (m_NodeParent) StartPosInCNode = m_NodeParent->m_LastVerticesData;

		// 成長ベクトル(どの方向に伸びるか)。回転では、このベクトルを加工する
		glm::vec3 StartGrowDir = glm::vec3(0.0f, 1.0f, 0.0f);
		if (m_NodeParent) StartGrowDir = m_NodeParent->m_LastGrowDir;

		// インデックスデータ
		unsigned short FirstIndices = LTree_Vertices.size()-1;
		unsigned short SecondIndices = FirstIndices + 1;

		// LTreeNodeをもとに木のメッシュを作成
		for (const auto& LWord : m_LAction)
		{
			// LWordをもとにL-Systemアクションを決定する
			if (LWord == 'F') // 先に進んで線を引く
			{
				// 頂点
				StartPosInCNode += StartGrowDir * LTreeLength;
				LTree_Vertices.push_back(StartPosInCNode);

				// 法線(成長ベクトルとvec3(1.0,0.0,0.0)との外積)
				LTree_Normals.push_back(glm::normalize(glm::cross(StartGrowDir, glm::vec3(1.0f, 0.0f, 0.0f))));

				// 半径
				//LTreeLength *= 0.8f;
				//LTreeRadius *= 0.8f;
				//LTreeRadiusList.push_back(LTreeRadius);

				// インデックスデータ
				LTree_Indices.push_back(FirstIndices);
				LTree_Indices.push_back(SecondIndices);
				FirstIndices++;
				SecondIndices++;

			}
			else if (LWord == '+') // 時計回りに回転
			{
				float angleA = (3.14f/4.0f) * mymath::rand(glm::vec2(StartPosInCNode.y + StartPosInCNode.x, StartPosInCNode.y + StartPosInCNode.z));
				float angleB = (3.14f / 4.0f) * mymath::rand(glm::vec2(StartPosInCNode.y + StartPosInCNode.z, StartPosInCNode.y + StartPosInCNode.x));
				float angleC = (3.14f / 4.0f) * mymath::rand(glm::vec2(StartPosInCNode.y + StartPosInCNode.z + StartPosInCNode.x, StartPosInCNode.y + StartPosInCNode.x));
				/*glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), StartGrowDir));
				glm::vec3 bionormal = glm::normalize(glm::cross(normal, StartGrowDir));
				
				glm::quat rot_normal = glm::quat(normal.x * glm::sin(angleA / 2.0), normal.y * glm::sin(angleA / 2.0), normal.z * glm::sin(angleA / 2.0), glm::cos(angleA / 2.0));
				glm::quat rot_bionormal = glm::quat(bionormal.x * glm::sin(angleB / 2.0), bionormal.y * glm::sin(angleB / 2.0), bionormal.z * glm::sin(angleB / 2.0), glm::cos(angleB / 2.0));
				glm::vec4 rotV = glm::mat4_cast(rot_normal * rot_bionormal) * glm::vec4(StartGrowDir, 0.0);
				StartGrowDir = glm::normalize(glm::vec3(rotV.x, rotV.y, rotV.z));*/

				glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3(angleA,angleB,angleC))) * glm::vec4(StartGrowDir, 0.0);
				StartGrowDir = glm::normalize(glm::vec3(rotV.x, rotV.y, rotV.z));
			}
			else if (LWord == '-') // 半時計周りに回転
			{
				float angleA = - (3.14f / 4.0f) * mymath::rand(glm::vec2(StartPosInCNode.y + StartPosInCNode.x, StartPosInCNode.y + StartPosInCNode.z));
				float angleB = - (3.14f / 4.0f) * mymath::rand(glm::vec2(StartPosInCNode.y + StartPosInCNode.z, StartPosInCNode.y + StartPosInCNode.x));
				float angleC = - (3.14f / 4.0f) * mymath::rand(glm::vec2(StartPosInCNode.y + StartPosInCNode.z, StartPosInCNode.y + StartPosInCNode.x));
				/*glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), StartGrowDir));
				glm::vec3 bionormal = glm::normalize(glm::cross(normal, StartGrowDir));

				glm::quat rot_normal = glm::quat(normal.x * glm::sin(angleA / 2.0), normal.y * glm::sin(angleA / 2.0), normal.z * glm::sin(angleA / 2.0), glm::cos(angleA / 2.0));
				glm::quat rot_bionormal = glm::quat(bionormal.x * glm::sin(angleB / 2.0), bionormal.y * glm::sin(angleB / 2.0), bionormal.z * glm::sin(angleB / 2.0), glm::cos(angleB / 2.0));
				glm::vec4 rotV = glm::mat4_cast(rot_normal * rot_bionormal) * glm::vec4(StartGrowDir, 0.0);
				StartGrowDir = glm::normalize(glm::vec3(rotV.x, rotV.y, rotV.z));*/

				glm::vec4 rotV = glm::mat4_cast(glm::quat(glm::vec3(angleA, angleB, angleC))) * glm::vec4(StartGrowDir, 0.0);
				StartGrowDir = glm::normalize(glm::vec3(rotV.x, rotV.y, rotV.z));
			}
			else // 無効な文字列 
			{
				//Console::Log("Invalid char ERROR >>>>>>>>>>>>>>>>>>>>>>>>>> %c\n", LWord); // A Bは素材なため無効な文字列としてきてもおｋ
			}
		}

		// 最後の頂点データを現在のノードの子要素との繋ぎ目にする
		m_LastVerticesData = LTree_Vertices[LTree_Vertices.size() - 1];

		// 上記と同様に現在のノードプロセスの中でいろいろと加工した成長ベクトルを子要素に引き継ぐ
		m_LastGrowDir = StartGrowDir;

		// 子要素のBuild
		for (auto& Node : m_LNodeList)
		{
			Node->BuildLNode(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, LTreeRadius, LTreeLength);
		}
	}

	void LTree::RunLSystem()
	{
		// LStructureから頂点データを作成
		std::vector<glm::vec3> LTree_Vertices;
		std::vector<glm::vec3> LTree_Normals;
		std::vector<float> LTreeRadiusList; // 段々縮小していく半径
		std::vector<unsigned short> LTree_Indices;

		// 基本パラメーター(初期値)
		float LTreeRadius = 1.0f;
		//float LTreeLength = 1.0f;
		float LTreeLength = 1.0f*0.5f;

		// 初期値を設定(原点)
		LTree_Vertices.push_back(glm::vec3(0.0f));
		LTree_Normals.push_back(glm::vec3(0.0f));
		LTreeRadiusList.push_back(LTreeRadius);

		// LTreeNodeをもとに木のメッシュを作成
		m_LRootNode->BuildLNode(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices, LTreeRadius, LTreeLength);

		// メッシュオブジェクトを構築
		BuildLTreeMesh(LTree_Vertices, LTree_Normals, LTreeRadiusList, LTree_Indices);
	}

	void LTree::BuildLTreeMesh(std::vector<glm::vec3>& LTree_Vertices, std::vector<glm::vec3>& LTree_Normals,
		const std::vector<float>& LTree_Radius, std::vector<unsigned short>& LTree_Indices)
	{
		// メッシュオブジェクトの構築
		std::vector<std::vector<float>> VertexData;
		std::vector<int> Dimentions;
		std::vector<unsigned short> Indices;

		VertexData.push_back(mymath::CastVec3ToLine_float(LTree_Vertices));
		VertexData.push_back(mymath::CastVec3ToLine_float(LTree_Normals));
		//VertexData.push_back(LTree_Radius);
		Dimentions.push_back(3);
		Dimentions.push_back(3);
		//Dimentions.push_back(1);
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