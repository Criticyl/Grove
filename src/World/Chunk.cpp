#include "Chunk.h"
#include <glad/glad.h>
#include "../Renderer/Shader.h"
#include "../Renderer/VAO.h"
#include "../Renderer/VBO.h"
#include "../Renderer/EBO.h"


namespace Grove {

    Chunk::Chunk(glm::vec3 position, FastNoiseLite& noise)
        : m_Position(position)
    {

        for (int i = 0; i < CHUNK_VOLUME; i++) {
            m_Voxels[i].ID = 0;
        }

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                float globalX = m_Position.x + x;
                float globalZ = m_Position.z + z;

                float noiseValue = noise.GetNoise(globalX, globalZ);
                int height = (int)((noiseValue + 1.0f) * 0.5f * 32.0f);
                if (height < 2) height = 2;

                for (int y = 0; y < height; y++) {
                    int voxelID = (y == height - 1) ? 1 : 2;
                    setVoxel(x, y, z, voxelID);
                }

            }
        }

        m_VAO = std::make_unique<VAO>();
    }

    Chunk::~Chunk() {
    }

    bool Chunk::isAir(int x, int y, int z) const {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) return true;
        return getVoxel(x, y, z).isAir();
    }

    void Chunk::setVoxel(int x, int y, int z, std::uint8_t id) {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
            return;

        int index = x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_SIZE);
        m_Voxels[index].ID = id;
    }

    Voxel Chunk::getVoxel(int x, int y, int z) const {
        if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
            return { 0 };

        int index = x + (y * CHUNK_SIZE) + (z * CHUNK_SIZE * CHUNK_SIZE);
        return m_Voxels[index];
    }

    void Chunk::addFace(const std::vector<float>& faceVertices, int x, int y, int z, int faceID) {
        unsigned int offset = m_Vertices.size() / 3;

        Voxel voxel = getVoxel(x, y, z);
        glm::vec3 col = voxel.getNoiseColour(x, y, z);

        for (int i = 0; i < 4; i++) {
            m_Vertices.push_back(faceVertices[i * 3] + x);
            m_Vertices.push_back(faceVertices[i * 3 + 1] + y);
            m_Vertices.push_back(faceVertices[i * 3 + 2] + z);

            m_Colours.push_back(col.r);
            m_Colours.push_back(col.g);
            m_Colours.push_back(col.b);
        }

        m_Indices.push_back(offset + 0);
        m_Indices.push_back(offset + 1);
        m_Indices.push_back(offset + 2);
        m_Indices.push_back(offset + 2);
        m_Indices.push_back(offset + 3);
        m_Indices.push_back(offset + 0);

    }

    void Chunk::generateMesh() {
        m_Vertices.clear();
        m_Indices.clear();
        m_Colours.clear();

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    if (isAir(x, y, z)) continue;

                    if (isAir(x + 1, y, z)) {
                        std::vector<float> face = {
                            0.5f, 0.5f, -0.5f,
                            0.5f, -0.5f, -0.5f,
                            0.5f, -0.5f, 0.5f,
                            0.5f, 0.5f, 0.5f
                        };
                        addFace(face, x, y, z, 0);
                    }

                    if (isAir(x - 1, y, z)) {
                        std::vector<float> face = {
                            -0.5f, 0.5f, 0.5f,
                            -0.5f, -0.5f, 0.5f,
                            -0.5f, -0.5f, -0.5f,
                            -0.5f, 0.5f, -0.5f
                        };
                        addFace(face, x, y, z, 1);
                    }

                    if (isAir(x, y + 1, z)) {
                        std::vector<float> face = {
                            -0.5f, 0.5f, 0.5f,
                            0.5f, 0.5f, 0.5f,
                            0.5f, 0.5f, -0.5f,
                            -0.5f, 0.5f, -0.5f
                        };
                        addFace(face, x, y, z, 2);
                    }

                    if (isAir(x, y - 1, z)) {
                        std::vector<float> face = {
                            -0.5f, -0.5f, -0.5f,
                            0.5f, -0.5f, -0.5f,
                            0.5f, -0.5f, 0.5f,
                            -0.5f, -0.5f, 0.5f
                        };
                        addFace(face, x, y, z, 3);
                    }

                    if (isAir(x, y, z + 1)) {
                        std::vector<float> face = {
                            -0.5f, 0.5f, 0.5f,
                            -0.5f, -0.5f, 0.5f,
                            0.5f, -0.5f, 0.5f,
                            0.5f, 0.5f, 0.5f
                        };
                        addFace(face, x, y, z, 4);
                    }

                    if (isAir(x, y, z - 1)) {
                        std::vector<float> face = {
                            0.5f, 0.5f, -0.5f,
                            0.5f, -0.5f, -0.5f,
                            -0.5f, -0.5f, -0.5f,
                            -0.5f, 0.5f, -0.5f
                        };
                        addFace(face, x, y, z, 5);
                    }
                }
            }
        }

        m_MeshDirty = true;
    }

    void Chunk::render(Shader& shader) {
        if (m_MeshDirty) {
            m_VAO->bind();
            m_VBO = std::make_unique<VBO>(m_Vertices);
            m_ColVBO = std::make_unique<VBO>(m_Colours);
            m_EBO = std::make_unique<EBO>(m_Indices);

            m_VAO->linkAttribute(*m_VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
            m_VAO->linkAttribute(*m_ColVBO, 1, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

            m_VAO->unbind();
            m_VBO->unbind();
            m_ColVBO->unbind();
            m_EBO->unbind();

            m_MeshDirty = false;
        }

        if (m_Indices.size() > 0) {

            m_VAO->bind();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_Position);

            float scale = 0.2f;
            model = glm::mat4(1.0f);
            model = glm::translate(model, m_Position * scale);
            model = glm::scale(model, glm::vec3(scale));

            shader.setMat4("model", model);

            glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        }
    }
}