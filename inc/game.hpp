#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>
#include <vector>

#include "collisions.hpp"
#include "inputs.hpp"
#include "player.hpp"

namespace ThePlayer {

class Engine {
  private:
    void handleCollisions(
        Player &player,
        std::vector<std::unique_ptr<const sf::RectangleShape>> &platforms);
    [[nodiscard]] InputState handleKeyPress();
    [[nodiscard]] CollisionDirection
    findCollisionDirection(Player &player, const sf::RectangleShape &platform);

  public:
    Engine();
    void run();
    ~Engine();
};

} // namespace ThePlayer
