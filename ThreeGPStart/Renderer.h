#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

struct SMesh//my mesh struct used throughout the code
{
public:
	GLuint vao;
	std::vector<GLuint>elements{};
	GLuint numElements{ 0 };
	GLuint texture;
	GLuint positionsVBO;
	GLuint UVcoords;
	GLuint elementsVBO;
	GLuint NormalsVBO;
	GLuint ColourVBO;
	std::string textureName{};
};
struct SModel//model struct use to handle the meshes
{
public:
	std::vector<SMesh> meshes{};
	std::string modelName{};
};

class Renderer
{
private:
	// Program object - to host shaders for the terrain and jeep
	GLuint m_program{ 0 };
	GLuint m_programCube{ 0 };// Program object - to host shaders for the cube
	GLuint m_programSkyBox{ 0 };// Program object - to host shaders for the skybox

	GLuint SkyboxTexture;

	std::vector<SModel> Models{};

	// Vertex Array Object to wrap all render settings
	GLuint m_VAO{ 0 };

	// Number of elments to use when rendering
	GLuint m_numElements{ 0};
	GLuint texture;

	bool m_wireframe{ false };

	bool CreateProgram();
	bool CreateCubeProgram();
	bool SkyBoxProgram();
public:
	Renderer();
	~Renderer();

	void SetVAO(GLuint tempVao);

	// Draw GUI
	void DefineGUI();

	// Create and / or load geometry, this is like 'level load'
	bool InitialiseGeometry();

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime);
};

