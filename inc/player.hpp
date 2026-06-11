#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "collisions.hpp"
#include "inputs.hpp"
#include "platforms.hpp"

namespace ThePlayer {
struct WorldUtils;

struct Limits {
    float minVal;
    float maxVal;
};

struct PlayerPhysics {
    sf::Vector2f velocity;
    sf::Vector2f pos;
    sf::Vector2f size;
    float jumpforce;
    float friction;
    float gravity;
    float acceleration;
    Limits velocityLimits;
    float groundY;
};

class Player {
  private:
    sf::RectangleShape _rect;
    PlayerPhysics _physics;
    sf::Clock _clock;
    InputState _inputState;
    bool _hasGroundSupport{false};
    // std::unique_ptr<WorldUtils> _utils_p;
    void updateY(float time);
    void updateX(float time);
    bool isGrounded();
    bool isBelowGround();
    void fixPosGroundY();
    void setInputState(InputState &inputState);
    void setGroundSupport(bool state);

  public:
    // Player(PlayerPhysics physics, std::unique_ptr<WorldUtils> _utils_p);
    Player(PlayerPhysics physics);
    void update(InputState &inputState);
    void handleGroundCollision(void);
    void handleCollision(const Platform &obj, CollisionDirection direction);
    [[nodiscard]] const sf::RectangleShape &getDrawObj() const;
    [[nodiscard]] const sf::Vector2f getSize() const;
    [[nodiscard]] const sf::Vector2f getPosition() const;
};

} // namespace ThePlayer
