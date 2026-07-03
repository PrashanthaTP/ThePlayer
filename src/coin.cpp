#include "coin.hpp"

Coin::Coin(int val, float radius)
    : _val(val),
      _radius(radius),
      _obj(sf::CircleShape(radius)) {
}

void Coin::draw(sf::RenderWindow &window) {
    window.draw(_obj);
}

void CoinManager::createCoins() {
    _coins.reserve(_n);
    for (int i{0}; i < _n; i++) {
        _coins[i] = Coin(10, 15);
    }
}

CoinManager::CoinManager(int n)
    : _n(n) {
    createCoins();
}

void CoinManager::update(sf::RenderWindow &window, float time) {
}

void CoinManager::draw(sf::RenderWindow &window) {
}
