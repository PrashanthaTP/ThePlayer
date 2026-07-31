#pragma once
#include <sfml/Graphics.hpp>
#include <vector>

class Coin {
  private:
    int _val;
    float _radius;
    sf::Vector2f _prevPos;
    bool _isHidden;
    sf::CircleShape _obj;

  public:
    Coin(int val, float radius);
    void draw(sf::RenderWindow &window);
    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;
    const sf::FloatRect getGlobalBounds() const;
    //TODO! setVisibility api instead of separate ones
    void hide();
    void unhide();
    bool isHidden() const;
    void setHidden(bool shouldHide);
    void notifyCollision();
    int getValue();
};

class CoinManager {
  private:
    int _n;
    int _n_hidden;
    std::vector<Coin> _coins;
    void createCoins();

  public:
    CoinManager(int n);
    void update(sf::RenderWindow &window, float time);
    void draw(sf::RenderWindow &window);
    std::vector<Coin> &getCoins();
};
