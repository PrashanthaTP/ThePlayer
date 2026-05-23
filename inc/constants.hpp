#pragma once
#include <SFML/Graphics.hpp>

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

extern const GameSettings GSettings;
extern const Colors GColors;
