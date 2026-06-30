#include "collisions.hpp"

std::string getCollisionDirectionStr(CollisionDirection dir) {
    std::string res;
    switch (dir) {
    case CollisionDirection::FROM_LEFT:
        res = "FROM_LEFT";
        break;
    case CollisionDirection::FROM_RIGHT:
        res = "FROM_RIGHT";
        break;
    case CollisionDirection::FROM_TOP:
        res = "FROM_TOP";
        break;
    case CollisionDirection::FROM_BOTTOM:
        res = "FROM_BOTTOM";
        break;
    default:
        res = "<unknown>";
    }
    return res;
}
