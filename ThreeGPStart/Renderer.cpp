#include "Renderer.h"
#include "Camera.h"
#include "ImageLoader.h"
#include <vector>

Renderer::Renderer() 
{

}

// On exit must clean up any OpenGL resources e.g. the program, the buffers
Renderer::~Renderer()
{
	// TODO: clean up any memory used including OpenGL objects via glDelete* calls
	glDeleteProgram(m_program);
	glDeleteBuffers(1, &m_VAO);
}

void Renderer::SetVAO(GLuint tempVao)
{
}

// Use IMGUI for a simple on screen GUI
void Renderer::DefineGUI()
{
	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("3GP");						// Create a window called "3GP" and append into it.

	ImGui::Text("Visibility.");					// Display some text (you can use a format strings too)	

	ImGui::Checkbox("Wireframe", &m_wireframe);	// A checkbox linked to a member variable

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		
	ImGui::End();
}

// Load, compile and link the shaders and create a program object to host them
bool Renderer::CreateProgram()// Program of the shaders of the Terrain and Jeep
{
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program))
		return false;

	return true;
}
bool Renderer::CreateCubeProgram()// Program of the shaders of the cube
{
	// Create a new program (returns a unqiue id)
	m_programCube = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/Cubevertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/Cubefragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_programCube, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_programCube, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_programCube))
		return false;

	return true;
}

bool Renderer::SkyBoxProgram()// Program for the shaders of the skybox
{
	// Create a new program (returns a unqiue id)
	m_programSkyBox = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/SkyBoxvertex_shader.vert") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/SkyBoxfragment_shader.frag") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_programSkyBox, vertex_shader);

	// The attibute location 0 maps to the input stream "vertex_position" in the vertex shader
	// Not needed if you use (location=0) in the vertex shader itself
	//glBindAttribLocation(m_program, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(m_programSkyBox, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_programSkyBox))
		return false;

	return true;
}

// Load / create geometry into OpenGL buffers	
bool Renderer::InitialiseGeometry()
{
	// Load and compile shaders into m_program
	if (!CreateProgram())
		return false;
	if (!CreateCubeProgram())
		return false;
	if (!SkyBoxProgram())
		return false;
	/*
		TODO 1: The first step is to create the vertices, colours and indices
		You could use std::vectors to hold these

		The cube is 20 by 20 centred on 0,0,0
		so the minimum values for x, y, z are -10,-10,-10
		and the maximum values are 10,10,10
	*/
	/*
		TODO 2: Next you need to create VBOs for the vertices and the colours
		You can look back to last week for examples
	*/
	SModel Cube;
	SMesh CubeMesh;
	std::vector<GLfloat> Vertice{//vertice points for the cube
		-10.0f,-10.0f,10.0f, //v0
		10.0f,-10.0f,10.0f, //v1
		10.0f,10.0f,10.0f, //v2
		-10.0f,10.0f,10.0f, //v3

		10.0f,-10.0f,-10.0f, //v7
		-10.0f,-10.0f,-10.0f, //v5
		-10.0f,10.0f,-10.0f, //v4
		10.0f,10.0f,-10.0f, //v6

		-10.0f,-10.0f,-10.0f, //v5
		-10.0f,-10.0f,10.0f, //v0
		-10.0f,10.0f,10.0f, //v3
		-10.0f,10.0f,-10.0f, //v4

		10.0f,-10.0f,10.0f, //v1
		10.0f,-10.0f,-10.0f, //v7
		10.0f,10.0f,-10.0f, //v6
		10.0f,10.0f,10.0f, //v2

		-10.0f,10.0f,10.0f, //v3
		10.0f,10.0f,10.0f, //v2
		10.0f,10.0f,-10.0f, //v6
		-10.0f,10.0f,-10.0f, //v4

		-10.0f,-10.0f,-10.0f, //v5
		10.0f,-10.0f,-10.0f, //v7
		10.0f,-10.0f,10.0f, //v1
		-10.0f,-10.0f,10.0f, //v0
	};
	std::vector<GLfloat>Color =//side colours
	{
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,

		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,

		1.0f,0.65f,0.0f,
		1.0f,0.65f,0.0f,
		1.0f,0.65f,0.0f,
		1.0f,0.65f,0.0f,

		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,

		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,

		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
	};
	CubeMesh.elements = {//order in which the vertices to be rendered
	0,1,2,
	0,2,3,

	4,5,6,
	4,6,7,

	8,9,10,
	8,10,11,

	12,13,14,
	12,14,15,

	16,17,18,
	16,18,19,

	20,21,22,
	20,22,23,
	};

	glGenBuffers(1, &CubeMesh.positionsVBO);//binding to positionvbo
	glBindBuffer(GL_ARRAY_BUFFER, CubeMesh.positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Vertice.size(), Vertice.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &CubeMesh.ColourVBO);//binding to ColourVBO
	glBindBuffer(GL_ARRAY_BUFFER, CubeMesh.ColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * Color.size(), Color.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &CubeMesh.elementsVBO);//binding to elementsVBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeMesh.elementsVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* CubeMesh.elements.size(), CubeMesh.elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	CubeMesh.numElements = CubeMesh.elements.size();

	glGenVertexArrays(1, &CubeMesh.vao);//binding to the vao of the cubeMesh
	glBindVertexArray(CubeMesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, CubeMesh.positionsVBO);//binding the position vbo to it
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, CubeMesh.ColourVBO);//binding the colour vbo to it
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeMesh.elementsVBO);//binding the elements vbo to it

	glBindVertexArray(0);//clearing vertex array

	Cube.meshes.push_back(CubeMesh);//push onto list in which will be rendered
	Models.push_back(Cube);



	SModel Jeep;
	Jeep.modelName = "Data\\Models\\Jeep\\jeep.obj";//initialising data for the jeep
	Models.push_back(Jeep);

	// Load in the jeep
	Helpers::ModelLoader loader;
	if (!loader.LoadFromFile("Data\\Models\\Jeep\\jeep.obj"))
		return false;

	glm::vec3 basicNormal{ 0,0,0 };

	// Now we can loop through all the mesh in the loaded model:
	for (int i = 0; i < Models.size(); i++)//if multiple meshes needed to be loaded
	{
		for (const Helpers::Mesh& mesh : loader.GetMeshVector())
		{
			SMesh current;

			current.numElements = mesh.elements.size();
			current.elements = mesh.elements;

			std::vector<glm::vec3> JeepNormals;
			for (int i = 0; i < current.numElements; i++)
			{
				JeepNormals.push_back(basicNormal);
			}
			for (size_t i = 0; i < current.numElements; i+= 3)//calculating the normals of the jeep so that lighting can properly be applied to it
			{
				glm::vec3 Jvertice1{ mesh.vertices[current.elements[i]] };
				glm::vec3 Jvertice2{ mesh.vertices[current.elements[i+1]] };
				glm::vec3 Jvertice3{ mesh.vertices[current.elements[i+2]] };

				glm::vec3 JTriside1 = Jvertice2 - Jvertice1;
				glm::vec3 JTriside2 = Jvertice3 - Jvertice1;

				glm::vec3 JNormal = glm::normalize(glm::cross(JTriside1, JTriside2));

				JeepNormals[current.elements[i]] += JNormal;
				JeepNormals[current.elements[i + 1]] += JNormal;
				JeepNormals[current.elements[i + 2]] += JNormal;
			}
			for (int i = 0; i < JeepNormals.size(); i++)
			{
				glm::normalize(JeepNormals[i]);
			}
			current.textureName = "Data\\Models\\Jeep\\jeep_rood.jpg";
			// We can extract from the mesh via:
			//mesh.vertices  - a vector of glm::vec3 (3 floats) giving the position of each vertex
			//mesh.elements - a vector of unsigned int defining which vert make up each triangle
			// TODO: create VBO for the vertices and a EBO for the elements
			glGenBuffers(1, &current.positionsVBO);//binding to the positionsvbo
			glBindBuffer(GL_ARRAY_BUFFER, current.positionsVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &current.NormalsVBO);//binding to the NormalsVBO
			glBindBuffer(GL_ARRAY_BUFFER, current.NormalsVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* JeepNormals.size(), JeepNormals.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);


			glGenBuffers(1, &current.UVcoords);//binding to the UVcoords
			glBindBuffer(GL_ARRAY_BUFFER, current.UVcoords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvCoords.size(), mesh.uvCoords.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &current.elementsVBO);//binding to the elementsVBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current.elementsVBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.elements.size(), mesh.elements.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glGenVertexArrays(1, &current.vao);//initialising vao
			glBindVertexArray(current.vao);
			glBindBuffer(GL_ARRAY_BUFFER, current.positionsVBO);//binding positionsVBO to the vao
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


			glBindBuffer(GL_ARRAY_BUFFER, current.UVcoords);//binding UVcoords to the vao
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, current.NormalsVBO);//binding NormalsVBO to the vao
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current.elementsVBO);//binding elementsVBO to the vao

			glBindVertexArray(0);//clearing vertex array

			Helpers::ImageLoader image;

			if (image.Load(current.textureName))//initialising and loading texture
			{
				glGenTextures(1, &current.texture);
				glBindTexture(GL_TEXTURE_2D, current.texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.Width(), image.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.GetData());
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			Models[i].meshes.push_back(current);//pushing back onto list to render
		}
	}

	SModel Terrain;
	SMesh TerrainA;
	TerrainA.textureName = "Data\\Models\\grass.jpg";
	Helpers::ImageLoader Terrainimage;

	if (Terrainimage.Load(TerrainA.textureName))//initialising and loading texture
	{
		glGenTextures(1, &TerrainA.texture);
		glBindTexture(GL_TEXTURE_2D, TerrainA.texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Terrainimage.Width(), Terrainimage.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Terrainimage.GetData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	int numCellsX{ 224 };//grid size
	int numCellsZ{ 224 };

	int numVertsX{ numCellsX + 1 };
	int numVertsZ{ numCellsZ + 1 };
	int numVerts{ numVertsX * numVertsZ };
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> UVCoords;

	std::vector<glm::vec3> Normals;
	bool toggleForDiamondPattern = true;

	Helpers::ImageLoader HeightMap;

	GLboolean Noise = true;

	if (HeightMap.Load("Data\\Heightmaps\\Heightmap.jpg"))//loading a heightmap
	{
		float VertxI = HeightMap.Width() / numCellsX;
		float VertzI = HeightMap.Height() / numCellsZ;
		for (int i = 0; i < numVertsX; i++)
		{
			for (int k = 0; k < numVertsZ; k++)
			{
				float temp = VertxI * i;
				float temp2 = VertzI * k;
				GLint pixelNumber = ((size_t)temp + (size_t)temp2 * HeightMap.Width()) * 4;//getting the offset
				BYTE* data = HeightMap.GetData();
				if (Noise)//if noise is wanted
				{
					verts.push_back(glm::vec3(i * 10, data[pixelNumber]+rand()%10, k * 10));//pushing back the data to the verts list
				}
				else//if noise isnt wanted
				{
					verts.push_back(glm::vec3(i * 10, data[pixelNumber], k * 10));//pushing back the data to the verts list
				}
				UVCoords.push_back(glm::vec2(k / (float)numCellsZ, i / (float)numCellsX));
			}
		}
	}
	else//in case of a failed heightmap load
	{

		for (int i = 0; i < numVertsZ; i++)
		{
			for (int k = 0; k < numVertsX; k++)
			{
				verts.push_back(glm::vec3(i * 10, rand()%10, k * 10));
				UVCoords.push_back(glm::vec2(k / (float)numCellsZ, i / (float)numCellsX));
			}
		}
	}

	for (int i = 0; i < verts.size(); i++)//filling the normal list to be edited later
	{
		Normals.push_back(basicNormal);
	}

	for (int z = 0; z < numCellsZ; z++)
	{
		for (int x = 0; x < numCellsX; x++)
		{
			int startVertIndex = z * numVertsX + x;

			if (toggleForDiamondPattern)//creating the diamond pattern
			{
				TerrainA.elements.push_back(startVertIndex);
				TerrainA.elements.push_back(startVertIndex + 1);
				TerrainA.elements.push_back(startVertIndex + numVertsX);

				TerrainA.elements.push_back(startVertIndex + 1);
				TerrainA.elements.push_back(startVertIndex + numVertsX + 1);
				TerrainA.elements.push_back(startVertIndex + numVertsX);
			}
			else//creating the diamond pattern
			{
				TerrainA.elements.push_back(startVertIndex);
				TerrainA.elements.push_back(startVertIndex + numVertsX+1);
				TerrainA.elements.push_back(startVertIndex + numVertsX);

				TerrainA.elements.push_back(startVertIndex + 1);
				TerrainA.elements.push_back(startVertIndex + numVertsX + 1);
				TerrainA.elements.push_back(startVertIndex);
			}
			toggleForDiamondPattern = !toggleForDiamondPattern;
		}
		toggleForDiamondPattern = !toggleForDiamondPattern;
	}

	for (size_t i = 0; i < TerrainA.elements.size(); i+=3)//calculating normals for the terrain
	{
		glm::vec3 vertice1{verts[TerrainA.elements[i]]};
		glm::vec3 vertice2{verts[TerrainA.elements[i+1]] };
		glm::vec3 vertice3{verts[TerrainA.elements[i+2]] };

		glm::vec3 Triside1 = vertice2 - vertice1;
		glm::vec3 Triside2 = vertice3 - vertice1;

		glm::vec3 Normal = glm::normalize(glm::cross(Triside1, Triside2));

		Normals[TerrainA.elements[i]] += Normal;
		Normals[TerrainA.elements[i+1]] += Normal;
		Normals[TerrainA.elements[i+2]] += Normal;
	}
	for (int i = 0; i < Normals.size(); i++)//normalizing the normals to make sure they are valid values
	{
		glm::normalize(Normals[i]);
	}

	TerrainA.numElements=TerrainA.elements.size();

	glGenBuffers(1, &TerrainA.positionsVBO);//binding to the terrain positionsVBO
	glBindBuffer(GL_ARRAY_BUFFER, TerrainA.positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* numVerts, verts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &TerrainA.UVcoords);//binding to the terrain UVcoords
	glBindBuffer(GL_ARRAY_BUFFER, TerrainA.UVcoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)* UVCoords.size(),UVCoords.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &TerrainA.NormalsVBO);//binding to the terrain NormalsVBO
	glBindBuffer(GL_ARRAY_BUFFER, TerrainA.NormalsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Normals.size(), Normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &TerrainA.elementsVBO);//binding to the terrain elementsVBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TerrainA.elementsVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* TerrainA.elements.size(), TerrainA.elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &TerrainA.vao);//initialising the terrain vao
	glBindVertexArray(TerrainA.vao);
	glBindBuffer(GL_ARRAY_BUFFER, TerrainA.positionsVBO);//committing positionsvbo to the terrain vao
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, TerrainA.UVcoords);//committing UVcoords to the terrain vao
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, TerrainA.NormalsVBO);//committing NormalsVBO to the terrain vao
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TerrainA.elementsVBO);//committing elementsVBO to the terrain vao

	Terrain.meshes.push_back(TerrainA);

	Models.push_back(Terrain);//pushing back the model onto the list in which needs to be rendered
	glBindVertexArray(0);//clearing vertex array buffers

	//loading all skybox images
	std::string back = "Data\\Models\\Sky\\Hills\\skybox_back.jpg";
	std::string bottom = "Data\\Models\\Sky\\Hills\\skybox_bottom.jpg";
	std::string front = "Data\\Models\\Sky\\Hills\\skybox_front.jpg";
	std::string left = "Data\\Models\\Sky\\Hills\\skybox_left.jpg";
	std::string right = "Data\\Models\\Sky\\Hills\\skybox_right.jpg";
	std::string top = "Data\\Models\\Sky\\Hills\\skybox_top.jpg";

	Helpers::ImageLoader Right;
	Helpers::ImageLoader Left;
	Helpers::ImageLoader Top;
	Helpers::ImageLoader Bottom;
	Helpers::ImageLoader Back;
	Helpers::ImageLoader Front;
	glGenTextures(1, &SkyboxTexture);//intialising skyboxtexture location
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture);
	if (Right.Load(right))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, Right.Width(), Right.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Right.GetData());
	}
	if (Left.Load(left))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, Left.Width(), Left.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Left.GetData());
	}
	if (Top.Load(top))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, Top.Width(), Top.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Top.GetData());
	}
	if (Bottom.Load(bottom))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, Bottom.Width(), Bottom.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Bottom.GetData());
	}
	if (Back.Load(back))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, Back.Width(), Back.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Back.GetData());
	}
	if (Front.Load(front))
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, Front.Width(), Front.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Front.GetData());
	}
	//loading all images to the correct locations for the cubemap^^

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//clamping edges to make sure lines cannot be seen and it looks seamless
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	std::vector<GLfloat> SkyboxTings = { -10.0f,  10.0f, -10.0f,//the skybox positions array
		-10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f, -10.0f,
		 10.0f,  10.0f,  10.0f,
		 10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f, -10.0f,
		 10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		 10.0f, -10.0f,  10.0f

	};

	GLuint positionsVBO{};

	glGenBuffers(1, &positionsVBO);//initiliasing the skybox positionsvbo
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * SkyboxTings.size(), SkyboxTings.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &m_VAO);//initilising the skybox vao
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);//binding positionsVBO to the skybox vao
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glBindVertexArray(0);

	return true;
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime)
{			
	// Configure pipeline settings
//	glEnable(GL_DEPTH_TEST); //op3
	glEnable(GL_CULL_FACE);

	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Clear buffers from previous frame
	//glClearColor(0.0f, 0.0f, 0.0f, 0.f); //op1
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);
	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, 1.0f, 5000.0f);

	// Compute camera view matrix and combine with projection matrix for passing to shader
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform = projection_xform * view_xform;

//	glDisable(GL_DEPTH_TEST); //op4
	glUseProgram(m_programSkyBox);//initialising the shader program for the skybox

	glm::mat4 model_xform = glm::mat4(1);

	GLuint SkyBox_Uniform = glGetUniformLocation(m_programSkyBox, "skybox");

	glm::mat4 view_xform2 = glm::mat4(glm::mat3(view_xform));
	glm::mat4 combined_xform2 = projection_xform * view_xform2;

	GLuint model_xform_id2 = glGetUniformLocation(m_programCube, "model_xform");
	glUniformMatrix4fv(model_xform_id2, 1, GL_FALSE, glm::value_ptr(model_xform));

	GLuint combined_xform_id2 = glGetUniformLocation(m_programSkyBox, "combined_xform");
	glUniformMatrix4fv(combined_xform_id2, 1, GL_FALSE, glm::value_ptr(combined_xform2));

	//passing through all the uniform values to the program^^^

	glDepthMask(GL_FALSE);//making sure all depth mask and buffers are off to wrap the skybox around the camera location
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, SkyboxTexture);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//enabling all depth mask and buffers again for rendering all other models
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);//clearing vertex array

	glUseProgram(m_programCube);//using the program for the cube shader files


//	model_xform = glm::mat4(1); // op2
	model_xform = glm::translate(model_xform, glm::vec3{ 2000.0f, 300.0f, 1000.0f });
	GLuint combined_xform_id = glGetUniformLocation(m_programCube, "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));

	static float angle = 0;
	static bool rotateY = true;

	if (rotateY) // Rotate around y axis		
		model_xform = glm::rotate(model_xform, angle, glm::vec3{ 0 ,1,0 });
	else // Rotate around x axis		
		model_xform = glm::rotate(model_xform, angle, glm::vec3{ 1 ,0,0 });

	angle += 0.001f;
	if (angle > glm::two_pi<float>())
	{
		angle = 0;
		rotateY = !rotateY;
	}
	//rotates the cube^^^

	GLuint model_xform_id = glGetUniformLocation(m_programCube, "model_xform");
	glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

	// Bind our VAO and render
	glBindVertexArray(Models[0].meshes[0].vao);//gets the correct files to render
	glDrawElements(GL_TRIANGLES, Models[0].meshes[0].numElements, GL_UNSIGNED_INT, (void*)0);

	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(m_program);
	model_xform = glm::mat4(1);

	glm::vec3 Camera_position = camera.GetPosition();
	GLuint Camera_position_id = glGetUniformLocation(m_program, "CameraPosition");
	glUniform3fv(Camera_position_id, 1, glm::value_ptr(Camera_position));

	// Send the combined matrix to the shader in a uniform
	combined_xform_id = glGetUniformLocation(m_program, "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));
	

	for (int i = 1; i < Models.size(); i++)//going through all the models in the model list
	{
		for (int x = 0; x < Models[i].meshes.size(); x++)//going through all the meshes in the mesh list
		{
			GLuint model_xform_id = glGetUniformLocation(m_program, "model_xform");
			glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));

			glActiveTexture(GL_TEXTURE0);//loading the texture for that model
			glBindTexture(GL_TEXTURE_2D, Models[i].meshes[x].texture);
			GLuint samplerd = glGetUniformLocation(m_program, "sampler_tex");
			glUniform1i(samplerd, 0);

			// Bind our VAO and render
			glBindVertexArray(Models[i].meshes[x].vao);//passing through the vao
			glDrawElements(GL_TRIANGLES, Models[i].meshes[x].numElements, GL_UNSIGNED_INT, (void*)0);//passing through the number of elements
		}
	}

}

