#pragma once
#include <SFML/Graphics.hpp>

class Platform {
  private:
    const sf::Vector2f _pos;
    const sf::Vector2f _size;
    sf::Vector2f _acc;
    sf::Vector2f _lastMoveDist;
    sf::RectangleShape _rect;
    void moveToOrigPos();

  public:
    Platform(const sf::Vector2f pos,
             const sf::Vector2f size,
             const sf::Vector2f acc);
    void moveLeft(float time);
    void moveRight(float time);
    void respawn();
    void draw(sf::RenderWindow &window);
    sf::Vector2f getSize() const;
    sf::Vector2f getOrigPosition() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getLastMoveDist() const;
    void setPosition(sf::Vector2f pos);
#if DEBUG
    sf::RectangleShape &getRect();
#endif
};

class PlatformManager {
  private:
    size_t _n;
    std::vector<Platform> _platforms;

    void createPlatforms();

  public:
    PlatformManager(size_t n);
    void draw(sf::RenderWindow &window);

    const std::vector<Platform> getPlatforms() const;
    void update(sf::RenderWindow &window, float time);
};

