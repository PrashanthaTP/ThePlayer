#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>
#include <vector>

#include "player.hpp"

namespace ThePlayer {

class Engine {
  private:
  public:
    Engine();
    void run();
    void handleCollisions(
        Player &player,
        std::vector<std::unique_ptr<const sf::RectangleShape>> &platforms);
    ~Engine();
};

} // namespace ThePlayer
