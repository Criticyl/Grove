#include "Chunk.h"
#include <glad/glad.h>
#include "../Renderer/Shader.h"
#include "../Renderer/VAO.h"
#include "../Renderer/VBO.h"
#include "../Renderer/EBO.h"


namespace Grove {

    struct AONeighbours {
        int s1[3];
        int s2[3];
        int c[3];
    };

    Chunk::Chunk(glm::vec3 position, FastNoiseLite& terrainNoise, FastNoiseLite& grassNoise, FastNoiseLite& stoneNoise)
        : m_Position(position), m_TerrainNoise(terrainNoise), m_GrassNoise(grassNoise), m_StoneNoise(stoneNoise)
    {

        for (int i = 0; i < CHUNK_VOLUME; i++) {
            m_Voxels[i].ID = 0;
        }

        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                float globalX = m_Position.x + x;
                float globalZ = m_Position.z + z;

                int height = getTerrainHeight(globalX, globalZ);

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
        if (y < 0) return false;
        if (y >= CHUNK_SIZE) return true;

        if (x < 0 || x >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) {
            int globalX = (int)std::floor(m_Position.x + x);
            int globalZ = (int)std::floor(m_Position.z + z);

            int height = getTerrainHeight(globalX, globalZ);

            return y >= height;

        }

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

    inline AONeighbours getAONeighbors(int faceID, int vertexIndex) {

        if (faceID == 2) {
            if (vertexIndex == 0) return { {-1, 1, 0}, {0, 1, 1}, {-1, 1, 1} };
            if (vertexIndex == 1) return { {1, 1, 0}, {0, 1, 1}, {1, 1, 1} };
            if (vertexIndex == 2) return { {1, 1, 0}, {0, 1, -1}, {1, 1, -1} };
            if (vertexIndex == 3) return { {-1, 1, 0}, {0, 1, -1}, {-1, 1, -1} };
        }

        if (faceID == 3) {
            if (vertexIndex == 0) return { {-1, -1, 0}, {0, -1, 1}, {-1, -1, 1} };
            if (vertexIndex == 1) return { {1, -1, 0}, {0, -1, 1}, {1, -1, 1} };
            if (vertexIndex == 2) return { {1, -1, 0}, {0, -1, -1}, {1, -1, -1} };
            if (vertexIndex == 3) return { {-1, -1, 0}, {0, -1, -1}, {-1, -1, -1} };
        }

        if (faceID == 0) {
            if (vertexIndex == 0) return { {1,1,0}, {1,0,-1}, {1,1,-1} };
            if (vertexIndex == 1) return { {1,-1,0}, {1,0,-1}, {1,-1,-1} };
            if (vertexIndex == 2) return { {1,-1,0}, {1,0,1}, {1,-1,1} };
            if (vertexIndex == 3) return { {1,1,0}, {1,0,1}, {1,1,1} };
        }

        if (faceID == 1) {
            if (vertexIndex == 0) return { {-1,1,0}, {-1,0,1}, {-1,1,1} };
            if (vertexIndex == 1) return { {-1,-1,0}, {-1,0,1}, {-1,-1,1} };
            if (vertexIndex == 2) return { {-1,-1,0}, {-1,0,-1}, {-1,-1,-1} };
            if (vertexIndex == 3) return { {-1,1,0}, {-1,0,-1}, {-1,1,-1} };
        }

        if (faceID == 4) {
            if (vertexIndex == 0) return { {0,1,1}, {-1,0,1}, {-1,1,1} };
            if (vertexIndex == 1) return { {0,-1,1}, {-1,0,1}, {-1,-1,1} };
            if (vertexIndex == 2) return { {0,-1,1}, {1,0,1}, {1,-1,1} };
            if (vertexIndex == 3) return { {0,1,1}, {1,0,1}, {1,1,1} };
        }

        if (faceID == 5) {
            if (vertexIndex == 0) return { {0,1,-1}, {1,0,-1}, {1,1,-1} };
            if (vertexIndex == 1) return { {0,-1,-1}, {1,0,-1}, {1,-1,-1} };
            if (vertexIndex == 2) return { {0,-1,-1}, {-1,0,-1}, {-1,-1,-1} };
            if (vertexIndex == 3) return { {0,1,-1}, {-1,0,-1}, {-1,1,-1} };
        }


        return { {0,0,0}, {0,0,0}, {0,0,0} };
    }
    
    float Chunk::calculateVertexAO(bool side1, bool side2, bool corner) {

        int neighbors = 0;
        if (side1) neighbors++;
        if (side2) neighbors++;
        if (corner) neighbors++;



        if (neighbors == 0) return 1.0f;
        if (neighbors == 1) return 0.8f;
        if (neighbors == 2) return 0.65f;
        return 0.5f;
    }

    void Chunk::addFace(const std::vector<float>& faceVertices, int x, int y, int z, int faceID) {
        unsigned int offset = m_Vertices.size() / 3;

        Voxel voxel = getVoxel(x, y, z);
        glm::vec3 baseColour = voxel.getBaseColour();

        float globalX = m_Position.x + x;
        float globalY = m_Position.y + y;
        float globalZ = m_Position.z + z;

        float noiseValue;
        switch (voxel.ID) {
            case 1: 
                noiseValue = m_GrassNoise.GetNoise(globalX, globalY, globalZ); 
                break;
            case 2: 
                noiseValue = m_StoneNoise.GetNoise(globalX, globalY, globalZ); 
                break;
            default:
                noiseValue = 1.0f;
                break;
        }

        float variance = 0.1f;
        glm::vec3 col = baseColour + (glm::vec3(noiseValue) * variance);
        col = glm::clamp(col, 0.0f, 1.0f);

        for (int i = 0; i < 4; i++) {


            m_Vertices.push_back(faceVertices[i * 3] + x);
            m_Vertices.push_back(faceVertices[i * 3 + 1] + y);
            m_Vertices.push_back(faceVertices[i * 3 + 2] + z);

            m_Colours.push_back(col.r);
            m_Colours.push_back(col.g);
            m_Colours.push_back(col.b);
            /*if (i == 0) {
                m_Colours.push_back(1.0f); 
                m_Colours.push_back(0.0f);
                m_Colours.push_back(0.0f);
            }
            if (i == 1) {
                m_Colours.push_back(0.0f);
                m_Colours.push_back(1.0f);
                m_Colours.push_back(0.0f);
            }
            if (i == 2) {
                m_Colours.push_back(0.0f);
                m_Colours.push_back(0.0f);
                m_Colours.push_back(1.0f);
            }
            if (i == 3) {
                m_Colours.push_back(1.0f);
                m_Colours.push_back(0.0f);
                m_Colours.push_back(1.0f);
            }*/


            AONeighbours n = getAONeighbors(faceID, i);

            // 2. Check Solids (!isAir)
            bool side1 = !isAir(x + n.s1[0], y + n.s1[1], z + n.s1[2]);
            bool side2 = !isAir(x + n.s2[0], y + n.s2[1], z + n.s2[2]);
            bool corner = !isAir(x + n.c[0], y + n.c[1], z + n.c[2]);

            float ao = calculateVertexAO(side1, side2, corner);
            m_AO.push_back(ao);

        }

        float ao0 = m_AO[offset + 0];
        float ao1 = m_AO[offset + 1];
        float ao2 = m_AO[offset + 2];
        float ao3 = m_AO[offset + 3];

        float dist1 = abs(ao0 - ao2);
        float dist2 = abs(ao1 - ao3);

        if (dist1 > dist2) {
            m_Indices.push_back(offset + 1);
            m_Indices.push_back(offset + 2);
            m_Indices.push_back(offset + 3);
            m_Indices.push_back(offset + 3);
            m_Indices.push_back(offset + 0);
            m_Indices.push_back(offset + 1);
        }
        else {
            m_Indices.push_back(offset + 0);
            m_Indices.push_back(offset + 1);
            m_Indices.push_back(offset + 2);
            m_Indices.push_back(offset + 2);
            m_Indices.push_back(offset + 3);
            m_Indices.push_back(offset + 0);
        }

       

    }

    int Chunk::getTerrainHeight(int globalX, int globalZ) const {
        float noiseValue = m_TerrainNoise.GetNoise((float)globalX, (float)globalZ);
        float scaledHeight = (noiseValue + 1.0f) * 0.5f * 32.0f;
        int height = (int)(scaledHeight + 0.1f);
        if (height < 2) height = 2;

        return height;
    }

    void Chunk::generateMesh() {
        m_Vertices.clear();
        m_Indices.clear();
        m_Colours.clear();
        m_AO.clear();

        m_Vertices.shrink_to_fit();
        m_Indices.shrink_to_fit();
        m_Colours.shrink_to_fit();
        m_AO.shrink_to_fit();

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
            m_AOVBO = std::make_unique<VBO>(m_AO);
            m_EBO = std::make_unique<EBO>(m_Indices);

            m_VAO->linkAttribute(*m_VBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
            m_VAO->linkAttribute(*m_ColVBO, 1, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
            m_VAO->linkAttribute(*m_AOVBO, 2, 1, GL_FLOAT, sizeof(float), (void*)0);

            m_VAO->unbind();
            m_VBO->unbind();
            m_ColVBO->unbind();
            m_AOVBO->unbind();
            m_EBO->unbind();

            m_MeshDirty = false;
        }

        if (m_Indices.size() > 0) {

            m_VAO->bind();

            float scale = 0.2f;
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, m_Position * scale);
            model = glm::scale(model, glm::vec3(scale));

            shader.setMat4("model", model);

            glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        }
    }
}