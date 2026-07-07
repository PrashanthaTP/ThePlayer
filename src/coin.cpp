#include "coin.hpp"
#include <iostream>
#include "rng.hpp"

Coin::Coin(int val, float radius)
    : _val(val),
      _radius(radius),
      _obj(sf::CircleShape(radius)) {
    _obj.setFillColor(sf::Color::Yellow);
    _obj.setOutlineColor(sf::Color::Blue);
    _obj.setOutlineThickness(5.0f);
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
    _coins.reserve(_n);

    float xOffset = 100;
    for (int i{0}; i < _n; i++) {
        _coins.emplace_back(10, 15);
        _coins.back().setPosition({xOffset * i + 100.0f, 1.0f * Rng::getRandom(120,180)});
    }
}


void CoinManager::update(sf::RenderWindow &window, float time) {
}

void CoinManager::draw(sf::RenderWindow &window) {
    bool once = true;
    for (auto &coin : _coins) {
        coin.draw(window);
        if (once) {
            once = false;
        }
    }
}
