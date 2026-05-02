#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <iostream>
#include <string>

#include "game.hpp"

using namespace ThePlayer;
using std::cout;

int main(int argc, char *argv[]) {
    Engine engine;
    engine.run();
    return 0;
};
