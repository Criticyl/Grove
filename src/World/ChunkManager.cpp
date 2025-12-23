#include "ChunkManager.h"

namespace Grove {

    ChunkManager::ChunkManager() {

        m_TerrainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_TerrainNoise.SetSeed(12);
        m_TerrainNoise.SetFractalType(FastNoiseLite::FractalType_Ridged);
        m_TerrainNoise.SetFractalOctaves(4);
        m_TerrainNoise.SetFractalLacunarity(2.0f);
        m_TerrainNoise.SetFractalGain(0.5f);
        m_TerrainNoise.SetFrequency(0.003f);

        m_GrassNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        m_GrassNoise.SetSeed(33);
        m_GrassNoise.SetFrequency(0.05f);

        m_StoneNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
        m_StoneNoise.SetSeed(1);
        m_StoneNoise.SetFrequency(0.1f);
        m_StoneNoise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);

    }

    ChunkManager::~ChunkManager() {
        m_Chunks.clear();
    }

    Chunk* ChunkManager::getChunkPtr(int x, int z) {
        auto it = m_Chunks.find({ x, z });
        if (it != m_Chunks.end()) return it->second.get();
        return nullptr;
    }

    void ChunkManager::createChunk(int x, int z) {
        ChunkCoord coord = { x, z };

        float worldX = x * CHUNK_SIZE;
        float worldZ = z * CHUNK_SIZE;

        std::unique_ptr<Chunk> newChunk = std::make_unique<Chunk>(glm::vec3(worldX, 0.0f, worldZ), m_TerrainNoise, m_GrassNoise, m_StoneNoise);

        m_Chunks[coord] = std::move(newChunk);
        updateChunk(x, z);
        if (getChunkPtr(x - 1, z)) updateChunk(x - 1, z);
        if (getChunkPtr(x + 1, z)) updateChunk(x + 1, z);
        if (getChunkPtr(x, z - 1)) updateChunk(x, z - 1);
        if (getChunkPtr(x, z + 1)) updateChunk(x, z + 1);
    }

    void ChunkManager::updateChunk(int x, int z) {
        ChunkCoord coord = { x, z };

        if (m_MeshQueueSet.find(coord) == m_MeshQueueSet.end()) {
            m_MeshQueue.push_back(coord);
            m_MeshQueueSet.insert(coord);
        }
        
    }

    void ChunkManager::update(glm::vec3 playerPos, int workSize = 1) {
        float chunkWidth = CHUNK_SIZE * VOXEL_SIZE;

        int playerChunkX = static_cast<int>(std::floor(playerPos.x / chunkWidth));
        int playerChunkZ = static_cast<int>(std::floor(playerPos.z / chunkWidth));

        int maxChunkCreations = (workSize > 0) ? workSize : 1;
        int maxMeshes = (workSize > 0) ? workSize : 2;
        int chunksBuiltThisFrame = 0;

        for (int x = -RENDER_DISTANCE; x <= RENDER_DISTANCE; x++) {
            for (int z = -RENDER_DISTANCE; z <= RENDER_DISTANCE; z++) {

                ChunkCoord coord = { playerChunkX + x, playerChunkZ + z };
                if (m_Chunks.find(coord) == m_Chunks.end()) {
                    createChunk(coord.x, coord.z);

                    chunksBuiltThisFrame++;
                    if (chunksBuiltThisFrame >= maxChunkCreations) {
                        goto STOP_CHUNK_CREATION;
                    }
                }
            }
        }

        STOP_CHUNK_CREATION:
        int meshesBuilt = 0;
        const int MAX_MESHES_PER_FRAME = 2;

        while (!m_MeshQueue.empty() && meshesBuilt < MAX_MESHES_PER_FRAME) {
            ChunkCoord coord = m_MeshQueue.front();
            m_MeshQueue.pop_front();
            m_MeshQueueSet.erase(coord);

            Chunk* chunk = getChunkPtr(coord.x, coord.z);
            if (chunk) {
                Chunk* left = getChunkPtr(coord.x - 1, coord.z);
                Chunk* right = getChunkPtr(coord.x + 1, coord.z);
                Chunk* back = getChunkPtr(coord.x, coord.z - 1);
                Chunk* front = getChunkPtr(coord.x, coord.z + 1);

                chunk->generateMesh(left, right, front, back);
                meshesBuilt++;
            }
        }

        for (auto it = m_Chunks.begin(); it != m_Chunks.end(); ) {
            ChunkCoord coord = it->first;

            float distX = static_cast<float>(coord.x - playerChunkX);
            float distZ = static_cast<float>(coord.z - playerChunkZ);

            if(std::abs(distX) > RENDER_DISTANCE + 2 || std::abs(distZ) > RENDER_DISTANCE + 2) {
                it = m_Chunks.erase(it);
            }
            else {
                ++it;
            }
        }

        //std::cout << "Chunks: " << m_Chunks.size() << std::endl;
    }

    void ChunkManager::render(Shader& shader) {
        for (auto& [coord, chunk] : m_Chunks) {
            chunk->render(shader);
        }

    }
}