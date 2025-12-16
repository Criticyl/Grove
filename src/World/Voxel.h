#pragma once
#include <cstdint>

namespace Grove {

    struct Voxel {
        std::uint8_t ID;

        bool isAir() const { return ID == 0; }
    };
}
