#include "ChunkManager.h"

namespace Grove {

    ChunkManager::ChunkManager() {

        int generationRadius = 3;

        for (int x = -generationRadius; x < generationRadius; x++) {
            for (int z = -generationRadius; z < generationRadius; z++) {
                createChunk(x, z);
            }
        }
    }

    ChunkManager::~ChunkManager() {
        m_Chunks.clear();
    }

    void ChunkManager::createChunk(int x, int z) {
        ChunkCoord coord = { x, z };

        float worldX = x * CHUNK_SIZE;
        float worldZ = z * CHUNK_SIZE;

        m_Chunks[coord] = std::make_unique<Chunk>(glm::vec3(worldX, 0.0f, worldZ));
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