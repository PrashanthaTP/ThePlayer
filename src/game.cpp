#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

#include "constants.hpp"
#include "game.hpp"
#include "platforms.hpp"
#include "rng.hpp"

namespace ThePlayer {

static std::vector<Platform> platforms;

class PlatformManager {
  private:
    size_t _n;
    std::vector<Platform> _platforms;

    void createPlatforms() {
        int offset = 0;
        for (size_t i{0}; i < _n; i++) {

            sf::Vector2f size{1.0f * Rng::getRandom(60, 80),
                              1.0f * Rng::getRandom(30, 40)};

            sf::Vector2f pos{GSettings.width + 1.0f + offset,
                             GSettings.groundY -
                                 1.0f * Rng::getRandom(100, 200) - size.y};

            sf::Vector2f acc{900.0f, 0.0f};

            _platforms.emplace_back(pos, size, acc);
            offset = (i + 1) * Rng::getRandom(300, 400);
        }
    }

  public:
    PlatformManager(size_t n)
        : _n(n) {
        createPlatforms();
    }
    void draw(sf::RenderWindow &window) {
        for (auto &p : _platforms) {
            p.draw(window);
        }
    }

    const std::vector<Platform> getPlatforms() const {
        return _platforms;
    }
    void update(sf::RenderWindow &window, float time) {
        for (auto &p : _platforms) {
            p.moveLeft(time);
#if DEBUG
            std::cout << "Platform:\n";
            std::cout << p.getPosition().x << "\n";
            std::cout << p.getPosition().y << "\n";
            std::cout << p.getSize().x << "\n";
            std::cout << p.getSize().y << "\n";
#endif
            if (p.getPosition().x + p.getSize().x < 0) {
                p.setPosition({(float)window.getSize().x,
                               GSettings.groundY -
                                   1.0f * Rng::getRandom(100, 300) -
                                   p.getSize().y});
            }
        }
    }
};

static PlatformManager platformManager(3);

sf::RectangleShape addBoundingBox(const sf::RenderWindow &window,
                                  const sf::Sprite sprite) {
    sf::FloatRect rect = sprite.getGlobalBounds();
    sf::RectangleShape resRect(sf::Vector2f(rect.size.x, rect.size.y));
    resRect.setOutlineColor(sf::Color::Red);
    resRect.setFillColor(sf::Color::Transparent);
    resRect.setOutlineThickness(2.0f);
    resRect.setPosition({rect.position.x, rect.position.y});
    return resRect;
}

Engine::Engine() {
}

Engine::~Engine() {
}

[[nodiscard]] CollisionDirection
#if DEBUG
Engine::findCollisionDirection(Player &player,
                               const sf::RectangleShape &platform) {
#else
Engine::findCollisionDirection(const Player &player, const Platform &platform) {
#endif
    const sf::Vector2f size = player.getSize();
    const sf::Vector2f pos = player.getPosition();
    float leftX = pos.x;
    float rightX = pos.x + size.x;
    float topY = pos.y;
    float bottomY = pos.y + size.y;

    float pLeftX = platform.getPosition().x;
    float pRightX = pLeftX + platform.getSize().x;
    float pTopY = platform.getPosition().y;
    float pBottomY = pTopY + platform.getSize().y;

    float overlapFromLeftX = rightX - pLeftX;
    float overlapFromRightX = pRightX - leftX;
    float overlapFromTopY = bottomY - pTopY;
    float overlapFromBottomY = pBottomY - topY;

    float res = GSettings.width + 10.0f;
    CollisionDirection resDir = CollisionDirection::FROM_LEFT;
    if (overlapFromLeftX && overlapFromLeftX < overlapFromRightX) {
        if (overlapFromLeftX < res) {
            resDir = CollisionDirection::FROM_LEFT;
            res = overlapFromLeftX;
        }
    }
    if (overlapFromRightX && overlapFromRightX < overlapFromLeftX) {
        if (overlapFromRightX < res) {
            resDir = CollisionDirection::FROM_RIGHT;
            res = overlapFromRightX;
        }
    }
    if (overlapFromTopY && overlapFromTopY < overlapFromBottomY) {
        if (overlapFromTopY < res) {
            resDir = CollisionDirection::FROM_TOP;
            res = overlapFromTopY;
        }
    }
    if (overlapFromBottomY && overlapFromBottomY < overlapFromTopY) {
        if (overlapFromBottomY < res) {
            resDir = CollisionDirection::FROM_BOTTOM;
            res = overlapFromBottomY;
        }
    }
    return resDir;
}

#if DEBUG
void Engine::handleCollisions(
    Player &player,
    std::vector<std::shared_ptr<const sf::RectangleShape>> &platforms) {
    const sf::Vector2f size = player.getSize();
    const sf::Vector2f pos = player.getPosition();
    float leftX = pos.x;
    float rightX = pos.x + size.x;
    float topY = pos.y;
    float bottomY = pos.y + size.y;
    for (auto &platform_p : platforms) {
        if (!platform_p) {
            continue;
        }

        auto platform = *platform_p;
        float pLeftX = platform.getPosition().x;
        float pRightX = pLeftX + platform.getSize().x;
        float pTopY = platform.getPosition().y;
        float pBottomY = pTopY + platform.getSize().y;
#ifdef DEBUG
        std::cout << leftX << " | " << rightX << " | " << topY << " | "
                  << bottomY << "\n";
        std::cout << pLeftX << " | " << pRightX << " | " << pTopY << " | "
                  << pBottomY << "\n";
#endif
        bool xColliding = rightX >= pLeftX && leftX <= pRightX;
        bool yColliding = topY <= pBottomY && bottomY >= pTopY;
        if (xColliding && yColliding) {
            CollisionDirection direction =
                findCollisionDirection(player, platform);
            player.handleCollision(platform, direction);
        }
    }
    if (bottomY >= GSettings.groundY) {
        player.handleGroundCollision();
    }
}
#endif

void Engine::handleCollisions(Player &player) {
    const sf::Vector2f size = player.getSize();
    const sf::Vector2f pos = player.getPosition();
    float leftX = pos.x;
    float rightX = pos.x + size.x;
    float topY = pos.y;
    float bottomY = pos.y + size.y;
    for (auto &platform : platformManager.getPlatforms()) {

        float pLeftX = platform.getPosition().x;
        float pRightX = pLeftX + platform.getSize().x;
        float pTopY = platform.getPosition().y;
        float pBottomY = pTopY + platform.getSize().y;
#ifdef DEBUG
        std::cout << leftX << " | " << rightX << " | " << topY << " | "
                  << bottomY << "\n";
        std::cout << pLeftX << " | " << pRightX << " | " << pTopY << " | "
                  << pBottomY << "\n";
#endif
        bool xColliding = rightX >= pLeftX && leftX <= pRightX;
        bool yColliding = topY <= pBottomY && bottomY >= pTopY;
        if (xColliding && yColliding) {
            CollisionDirection direction =
                findCollisionDirection(player, platform);
            player.handleCollision(platform, direction);
        }
    }
    if (bottomY >= GSettings.groundY) {
        player.handleGroundCollision();
    }
}

[[nodiscard]] InputState Engine::handleKeyPress() {
    InputState inputState;
    inputState.left = false;
    inputState.right = false;
    inputState.up = false;
    inputState.down = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        inputState.left = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        inputState.right = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        inputState.up = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        inputState.up = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        inputState.left = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        inputState.right = true;
    }
    return inputState;
}

void Engine::run() {
    sf::RenderWindow window(sf::VideoMode({GSettings.width, GSettings.height}),
                            GSettings.name,
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(30);

    sf::RectangleShape groundLine({GSettings.width * 1.0f, 2});
    groundLine.setFillColor(sf::Color::Red);
    groundLine.setPosition({0, GSettings.groundY});

    sf::Texture bgTexture;

    if (!bgTexture.loadFromFile("assets/bg.png")) {
        std::cerr << "Error loading background image;\n";
        exit(EXIT_FAILURE);
    }

    sf::Sprite bgSprite(bgTexture);
    bgSprite.setPosition({0.0f, 0.0f});

    sf::Texture treeTexture;
    if (!treeTexture.loadFromFile("assets/experiment/02_tree.png")) {
        std::cerr << "Error loading tree image;\n";
        exit(EXIT_FAILURE);
    }
    sf::Sprite treeSprite1(treeTexture);
    treeSprite1.setPosition(
        {100.0f, GSettings.groundY - treeTexture.getSize().y});
    sf::Sprite treeSprite2(treeTexture);
    treeSprite2.setPosition(
        {400.0f, GSettings.groundY - treeTexture.getSize().y});

    sf::RectangleShape boundingBox1 = addBoundingBox(window, treeSprite1);
    sf::RectangleShape boundingBox2 = addBoundingBox(window, treeSprite2);

    /*
std::vector<std::shared_ptr<const sf::RectangleShape>> platformsPtrs;

platformsPtrs.emplace_back(
    std::make_shared<const sf::RectangleShape>(boundingBox1));
platformsPtrs.emplace_back(
    std::make_shared<const sf::RectangleShape>(boundingBox2));
    */

    sf::Clock clock;

    PlayerPhysics physics = {
        .velocity = {0.0f, 0.0f},
        .pos = {(float)GSettings.width / 2 - 15.0f, GSettings.groundY - 60.0f},
        .size = {30.0f, 60.0f},
        .jumpforce = 450.0f,
        .friction = 0.9f,
        .gravity = 800.0f,
        .acceleration = 900.0f,
        .velocityLimits = {-400.0f, 400.0f},
        .groundY = GSettings.groundY};
    Player player(physics);

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear();
        float time = clock.restart().asSeconds();

        platformManager.update(window, time);
        InputState inputState = handleKeyPress();
        player.update(inputState);
        handleCollisions(player);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            window.close();
        }

        window.draw(bgSprite);

        // window.draw(treeSprite1);
        // window.draw(treeSprite2);
        // window.draw(boundingBox1);
        // window.draw(boundingBox2);
        window.draw(player.getDrawObj());
        window.draw(groundLine);

        platformManager.draw(window);

        // std::cout  << p.getSize().x << " | " << p.getSize().y << "\n";
        // std::cout  << p.getPosition().x << " | " << p.getPosition().y <<
        // "\n"; window.draw(p.getRect());
        window.display();
    }
}

} // namespace ThePlayer
