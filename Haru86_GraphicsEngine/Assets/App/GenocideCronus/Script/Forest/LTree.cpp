#include "LTree.h"
#include "GraphicsEngine/Message/Console.h"

namespace myapp {
	LTree::LTree():
		m_LStep(-1),
		m_StartStructure(""),
		m_LStructure("")
	{
		Start();
	}

	void LTree::Start()
	{
		// L-System
		PrepareLSystem();
		GenerateLStructure();
		RunLSystem();

		// オブジェクト(Mesh(RunLSystemで作る)/Mat/TRS/CS)など

	}

	void LTree::PrepareLSystem() 
	{
		// 基礎情報
		m_LStep = 5;
		m_StartStructure = "A";
		//m_StartStructure = "ABCDEFG";

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
			// 今のステップでの更新内容
			std::string nowStep_LStructure= "";

			//
			for (int LSNum=0; LSNum< m_LStructure.size(); LSNum++)
			{
				char& now_word = m_LStructure[LSNum];
				//Console::Log("now_word: %c\n", now_word);

				// ルールをチェックする
				for (const auto& Rule : m_LRule) {
					// ターゲットであれば書き換える(nowStep_LStructureにはrResult_LWordを足す)
					if (now_word == Rule.rTarget_LWord)
					{
						//Console::Log("now_word match rTarget_LWord : %c\n",now_word);
						nowStep_LStructure += Rule.rResult_LWord;
					}
					else // 合致しなければそのまま(rTarget_LWordを足す-->もとの文字)
					{
						nowStep_LStructure += Rule.rTarget_LWord;
					}
				}
			}

			// 更新された内容を反映する
			//Console::Log("nowStep_LStructure: %s\n", nowStep_LStructure.c_str());
			m_LStructure = nowStep_LStructure;
		}

		//Console::Log("m_LStructure: %s\n", m_LStructure.c_str());
		//Console::Log("m_LStructure.size(): %d\n", m_LStructure.size());
	}

	void LTree::RunLSystem()
	{
		// LStructureをもとに木のメッシュを作成
		for (const auto& LWord : m_LStructure)
		{
			// LWordをもとにL-Systemアクションを決定する
			if (LWord == 'F') // 先に進んで線を引く
			{
				Console::Log("%c Go Forward and Draw Line\n", LWord);
			}
			else if (LWord == '+') // 時計回りに回転
			{
				Console::Log("%c Rotate Positive\n", LWord);
			}
			else if (LWord == '-') // 半時計周りに回転
			{
				Console::Log("%c Rotate Negative\n", LWord);
			}
			else if (LWord == '[') // 分岐開始
			{
				Console::Log("%c Start Devide\n", LWord);
			}
			else if (LWord == ']') // 分岐終了
			{
				Console::Log("%c End Devide\n", LWord);
			}
			else // 無効な文字列 
			{
				//Console::Log("Invalid char ERROR >>>>>>>>>>>>>>>>>>>>>>>>>> %c\n", LWord); // A Bは素材なため無効な文字列としてきてもおｋ
			}
		}
	}

	void LTree::Update()
	{

	}

	void LTree::Draw() 
	{

	}
}