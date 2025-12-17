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

    const int CHUNK_SIZE = 32;
    const int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    class Chunk {
    public:
        Chunk(glm::vec3 position, FastNoiseLite& noise);
        ~Chunk();

        void generateMesh();
        void render(Shader& shader);

        Voxel getVoxel(int x, int y, int z) const;
        void setVoxel(int x, int y, int z, std::uint8_t id);

    private:

        bool isAir(int x, int y, int z) const;
        void addFace(const std::vector<float>& faceVertices, int x, int y, int z, int faceID);

    private:

        Voxel m_Voxels[CHUNK_VOLUME];
        glm::vec3 m_Position;
        std::vector<float> m_Colours;

        std::vector<float> m_Vertices;
        std::vector<std::uint32_t> m_Indices;

        std::unique_ptr<VAO> m_VAO;
        std::unique_ptr<VBO> m_VBO;
        std::unique_ptr<VBO> m_ColVBO;
        std::unique_ptr<EBO> m_EBO;

        bool m_MeshDirty = false;
    };
}
