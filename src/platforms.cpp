#include "platforms.hpp"

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

sf::Vector2f Platform::getPosition() const {
    return _rect.getPosition();
}

sf::Vector2f Platform::getLastMoveDist() const {
    return _lastMoveDist;
}

void Platform::setPosition(sf::Vector2f pos) {
    _rect.setPosition(pos);
}

#if DEBUG
sf::RectangleShape &Platform::getRect() {
    return _rect;
}
#endif
