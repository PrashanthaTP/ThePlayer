#pragma once
#include <cstdint>
enum class CollisionObject : uint8_t { COLLISION_WALL, COLLISION_GROUND };

enum class CollisionDirection : uint8_t {
    FROM_LEFT,
    FROM_RIGHT,
    FROM_TOP,
    FROM_BOTTOM
};
