#include "player.hpp"

#include <algorithm>
#include <iostream>
#include <cstring>

namespace ThePlayer {
Player::Player(PlayerPhysics physics) : _physics(physics) {
    _rect = sf::RectangleShape(_physics.size);
    _rect.setPosition(_physics.pos);
}

bool Player::isGrounded() {
    return _rect.getPosition().y + _rect.getSize().y == _physics.groundY;
}

bool Player::isBelowGround() {
    return _rect.getPosition().y + _rect.getSize().y > _physics.groundY;
}

void Player::fixPosGroundY() {
    _rect.setPosition(
        {_rect.getPosition().x, _physics.groundY - _rect.getSize().y});
}

void Player::setInputState(InputState &inputState) {
    memcpy(&_inputState, &inputState, sizeof(InputState));
}

void Player::updateY(float time) {
    // ------------- Apply Physics -------------
    if (!isGrounded()) {
        _physics.velocity.y += _physics.gravity * time;
    }
    if (isGrounded() && _inputState.up) {
        _physics.velocity.y = -_physics.jumpforce;
    }
    float offsetY = _physics.velocity.y * time;

    // ------------- Move -------------
    _rect.move({0.0f, offsetY});

    // ------------- Corrections -------------
    if (isBelowGround()) {
        fixPosGroundY();
    }
    if (_physics.velocity.y > 0 && isGrounded()) {
        _physics.velocity.y = 0;
    }
}

void Player::updateX(float time) {
    bool isMoving = false;

    if (_inputState.left) {
        _rect.setFillColor(sf::Color::Yellow);
        // player.velocity.x = std::min(player.velocity.x, 0.0f);//helps when
        // suddenly switching from different direction
        if (_physics.velocity.x > 0) {
            _physics.velocity.x *= 0.5f; // damp since changing direction now
        }
        _physics.velocity.x += -1.0f * _physics.acceleration * time;
        isMoving = true;
    }
    if (_inputState.right) {
        _rect.setFillColor(sf::Color::Magenta);
        // player.velocity.x = std::max(player.velocity.x, 0.0f);//helps when
        // suddenly switching from different direction
        if (_physics.velocity.x < 0) {
            _physics.velocity.x *= 0.5f; // damp since changing direction now
        }
        _physics.velocity.x += 1.0f * _physics.acceleration * time;
        isMoving = true;
    }

    if (!isMoving) { // apply when (A/D) key not pressed so that actually meant
                     // speed not affected
        // Move velocity towards zero
        _physics.velocity.x *= _physics.friction;
    }
    if (std::abs(_physics.velocity.x) < 0.01f) {
        _physics.velocity.x = 0;
    }

    _physics.velocity.x = std::clamp(_physics.velocity.x,
                                     _physics.velocityLimits.minVal,
                                     _physics.velocityLimits.maxVal);

    _rect.move({_physics.velocity.x * time, 0});
}

void Player::update(InputState &inputState) {
    setInputState(inputState);
    float time = _clock.restart().asSeconds();
    updateY(time);
    updateX(time);
}

void Player::handleCollision(const sf::RectangleShape &obj) {
    if (_physics.velocity.x > 0) {
        // the object is on the right
        _rect.setPosition(
            {obj.getPosition().x - _rect.getSize().x, _rect.getPosition().y});
    } else if (_physics.velocity.x < 0) {
        // the object is on the left
        _rect.setPosition(
            {obj.getPosition().x + obj.getSize().x, _rect.getPosition().y});
    }
    _physics.velocity.x = 0;
}

[[nodiscard]] const sf::RectangleShape &Player::getDrawObj() const {
    return _rect;
}

[[nodiscard]] const sf::Vector2f Player::getSize() const {
    return _rect.getSize();
};

[[nodiscard]] const sf::Vector2f Player::getPosition() const {
    return _rect.getPosition();
};

}; // namespace ThePlayer
