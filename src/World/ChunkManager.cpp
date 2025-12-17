#include "ChunkManager.h"

namespace Grove {

    ChunkManager::ChunkManager() {

        m_TerrainNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_TerrainNoise.SetSeed(12);
        m_TerrainNoise.SetFrequency(0.003f);

        m_GrassNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        m_GrassNoise.SetSeed(33);
        m_GrassNoise.SetFrequency(0.05f);

        m_StoneNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
        m_StoneNoise.SetSeed(1);
        m_StoneNoise.SetFrequency(0.1f);
        m_StoneNoise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);

        int generationRadius = 15;

        for (int x = -generationRadius; x < generationRadius; x++) {
            for (int z = -generationRadius; z < generationRadius; z++) {
                createChunk(x, z);
            }
        }

        std::cout << "Chunks: " << m_Chunks.size() << std::endl;
    }

    ChunkManager::~ChunkManager() {
        m_Chunks.clear();
    }

    void ChunkManager::createChunk(int x, int z) {
        ChunkCoord coord = { x, z };

        float worldX = x * CHUNK_SIZE;
        float worldZ = z * CHUNK_SIZE;

        m_Chunks[coord] = std::make_unique<Chunk>(glm::vec3(worldX, 0.0f, worldZ), m_TerrainNoise, m_GrassNoise, m_StoneNoise);
        m_Chunks[coord]->generateMesh();
    }

    void ChunkManager::update() {

    }

    void ChunkManager::render(Shader& shader) {
        for (auto& [coord, chunk] : m_Chunks) {
            chunk->render(shader);
        }

    }
}