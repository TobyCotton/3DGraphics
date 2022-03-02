#include "Model.h"

Model::Model(std::string File, std::string texture, std::vector<Helpers::Mesh> MeshList)
{
    if (!m_loader.LoadFromFile(File))
        fail++;
    m_MeshList = MeshList;
    m_texture = texture;
}

void Model::Binding(std::vector<Helpers::Mesh> ModelV)
{
    for (const Helpers::Mesh& mesh : m_loader.GetMeshVector())
    {
        Mesh ting;
        m_DataMesh.push_back(ting);
        glGenBuffers(1, &ting.m_positionsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, ting.m_positionsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.vertices.size(), mesh.vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &ting.m_UVcoords);
        glBindBuffer(GL_ARRAY_BUFFER, ting.m_UVcoords);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.uvCoords.size(), mesh.uvCoords.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &ting.m_elementsVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ting.m_elementsVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.elements.size(), mesh.elements.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

GLuint Model::ReturnElement(GLuint i)
{
    return m_DataMesh[i].m_elementNumber;
}
