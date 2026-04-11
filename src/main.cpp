#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
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
	sf::Color up = sf::Color::Blue;
	sf::Color down = sf::Color::Green;
	sf::Color left = sf::Color::Red;
	sf::Color right = sf::Color::Yellow;
};

Colors GColors;

int main (int argc, char *argv[]) {
	
	sf::RenderWindow window(sf::VideoMode({GSettings.width, GSettings.height}),
						    GSettings.name);
	window.setFramerateLimit(60);


	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(20,20));
	rect.setPosition({(float)GSettings.width/2-10, (float)GSettings.height/2-10});

	while(window.isOpen()){
	
		while(const auto event = window.pollEvent()){
			if(event->is<sf::Event::Closed>()){
				window.close();
			}
		}
		window.clear();
		window.draw(rect);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
			rect.setFillColor(GColors.up);
			rect.move({0, -5.0f});
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
			rect.setFillColor(GColors.down);
			rect.move({0, 5.0f});
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
			rect.setFillColor(GColors.left);
			rect.move({-5.0f, 0});
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
			rect.setFillColor(GColors.right);
			rect.move({5.0f, 0});
		}
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)){
			window.close();
		}
		window.display();
	}
 
	return 0;
}
