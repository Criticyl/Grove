#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Grove {

    struct Voxel {
        std::uint8_t ID;

        bool isAir() const { return ID == 0; }

        glm::vec3 getBaseColour() const {
            switch (ID) {
                case 1: return glm::vec3(0.23f, 0.9f, 0.3f);
                case 2: return glm::vec3(0.4f, 0.4f, 0.4f);
                default: return glm::vec3(0.0f, 0.0f, 0.0f);
            }
        }

        glm::vec3 getNoiseColour(int x, int y, int z) const {
            glm::vec3 baseColour = getBaseColour();

            float noise = (float)((x * 374761363 + y * 764382931 + z * 322832947) % 100) / 100.0f;
            float variance = (noise - 0.5f) * 0.15f;

            return glm::clamp(baseColour + glm::vec3(variance), 0.0f, 1.0f);
        }
    };
}
