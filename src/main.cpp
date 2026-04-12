#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <string>

using std::cout;

struct GameSettings {
    std::string name;
    unsigned width;
    unsigned height;
};

const GameSettings GSettings = {
    .name = "ThePlayer",
    .width = 720,
    .height = 360,
};

struct Colors {
    sf::Color up = sf::Color::White;
    sf::Color down = sf::Color::Green;
    sf::Color left = sf::Color::Red;
    sf::Color right = sf::Color::Yellow;
};

struct Entity {
    float velocity;
    sf::RectangleShape rect;
};

Colors GColors;

int main(int argc, char *argv[]) {

    sf::RenderWindow window(sf::VideoMode({GSettings.width, GSettings.height}),
                            GSettings.name);
    window.setFramerateLimit(60);

    Entity player{.velocity = 100.0f,
                  .rect = sf::RectangleShape({20.0f, 20.0f})};

    player.rect.setSize(sf::Vector2f(20, 20));
    player.rect.setPosition(
        {(float)GSettings.width / 2 - 10, (float)GSettings.height / 2 - 10});

    sf::Clock clock;
    while (window.isOpen()) {

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear();

        float time = clock.restart().asSeconds();
        auto &rect = player.rect;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            rect.setFillColor(GColors.up);
            rect.move({0, -1.0f * player.velocity * time});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            rect.setFillColor(GColors.down);
            rect.move({0, player.velocity * time});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            rect.setFillColor(GColors.left);
            rect.move({-1.0f * player.velocity * time, 0});
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            rect.setFillColor(GColors.right);
            rect.move({player.velocity * time, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
            window.close();
        }

        window.draw(rect);
        window.display();
    }

    return 0;
}
