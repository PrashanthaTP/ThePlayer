#include "player.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>

namespace ThePlayer {
Player::Player(PlayerPhysics physics)
    : _physics(physics) {
    _rect = sf::RectangleShape(_physics.size);
    _rect.setPosition(_physics.pos);
}

void Player::setGroundSupport(bool state) {
    _hasGroundSupport = state;
}

bool Player::isGrounded() {
    return _rect.getPosition().y + _rect.getSize().y == _physics.groundY ||
           _hasGroundSupport;
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
        setGroundSupport(false); // jumped so not on ground now
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

void Player::handleCollision(const Platform &obj,
                             CollisionDirection direction) {

    std::cout << "Collision Direction: " << getCollisionDirectionStr(direction)
              << "\n";
    // regardless of the direction if the player is above the object (not bottom though right?)
    // then player should be able to stand on it ?
    sf::Vector2f playerPos = _rect.getPosition();
    sf::Vector2f playerSize = _rect.getSize();

    sf::Vector2f objPos = obj.getPosition();
    sf::Vector2f objSize = obj.getSize();

    float playerTopY = playerPos.y;
    float playerBottomY = playerPos.y + _rect.getSize().y;
    float objTopY = objPos.y;

    // UNUSED ?
    bool standingNearLeftEdge = false;
    bool standingNearRightEdge = false;
    float overlap = 0;
    // TODO!! : Rethink about this logic
    if (playerPos.x < objPos.x && playerPos.x + playerSize.x > objPos.x) {
        standingNearLeftEdge = true;

        /*
         *
         *   pPos.x           pPos.x + pSize.x
         *    ------------------
         *    \                \
         *    \                \                  objPos.x + objSize.x
         *    \----------------\--------------------
         *          \                              \
         *
         */
        // overlap = playerPos.x + playerSize.x - objPos.x;
        overlap =
            objSize.x - (objPos.x + objSize.x - (playerPos.x + playerSize.x));
    } else if (playerPos.x + playerSize.x > objPos.x + objSize.x &&
               playerPos.x < objPos.x + objSize.x) {
        standingNearRightEdge = true;
        overlap = objSize.x - (playerPos.x - objPos.x);
    } else if (playerPos.x > objPos.x &&
               playerPos.x + playerSize.x < objPos.x + objSize.x) {
        // full overlap
        overlap = playerSize.x;
    }
    // UNUSED ?
    bool isPlayerXInBetweenObjX = standingNearLeftEdge || standingNearRightEdge;

    // if (playerTopY < objTopY && isPlayerXInBetweenObjX &&
    if (playerPos.y <= objPos.y &&
        overlap >= _rect.getSize().x / 2) { // place on top of platform
        // no need to change position!?
        std::cout << "Condition 1\n";
        _rect.setPosition(
            {_rect.getPosition().x, obj.getPosition().y - _rect.getSize().y});
        // when player is moved from the edge it wont fall immediately
        // otherwise this downward velocity keep buildling
        setGroundSupport(true);
        _physics.velocity.y = 0;
#if 0
        std::cout << "last move dist: " << obj.getLastMoveDist().x << " | "
                  << obj.getLastMoveDist().y << "\n";
        std::cout << "x velocity: " << _physics.velocity.x << "\n";
        std::cout << "Overlap: " << overlap << " / " << _rect.getSize().x
                  << "\n";
#endif

        // move along with platform if the player is still or moving in the
        // opposite direction of the platform
        sf::Vector2f distCoveredByPlatform = obj.getLastMoveDist();
        if ((distCoveredByPlatform.x < 0 &&
             _physics.velocity.x >= 0 /*&& _physics.velocity.y == 0*/) ||
            (distCoveredByPlatform.x > 0 && _physics.velocity.x <= 0)) {
            _rect.move(distCoveredByPlatform);
        }
    } else {
        setGroundSupport(false);
        switch (direction) {
        case CollisionDirection::FROM_LEFT:
            if (_physics.velocity.x >=
                0) // without this check the player just 'snaps' to wall once
                   // hit when walls are also moving left
            {
                // the object is on the right
                _rect.setPosition({obj.getPosition().x - _rect.getSize().x,
                                   _rect.getPosition().y});
            }
            _physics.velocity.x = 0;
            break;
        case CollisionDirection::FROM_RIGHT:
            if (_physics.velocity.x <= 0) {
                //  the object is on the left
                _rect.setPosition({obj.getPosition().x + obj.getSize().x,
                                   _rect.getPosition().y});
            }

            _physics.velocity.x = 0;
            break;
        case CollisionDirection::FROM_BOTTOM:
            if (_physics.velocity.y <= 0) {
                //  the object is below the 'object'
                _rect.setPosition({_rect.getPosition().x,
                                   obj.getPosition().y + obj.getSize().y});
            }
            // reverse direction
            //_physics.velocity.y *= -1.0f; // increased gravity -> faster downward movement

            _physics.velocity.y = 0;
            break;
        case CollisionDirection::FROM_TOP:
            _physics.velocity.y = 0;
            break;
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
