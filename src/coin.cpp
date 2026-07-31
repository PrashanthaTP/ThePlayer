#include "coin.hpp"
#include "rng.hpp"
#include <iostream>
#include <vector>

Coin::Coin(int val, float radius)
    : _val(val),
      _radius(radius),
      _isHidden(false),
      _obj(sf::CircleShape(radius)) {
    _obj.setFillColor(sf::Color::Yellow);
    _obj.setOutlineColor(sf::Color::Blue);
    _obj.setOutlineThickness(5.0f);
}

void Coin::draw(sf::RenderWindow &window) {
    if (_isHidden) {
        return;
    }
    window.draw(_obj);
}

void Coin::setPosition(sf::Vector2f pos) {
    _prevPos = _obj.getPosition();
    _obj.setPosition(pos);
}

sf::Vector2f Coin::getPosition() const {
    return _obj.getPosition();
}

const sf::FloatRect Coin::getGlobalBounds() const {
    return _obj.getGlobalBounds();
}

// TODO: collision reaction should be controlled from CoinManager
void Coin::hide() {
    _isHidden = true;
    // setPosition({-10, -10});
}

void Coin::unhide() {
    _isHidden = false;
}

bool Coin::isHidden() const {
    return _isHidden;
}

void Coin::setHidden(bool shouldHide) {
    if (shouldHide) {
        hide();
    } else {
        unhide();
    }
}

void Coin::notifyCollision() {
    hide();
}

int Coin::getValue() {
    return _val;
}

CoinManager::CoinManager(int n)
    : _n(n),
      _n_hidden(0) {
    _coins.reserve(_n);
    createCoins();
}

void CoinManager::createCoins() {

    float xOffset = 100;
    for (int i{0}; i < _n; i++) {
        _coins.emplace_back(10, 15);
        _coins.back().setPosition(
            {xOffset * i + 100.0f, 1.0f * Rng::getRandom(120, 180)});
    }
}

void CoinManager::update(sf::RenderWindow &window, float time) {
}

void CoinManager::draw(sf::RenderWindow &window) {
    static int cnt = 0;
    cnt++;
    if (cnt % 5 == 0) {
        cnt = 0;
        int countHidden = 0;
        for (auto &coin : _coins) {
            if (coin.isHidden()) {
                countHidden++;
            }
        }
        if(countHidden==_coins.size()){
            _coins.clear();
            createCoins();
        }

    }

    for (auto &coin : _coins) {
        coin.draw(window);
    }
}

std::vector<Coin> &CoinManager::getCoins() {
    return _coins;
}
