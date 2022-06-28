#pragma once
#include "../GraphicsMain/GraphicsMain.h"
#include <glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include <memory>

class Texture;

class GraphicsRenderer
{
	GLFWwindow* sWindow;
	int sWindowWidth;
	int sWindowHeight;
	class GraphicsMain* mgame;

	//FrameBuffer
	unsigned int polygon_frameBuffer;
	unsigned int polygon_depthBuffer;
	unsigned int raymarching_frameBuffer;
	unsigned int raymarching_depthBuffer;
	unsigned int p_r_BlendingBuffer;
	//PostProcess FrameBuffer
	unsigned int m_PolygonPostProcess_FrameBuffer;
	unsigned int m_LatePostProcess_FrameBuffer;

	//ポリゴンとレイマーチングのミックス
	std::unique_ptr<class PolygonRaymarchingMixer> m_Mixer;

	// 背景色
	glm::vec4 m_BackgroudColor;
public:
	static GraphicsRenderer* GetInstance() {
		return renderer_instance;
	}
	static void Create();
	static void Destroy();
	static void CheckError();

	GraphicsRenderer(class GraphicsMain* game);
	~GraphicsRenderer();
	bool Initialize(float width, float height);
	void Draw();
	bool CreateFrameBuffer(int width, int height,std::shared_ptr<Texture> fTex,unsigned int& fBuffer, GLint internalformat, GLint format, GLenum type = GL_UNSIGNED_BYTE,bool UseTex=true);
	glm::vec2 GetScreenSize();
	GLFWwindow* GetWindow();
	
	float deltaTime;
	float frameResolusion;

	//FrameTextures
	std::shared_ptr<Texture> polygon_frameTexture;
	std::shared_ptr<Texture> polygon_depthTexture;
	std::shared_ptr<Texture> raymarching_frameTexture;
	std::shared_ptr<Texture> raymarching_depthTexture;
	std::shared_ptr<Texture> p_r_BlendingTexture;
	//PostProcess FrameTexture
	std::shared_ptr<Texture> m_PolygonPostProcess_FrameTexture;
	std::shared_ptr<Texture> m_LatePostProcess_FrameTexture;

	// 背景色
	void SetBackgroudColor(glm::vec4 BackgroudColor);

	friend class PolygonRaymarchingMixer;
private:

protected:
	static GraphicsRenderer* renderer_instance;
};

