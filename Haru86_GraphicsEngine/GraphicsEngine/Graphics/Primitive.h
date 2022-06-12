#pragma once
#include <vector>
#include <string>

class Primitive
{
public:
	Primitive(const std::vector<float>& vertices,const std::vector<int>& indices);
	Primitive(const std::vector<std::vector<float>>& vertices,const std::vector<int>& dimention,const std::vector<unsigned short>& indices);
	~Primitive();
	unsigned int GetNumIndices() { return mNumIndices; }
	int GetVertNum() { return vertNum; }
	void CreatePrimitive(const std::vector<float>& vertices,const std::vector<int>& indices);
	void CreatePrimitive(const std::vector<std::vector<float>>& vertices,const std::vector<int>& dimention,const std::vector<unsigned short>& indices);
	void Draw();
	void Draw(std::string drawType);
	
	const std::vector<std::vector<float>>& GetVertices()const;
	const std::vector<int>& GetDimentions()const;
	const std::vector<unsigned short>& GetIndices()const;
	
	void SetActive();
	void UnLoadData();

	void Release();

	static void CreateBoard(std::vector<float>* vertices,std::vector<int>* indices);
	static void CreateBoard(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices);
	static void CreateCube(std::vector<float>* vertices, std::vector<int>* indices);
	static void CreateCube(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices);
	static void CreateSphere(std::vector<float>* vertices, std::vector<int>* indices, float row = 16.0f, float column=16.0f,float rad=1.0f);
	static void CreateSphere(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices, float row = 16.0f, float column=16.0f,float rad=1.0f);
	static void CreatePoint(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices);
private:
	unsigned int mNumIndices;
	unsigned int mVertexArray;
	int vertNum;

	std::vector<std::vector<float>> m_vertices;
	std::vector<int> m_dimention;
	std::vector<unsigned short> m_indices;
};

