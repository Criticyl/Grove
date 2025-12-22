#pragma once

#include "Voxel.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <FastNoiseLite.h>


namespace Grove {
    class Shader;
    class VAO;
    class VBO;
    class EBO;
}

namespace Grove {

    const int CHUNK_SIZE = 64;
    const int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    class Chunk {
    public:
        Chunk(glm::vec3 position, FastNoiseLite& terrainNoise, FastNoiseLite& grassNoise, FastNoiseLite& stoneNoise);
        ~Chunk();

        void generateMesh(const Chunk* left, const Chunk* right, const Chunk* front, const Chunk* back);
        void render(Shader& shader);

        Voxel getVoxel(int x, int y, int z) const;
        void setVoxel(int x, int y, int z, std::uint8_t id);

    private:

        bool isAir(int x, int y, int z) const;
        void addFace(const std::vector<float>& faceVertices, int x, int y, int z, int faceID);
        int getTerrainHeight(int globalX, int globalY, int globalZ) const;
        float calculateVertexAO(bool side1, bool side2, bool corner);
        void generateTerrain();

    private:

        Voxel m_Voxels[CHUNK_VOLUME];
        glm::vec3 m_Position;

        FastNoiseLite m_TerrainNoise;
        FastNoiseLite m_GrassNoise;
        FastNoiseLite m_StoneNoise;

        std::unique_ptr<VAO> m_VAO;
        std::unique_ptr<VBO> m_VBO;
        std::unique_ptr<VBO> m_ColVBO;
        std::unique_ptr<VBO> m_AOVBO;
        std::unique_ptr<EBO> m_EBO;

        unsigned int m_IndexCount = 0;
    };
}
