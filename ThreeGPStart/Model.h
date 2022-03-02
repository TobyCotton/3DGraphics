#pragma once
#include "ExternalLibraryHeaders.h"
#include <vector>
#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"
#include "ImageLoader.h"
#include <string>
class Model
{
protected:
	struct Mesh
	{
	public:
		GLuint m_positionsVBO;
		GLuint m_UVcoords;
		GLuint m_elementsVBO;
		GLuint m_elementNumber{0};
		GLuint m_VAOM{ 0 };
	};
private:
	Helpers::ModelLoader m_loader;
	Helpers::ImageLoader m_image;
	std::string m_texture;
	std::vector<Mesh> m_DataMesh;
	std::vector<Helpers::Mesh> m_MeshList;
	GLuint fail{ 0 };
public:
	Model(std::string File,std::string texture, std::vector<Helpers::Mesh> MeshList);
	void Binding(std::vector<Helpers::Mesh> ModelV);
	GLuint ReturnElement(GLuint i);
};

