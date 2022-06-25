#pragma once
#include <vector>
#include <string>

class Primitive
{
public:
	Primitive(const std::vector<std::vector<float>>& vertices,const std::vector<int>& dimention,const std::vector<unsigned short>& indices);
	~Primitive();
	void CreatePrimitive(const std::vector<std::vector<float>>& vertices,const std::vector<int>& dimention,const std::vector<unsigned short>& indices);
	void Draw();
	
	void SetActive();

	static void CreateBoard(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices);
	static void CreatePoint(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices);

	unsigned int mNumIndices;
private:
	unsigned int mVertexArray;
	int vertNum;

	std::vector<std::vector<float>> m_vertices;
	std::vector<int> m_dimention;
	std::vector<unsigned short> m_indices;
};

