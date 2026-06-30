#include "coin.hpp"

Coin::Coin(int val, float radius)
    : _val(val),
      _radius(radius),
      _obj(sf::CircleShape(radius)) {
}

void Coin::draw(sf::RenderWindow &window) {
    window.draw(_obj);
}

CoinManager::CoinManager() {
}

void CoinManager::update(sf::RenderWindow &window, float time) {
}

void CoinManager::draw(sf::RenderWindow &window) {
}
