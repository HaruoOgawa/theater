#pragma once
#include <GL/glew.h>
#include <GL/glut.h>
#include <memory>

static class Graphics
{
public:
	static void DrawInstancedWithMesh(std::shared_ptr<class Mesh> mesh,int count,std::shared_ptr<class Material> material, GLenum rendermode);
	
};

