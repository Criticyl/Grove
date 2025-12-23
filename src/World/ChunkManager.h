#pragma once

#include <map>
#include <vector>
#include <memory>
#include <deque>
#include <set>
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

        void update(glm::vec3 playerPos, int workSize);
        void updateChunk(int x, int z);
        void render(Shader& shader);

    public:

        const float VOXEL_SIZE = 0.2f;
        const int RENDER_DISTANCE = 8;


    private:

        std::map<ChunkCoord, std::unique_ptr<Chunk>> m_Chunks;
        std::deque<ChunkCoord> m_MeshQueue;
        std::set<ChunkCoord> m_MeshQueueSet;
        FastNoiseLite m_TerrainNoise;
        FastNoiseLite m_GrassNoise;
        FastNoiseLite m_StoneNoise;
        FastNoiseLite m_ContinentNoise;
        FastNoiseLite m_MountainNoise;

        void createChunk(int x, int z);
        Chunk* getChunkPtr(int x, int z);

    };
}