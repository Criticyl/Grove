#include "ChunkManager.h"

namespace Grove {

    ChunkManager::ChunkManager() {

        m_Noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
        m_Noise.SetSeed(12);
        m_Noise.SetFrequency(0.003f);

        int generationRadius = 3;

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

        m_Chunks[coord] = std::make_unique<Chunk>(glm::vec3(worldX, 0.0f, worldZ), m_Noise);
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