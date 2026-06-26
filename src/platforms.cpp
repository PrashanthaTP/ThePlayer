#include "platforms.hpp"
#include "constants.hpp"
#include "rng.hpp"
#include <iostream>

Platform::Platform(const sf::Vector2f pos,
                   const sf::Vector2f size,
                   const sf::Vector2f acc)
    : _pos(pos),
      _size(size),
      _acc(acc),
      _lastMoveDist({0.0f, 0.0f}),
      _rect(size) {
    _rect.setOutlineColor(sf::Color::Red);
    _rect.setFillColor(sf::Color::Blue);
    _rect.setOutlineThickness(2.0f);
    moveToOrigPos();
};

void Platform::moveToOrigPos() {
    _rect.setPosition(_pos);
    std::cout << "Position: " << _rect.getPosition().x << "\n";

}

void Platform::moveLeft(float time) {
    sf::Vector2f dist{-1 * _acc.x * time * time, 0.0f};
    _rect.move(dist);
    _lastMoveDist = dist;
}

void Platform::moveRight(float time) {
    sf::Vector2f dist{0.0f, _acc.x * time * time};
    _rect.move(dist);
    _lastMoveDist = dist;
}

void Platform::respawn() {
    moveToOrigPos();
}

void Platform::draw(sf::RenderWindow &window) {
    window.draw(_rect);
}

sf::Vector2f Platform::getSize() const {
    return _rect.getSize();
}

sf::Vector2f Platform::getOrigPosition() const {
    return _pos;
}

sf::Vector2f Platform::getPosition() const {
    return _rect.getPosition();
}

sf::Vector2f Platform::getLastMoveDist() const {
    return _lastMoveDist;
}
void Platform::setPosition(sf::Vector2f pos) {
    std::cout <<"SetPosition called: " << pos.x << "," << pos.y << "\n";
    _rect.setPosition(pos);
}

#if DEBUG
sf::RectangleShape &Platform::getRect() {
    return _rect;
}
#endif

/*---------------------------------------------------------------
 *
 *         Platform Manager
 *
 *----------------------------------------------------------------
 */
PlatformManager::PlatformManager(size_t n)
    : _n(n) {
    createPlatforms();
}

void PlatformManager::createPlatforms() {
    int offset = 0;
    for (size_t i{0}; i < _n; i++) {

        //offset = i * Rng::getRandom(300, 400);
        offset += 500;

        sf::Vector2f size{80, 40};

        sf::Vector2f pos{GSettings.width + 1.0f + offset,
                         GSettings.groundY - 1.0f * Rng::getRandom(30, 100) -
                             size.y};

        //sf::Vector2f acc{1.0f * Rng::getRandom(900, 1000), 0.0f};
        sf::Vector2f acc{1200.0f, 0.0f};

        _platforms.emplace_back(pos, size, acc);
    }
}
void PlatformManager::draw(sf::RenderWindow &window) {
    int c = 0;
    for (auto &p : _platforms) {
        p.draw(window);
    }
}

const std::vector<Platform> PlatformManager::getPlatforms() const {
    return _platforms;
}

void PlatformManager::update(sf::RenderWindow &window, float time) {
    for (auto &p : _platforms) {
        p.moveLeft(time);
#if DEBUG
        std::cout << "Platform:\n";
        std::cout << p.getPosition().x << "\n";
        std::cout << p.getPosition().y << "\n";
        std::cout << p.getSize().x << "\n";
        std::cout << p.getSize().y << "\n";
#endif
        if ((p.getPosition().x + p.getSize().x) < 0.0f) {
            sf::Vector2f origPos = p.getOrigPosition();
            //p.setPosition({(float)window.getSize().x + 1.0f * Rng::getRandom(0,200),
            p.setPosition({origPos.x,
                           GSettings.groundY - 1.0f * Rng::getRandom(0,80) -
                               p.getSize().y});
        }
    }
}
