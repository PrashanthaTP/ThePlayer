#include "platforms.hpp"

Platform::Platform(const sf::Vector2f pos,
                   const sf::Vector2f size,
                   const sf::Vector2f acc)
    : _size(size),
      _pos(pos),
      _acc(acc),
      _rect(sf::RectangleShape(size)) {
    _rect.setOutlineColor(sf::Color::Red);
    _rect.setFillColor(sf::Color::Blue);
    _rect.setOutlineThickness(2.0f);
    moveToOrigPos();
};

void Platform::moveToOrigPos() {
    _rect.setPosition(_pos);
}

void Platform::moveLeft(float time) {
    _rect.move({-1 * _acc.x * time * time, 0.0f});
}

void Platform::moveRight(float time) {
    _rect.move({0.0f, _acc.x * time * time});
}

void Platform::respawn() {
    moveToOrigPos();
}

void Platform::draw(sf::RenderWindow &window) {
    window.draw(_rect);
}

sf::Vector2f Platform::getPosition(){
      return _rect.getPosition(); 
}

