#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
using namespace std;

constexpr unsigned int wndWidth{ 800 }, wndHeight{ 600 };

class Ball
{
public:
	static const sf::Color defColor;
	static constexpr float defRadius{ 10.f };
	static constexpr float defVelocity{ 1.f };

	sf::CircleShape shape;

	sf::Vector2f velocity{ -defVelocity, -defVelocity };

	Ball(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setRadius(defRadius);
		shape.setFillColor(defColor);
		shape.setOrigin(defRadius, defRadius);
	}

	void update()
	{
		shape.move(velocity);
	}
	void draw(sf::RenderWindow& mTarget)
	{
		mTarget.draw(shape);
	}
};

const sf::Color Ball::defColor{ sf::Color::Red };

int main(int argc, char *argv[])
{
	Ball ball{ wndWidth / 2., wndHeight / 2.f };

	sf::RenderWindow window{ { wndWidth, wndHeight }, "Arkanoid - 2" };
	window.setFramerateLimit(60);

	while (true)
	{
		window.clear(sf::Color::Black);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			break;

		ball.update();
		ball.draw(window);


		window.display();
	}
	
	
	return 0;
}