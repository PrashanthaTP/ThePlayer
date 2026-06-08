#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>
#include <vector>

#include "collisions.hpp"
#include "inputs.hpp"
#include "player.hpp"
#include "platforms.hpp"


namespace ThePlayer {

class Engine {
  private:
  #if DEBGUG
    void handleCollisions(
        Player &player,
        std::vector<std::shared_ptr<const sf::RectangleShape>> &platforms);
  #else
    void handleCollisions( Player &player);
  #endif
    [[nodiscard]] InputState handleKeyPress();
    [[nodiscard]] CollisionDirection
  #if DEBGUG
    findCollisionDirection(Player &player, const sf::RectangleShape &platform);
  #else
    findCollisionDirection(const Player &player, const Platform &platform);

  #endif


  public:
    Engine();
    void run();
    ~Engine();
};

} // namespace ThePlayer
