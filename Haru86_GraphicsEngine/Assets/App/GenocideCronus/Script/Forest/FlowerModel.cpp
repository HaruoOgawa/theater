#include "FlowerModel.h"
#include "Flower.h"
#include "Stem.h"
#include "Leaf.h"

namespace myapp {
	FlowerModel::FlowerModel() :
		count(2048),
		m_Flower(nullptr),
		m_Stem(nullptr),
		m_Leaf(nullptr)
	{
		Start();
	}

	void FlowerModel::Start() {
		// 初期化
		m_Flower = std::make_shared<Flower>(this);
		m_Stem = std::make_shared<Stem>(this);
		m_Leaf = std::make_shared<Leaf>(this);

		// バッファとコンピュートシェーダー / バッファとマテリアル をそれぞれ結び付ける(BufferIndexが必要なため初期化後に割り当てる)
		m_Stem->LinkBufferToResources();
		m_Flower->LinkBufferToResources(m_Stem);
		m_Leaf->LinkBufferToResources(m_Stem);
	}

	void FlowerModel::Update() {
		m_Flower->Update();
		m_Stem->Update();
		m_Leaf->Update();
	}

	void FlowerModel::Draw() {
		m_Flower->Draw();
		m_Stem->Draw();
		m_Leaf->Draw();
	}
}