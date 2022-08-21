#pragma once
#include <memory>
#include <functional>

class Texture;
class CBloom;
class Mesh;
class Material;
class TransformComponent;
class MeshRendererComponent;

class PostProcess
{
	//base
	std::shared_ptr<Mesh> m_mesh;
	std::shared_ptr<Material> m_material;
	
	//bloom
	std::unique_ptr<CBloom> m_Bloom;
	std::shared_ptr<Texture> m_BloomTexture;
	unsigned int m_BloomFrameBuffer;

	// VPMatrixŽæ“¾—pTRS
	std::shared_ptr<TransformComponent> m_transform;

public:
	static void CreateInstance();
	static PostProcess* GetInstance();
	static void DestroyInstance();
	
	PostProcess();
	virtual ~PostProcess()=default;
	void DrawPolygonPostProcess(const std::shared_ptr<Texture>& SrcTexture,const unsigned int& DestBuffer)const;
	void DrawLatePostProcess(const std::shared_ptr<Texture>& SrcTexture, const unsigned int& DestBuffer)const;
	
	//
	bool m_UsePostProcess;
	std::shared_ptr<MeshRendererComponent> m_LateMeshRenderer;
	std::function<void(void)> m_LatePostProcesCallBack;

	//bloom
	bool m_UseBloom;
	float m_BloomIntensity;
	float m_BloomThreshold;

	// SSR
	bool m_UseSSR;
protected:
	static PostProcess* instance;
};

