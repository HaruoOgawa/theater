#include "FlowerModel.h"
#include "Flower.h"
#include "Stem.h"
#include "Leaf.h"

namespace myapp {
	FlowerModel::FlowerModel() :
		m_Flower(nullptr),
		m_Stem(nullptr),
		m_Leaf(nullptr)
	{
		Start();
	}

	void FlowerModel::Start() {
		m_Flower = std::make_shared<Flower>();
		m_Stem = std::make_shared<Stem>();
		m_Leaf = std::make_shared<Leaf>();
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