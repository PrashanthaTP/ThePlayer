#pragma once
#include <sfml/Graphics.hpp>

class Coin {
  private:
    int _val;
    float _radius;
    sf::CircleShape _obj;

  public:
    Coin(int val, float radius);
    void draw(sf::RenderWindow &window);
};

class CoinManager {
  public:
    CoinManager();
    void update(sf::RenderWindow &window, float time);
    void draw(sf::RenderWindow &window);
};
