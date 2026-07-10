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
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
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
    const std::vector<Coin> &getCoins();
};
