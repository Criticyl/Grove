#pragma once

#include <map>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <FastNoiseLite.h>

#include "Chunk.h"
#include "../Renderer/Shader.h"

namespace Grove {

    struct ChunkCoord {
        int x, z;

        bool operator<(const ChunkCoord& otherChunk) const {
            if (x != otherChunk.x) return x < otherChunk.x;
            return z < otherChunk.z;
        }
    };

    class ChunkManager {
    public:
        ChunkManager();
        ~ChunkManager();

        void update(glm::vec3 playerPos);
        void render(Shader& shader);

    public:

        const float VOXEL_SIZE = 0.2f;
        const int RENDER_DISTANCE = 8;


    private:

        std::map<ChunkCoord, std::unique_ptr<Chunk>> m_Chunks;
        FastNoiseLite m_TerrainNoise;
        FastNoiseLite m_GrassNoise;
        FastNoiseLite m_StoneNoise;

        void createChunk(int x, int z);

    };
}