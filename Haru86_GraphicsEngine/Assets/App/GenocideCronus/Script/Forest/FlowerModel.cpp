#include "FlowerModel.h"
#include "Flower.h"
#include "Stem.h"
#include "Leaf.h"

namespace myapp {
	FlowerModel::FlowerModel() :
		count(2048),
		m_Flower(nullptr),
		m_Stem(nullptr),
		m_Leaf(nullptr),
		flowersIsDone(false),
		stemIsDone(false),
		leafIsDone(false)
	{
		Start();
	}

	void FlowerModel::Start() {
		// 初期化
		m_Stem = std::make_shared<Stem>(this);
		stemIsDone = true;
		m_Leaf = std::make_shared<Leaf>(this);
		leafIsDone = true;
		m_Flower = std::make_shared<Flower>(this);
		flowersIsDone = true;

		// バッファとコンピュートシェーダー / バッファとマテリアル をそれぞれ結び付ける(BufferIndexが必要なため初期化後に割り当てる)
		m_Stem->LinkBufferToResources();
		m_Leaf->LinkBufferToResources(m_Stem);
		m_Flower->LinkBufferToResources(m_Stem);

		// 初回のDispatch(初期化コンピュートシェーダーの計算)
		// 上記のバインド後に行わないとBufferがなくて描画がおかしくなる(エラーはでないがそうなる)
		m_Stem->InitializeDispatch();
		m_Leaf->InitializeDispatch();
		m_Flower->InitializeDispatch();
	}

	void FlowerModel::Update() {
		m_Stem->Update();
		m_Leaf->Update();
		m_Flower->Update();
	}

	void FlowerModel::Draw() {
		m_Stem->Draw();
		m_Leaf->Draw();
		m_Flower->Draw();
	}
}