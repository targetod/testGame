#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
using namespace std;

constexpr unsigned int wndWidth{ 800 }, wndHeight{ 600 };

class Ball
{
public:
	static const sf::Color def
};

int main(int argc, char *argv[])
{
	sf::RenderWindow window{ { wndWidth, wndHeight }, "Arkanoid - 1" };
	window.setFramerateLimit(60);

	while (true)
	{
		window.clear(sf::Color::Black);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			break;
		window.display();
	}
	
	
	return 0;
}