#include "player.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>

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

void Player::handleGroundCollision() {
    // Should velocity.y be updated?
    fixPosGroundY();
}

void Player::handleCollision(const sf::RectangleShape &obj,
                             CollisionDirection direction) {

    // regardless of the direction if the player is above the object,
    // then player should be able to stand on it ?
    sf::Vector2f playerPos = _rect.getPosition();
    sf::Vector2f objPos = obj.getPosition();

    float playerTopY = playerPos.y;
    float playerBottomY = playerPos.y + _rect.getSize().y;
    float objTopY = objPos.y;

    bool standingNearLeftEdge = false;
    bool standingNearRightEdge = false;
    float overlap = 0;
    if ((playerPos.x >= objPos.x && playerPos.x < objPos.x + obj.getSize().x)) {
        standingNearLeftEdge = true;
        overlap = playerPos.x + _rect.getSize().x - objPos.x;
    }
    if ((playerPos.x + _rect.getSize().x > objPos.x &&
         playerPos.x + _rect.getSize().x <= objPos.x + obj.getSize().x)) {
        standingNearRightEdge = true;
        overlap = objPos.x + obj.getSize().x - playerPos.x;
    }
    bool isPlayerXInBetweenObjX = standingNearLeftEdge || standingNearRightEdge;

    if (playerTopY < objTopY && isPlayerXInBetweenObjX &&
        overlap >= _rect.getSize().x / 2) {
        // no need to change position!?
        _rect.setPosition(
            {_rect.getPosition().x, obj.getPosition().y - _rect.getSize().y});
        _physics.velocity.y =
            0; // when player is moved from the edge it wont fall immediately
    } else {

        _physics.velocity.x = 0;
        switch (direction) {
        case CollisionDirection::FROM_LEFT:
            // if (_physics.velocity.x > 0) {
            // the object is on the right
            {
                _rect.setPosition({obj.getPosition().x - _rect.getSize().x,
                                   _rect.getPosition().y});
                break;
            }
        case CollisionDirection::FROM_RIGHT: {
            // else if (_physics.velocity.x < 0)
            //  the object is on the left
            _rect.setPosition(
                {obj.getPosition().x + obj.getSize().x, _rect.getPosition().y});
            break;
        }

        default: {
            break;
        }
        }
    }
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
