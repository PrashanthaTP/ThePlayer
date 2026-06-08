#pragma once
#include <SFML/Graphics.hpp>

class Platform {
  private:
    sf::Vector2f _pos;
    sf::Vector2f _size;
    sf::Vector2f _acc;
    sf::RectangleShape _rect;
    void moveToOrigPos();

  public:
    Platform(const sf::Vector2f pos, const sf::Vector2f size, sf::Vector2f acc);
    void moveLeft(float time);
    void moveRight(float time);
    void respawn();
    void draw(sf::RenderWindow &window);
    sf::Vector2f getSize();
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f pos);
  #if DEBUG
    sf::RectangleShape& getRect();
  #endif
};
