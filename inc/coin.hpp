#pragma once
#include <sfml/Graphics.hpp>
#include <vector>

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
  private:
    int _n;
    std::vector<Coin> _coins;
    void createCoins();

  public:
    CoinManager(int n);
    void update(sf::RenderWindow &window, float time);
    void draw(sf::RenderWindow &window);
};
