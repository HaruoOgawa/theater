#pragma once
#include <memory>

class Texture;
class CBloom;
class Mesh;
class Material;

class PostProcess
{
	//base
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
	std::shared_ptr<Material> m_LateMaterial;
	bool m_UsePostProcess;

	//bloom
	bool m_UseBloom;
	float m_BloomIntensity;
	float m_BloomThreshold;
	std::unique_ptr<CBloom> m_Bloom;
	std::shared_ptr<Texture> m_BloomTexture;
	unsigned int m_BloomFrameBuffer;

public:
	static void CreateInstance();
	static PostProcess* GetInstance();
	static void DestroyInstance();
	
	PostProcess();
	~PostProcess();
	void EnablePostProcess();
	const bool GetUsePostProcess()const;
	void DrawPolygonPostProcess(const std::shared_ptr<Texture> SrcTexture,const unsigned int& DestBuffer)const;
	void DrawLatePostProcess(const std::shared_ptr<Texture> SrcTexture, const unsigned int& DestBuffer)const;
	void EnableBloom(float bloomThreshold,float bloomIntensity);
	
	const float GetBloomThreshold()const;
	const float GetBloomIntensity()const;

private:
	void Initialize();
	void Release();
	void DrawBloom();
protected:
	static PostProcess* instance;
};

