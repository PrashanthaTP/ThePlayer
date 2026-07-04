#include "coin.hpp"
#include <iostream>

Coin::Coin(int val, float radius)
    : _val(val),
      _radius(radius),
      _obj(sf::CircleShape(radius)) {
    _obj.setFillColor(sf::Color::Yellow);
    _obj.setOutlineColor(sf::Color::Blue);
}

void Coin::draw(sf::RenderWindow &window) {
    window.draw(_obj);
}

void Coin::setPosition(sf::Vector2f pos) {
    _obj.setPosition(pos);
}

sf::Vector2f Coin::getPosition() {
    return _obj.getPosition();
}

CoinManager::CoinManager(int n)
    : _n(n) {
    createCoins();
}

void CoinManager::createCoins() {
    std::cout << "Called CoinManager createcoins: " << _n << "\n";
    _coins.reserve(_n);
    float xOffset = 20;
    for (int i{0}; i < _n; i++) {
        _coins[i] = Coin(10, 15);
        _coins[i].setPosition({xOffset * i + 100.0f, 100.0f});
    }
}


void CoinManager::update(sf::RenderWindow &window, float time) {
}

void CoinManager::draw(sf::RenderWindow &window) {
    std::cout << "Called CoinManager: " << _n << "," << _coins.size() << "\n";
    bool once = true;
    for (auto &coin : _coins) {
        coin.draw(window);
        if (once) {
            std::cout << coin.getPosition().x << ", " << coin.getPosition().y
                      << "\n";
            once = false;
        }
    }
}
