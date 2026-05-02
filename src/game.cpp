#include <iostream>
#include <memory>
#include <vector>

#include "game.hpp"

namespace ThePlayer {

struct GameSettings {
    std::string name;
    unsigned width;
    unsigned height;

    float groundY;
};

struct Colors {
    sf::Color up = sf::Color::Cyan;
    sf::Color down = sf::Color::Green;
    sf::Color left = sf::Color::Red;
    sf::Color right = sf::Color::Yellow;
    sf::Color idle = sf::Color::White;
};

const GameSettings GSettings = {.name = "ThePlayer",
                                .width = 640,
                                .height = 640,
                                .groundY = 320.0f + 40 + 20};

Colors GColors;

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

Engine::Engine() {}

Engine::~Engine() {}

void Engine::handleCollisions(
    Player &player,
    std::vector<std::unique_ptr<const sf::RectangleShape>> &platforms) {
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
        std::cout << leftX << " | " << rightX << " | " << topY << " | "
                  << bottomY << "\n";
        std::cout << pLeftX << " | " << pRightX << " | " << pTopY << " | "
                  << pBottomY << "\n";
        bool xColliding = rightX >= pLeftX && leftX <= pRightX;
        bool yColliding = topY >= pBottomY && bottomY <= pTopY;
        if (xColliding && yColliding) {
            std::cout << "Collided\n";
            player.handleCollision(platform);
        }
    }
}

void Engine::run() {
    sf::RenderWindow window(sf::VideoMode({GSettings.width, GSettings.height}),
                            GSettings.name);
    window.setFramerateLimit(60);

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

    std::vector<std::unique_ptr<const sf::RectangleShape>> platforms;

    platforms.emplace_back(
        std::make_unique<const sf::RectangleShape>(boundingBox1));
    platforms.emplace_back(
        std::make_unique<const sf::RectangleShape>(boundingBox2));
    sf::Clock clock;

    PlayerPhysics physics = {
        .velocity = {0.0f, 0.0f},
        .pos = {(float)GSettings.width / 2 - 15.0f, GSettings.groundY - 60.0f},
        .size = {30.0f, 60.0f},
        .jumpforce = 350.0f,
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

        player.update();

        handleCollisions(player, platforms);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            window.close();
        }

        window.draw(bgSprite);
        window.draw(treeSprite1);
        window.draw(treeSprite2);
        window.draw(boundingBox1);
        window.draw(boundingBox2);
        window.draw(player.getDrawObj());
        window.draw(groundLine);
        window.display();
    }
}

} // namespace ThePlayer
