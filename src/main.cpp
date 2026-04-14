#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <string>

using std::cout;

struct GameSettings {
    std::string name;
    unsigned width;
    unsigned height;

    float groundY;
};

const GameSettings GSettings = {.name = "ThePlayer",
                                .width = 640,
                                .height = 640,
                                .groundY = 320.0f + 40 + 20};

struct Colors {
    sf::Color up = sf::Color::Cyan;
    sf::Color down = sf::Color::Green;
    sf::Color left = sf::Color::Red;
    sf::Color right = sf::Color::Yellow;
    sf::Color idle = sf::Color::White;
};

struct Entity {
    float baseVelocity;
    sf::Vector2f velocity;
    sf::RectangleShape rect;
    float gravity;
    float jumpforce;
    float friction;
};

Colors GColors;

bool isGrounded(const sf::RectangleShape &rect) {
    return rect.getPosition().y + rect.getSize().y >= GSettings.groundY;
}

int main(int argc, char *argv[]) {

    sf::RenderWindow window(sf::VideoMode({GSettings.width, GSettings.height}),
                            GSettings.name);
    window.setFramerateLimit(60);

    sf::RectangleShape groundLine({GSettings.width * 1.0f, 2});
    groundLine.setFillColor(sf::Color::Red);
    groundLine.setPosition({0, GSettings.groundY});

    Entity player{.baseVelocity = 100.0f,
                  .velocity = {0.0f, 0.0},
                  .rect = sf::RectangleShape({20.0f, 20.0f}),
                  .gravity = 800.0f,
                  .jumpforce = 350.0f,
                  .friction = 0.9f};

    player.rect.setPosition(
        //{(float)GSettings.width / 2 - 10, (float)GSettings.height / 2 + 40});
        {(float)GSettings.width / 2 - 10,
         GSettings.groundY - player.rect.getSize().y});

    sf::Texture bgTexture;

    if (!bgTexture.loadFromFile("assets/bg.png")) {
        std::cerr << "Error loading background image;\n";
        exit(EXIT_FAILURE);
    }
#if 0
    if(!bgTexture.resize({720, 720})){
        std::cerr << "Error resizeing background image";
        exit(EXIT_FAILURE);
    }
#endif
    sf::Sprite bgSprite(bgTexture);
    bgSprite.setPosition({0.0f, 0.0f});

    sf::Clock clock;

    // std::cout << player.rect.getPosition().y << "\n";
    while (window.isOpen()) {

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear();
        window.draw(bgSprite);

        float time = clock.restart().asSeconds();
        auto &rect = player.rect;
        rect.setFillColor(GColors.idle);

        if (!isGrounded(rect)) {
            player.velocity.y += player.gravity * time;
        }
        if (player.velocity.y > 0 && rect.getPosition().y + rect.getSize().y >= GSettings.groundY) {
            //std::cout << "below ground\n";
            rect.setPosition(
                {rect.getPosition().x, GSettings.groundY - rect.getSize().y});
            player.velocity.y = 0;
        }

        float offsetY = player.velocity.y * time;
        // offsetY = std::min(GSettings.groundY - rect.getPosition().y,
        float newPosY = rect.getPosition().y + rect.getSize().y + offsetY;
        if (newPosY > GSettings.groundY) {
            //std::cout << "Possibility to move below ground\n";
            offsetY = GSettings.groundY - (rect.getPosition().y + rect.getSize().y);
        }
        rect.move({0.0f, offsetY});

        if (isGrounded(rect) &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            player.velocity.y = -player.jumpforce;
        }
        if (player.velocity.x < 0.000005 || player.velocity.x > -0.000005) {
            player.velocity.x = 0;
        }

        if (player.velocity.x != 0) {
            player.velocity.x *= player.friction;
        }
#if 0
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            rect.setFillColor(GColors.up);
            rect.move({0, -1.0f * player.velocity.x * time});
        } 

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            rect.setFillColor(GColors.down);
            rect.move({0, player.velocity.x * time});
        }
#endif
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            rect.setFillColor(GColors.left);
            player.velocity.x = -1.0f * player.baseVelocity;
            // rect.move({-1.0f * player.velocity.x * time, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            rect.setFillColor(GColors.right);
            player.velocity.x = 1.0f * player.baseVelocity;
            // rect.move({player.velocity.x * time, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            window.close();
        }

        rect.move({player.velocity.x * time, 0});

        window.draw(rect);
        window.draw(groundLine);
        window.display();
    }

    return 0;
}
