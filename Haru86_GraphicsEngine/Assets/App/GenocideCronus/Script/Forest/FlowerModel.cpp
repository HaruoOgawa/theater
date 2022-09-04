#include "FlowerModel.h"
#include "Flower.h"
#include "Stem.h"

namespace myapp {
	FlowerModel::FlowerModel() :
		count(1024),
		m_Flower(nullptr),
		m_Stem(nullptr),
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
		leafIsDone = true;
		m_Flower = std::make_shared<Flower>(this);
		flowersIsDone = true;

		// バッファとコンピュートシェーダー / バッファとマテリアル をそれぞれ結び付ける(BufferIndexが必要なため初期化後に割り当てる)
		m_Stem->LinkBufferToResources();
		m_Flower->LinkBufferToResources(m_Stem);

		// 初回のDispatch(初期化コンピュートシェーダーの計算)
		// 上記のバインド後に行わないとBufferがなくて描画がおかしくなる(エラーはでないがそうなる)
		m_Stem->InitializeDispatch();
		m_Flower->InitializeDispatch();
	}

	void FlowerModel::Update() {
		m_Stem->Update();
		m_Flower->Update();
	}

	void FlowerModel::Draw() {
		m_Stem->Draw();
		m_Flower->Draw();
	}
}