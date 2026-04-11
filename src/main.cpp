#include <SFML/Graphics.hpp>
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
		window.display();
	}
 
	return 0;
}
