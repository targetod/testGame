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
	//bound
	float x() const noexcept{ return shape.getPosition().x; }
	float y() const noexcept{ return shape.getPosition().y; }
	float left() const noexcept{ return x() - shape.getRadius(); }
	float right() const noexcept{ return x() + shape.getRadius(); }
	float top() const noexcept{ return y() - shape.getRadius(); }
	float bottom() const noexcept{ return y() + shape.getRadius(); }



	void update(){
		shape.move(velocity);
		solveBoundCollisions();
	}

	void draw(sf::RenderWindow& mTarget){
		mTarget.draw(shape);
	}

private:
	void solveBoundCollisions(){
		if (left() < 0)
			velocity.x = defVelocity;
		else if (right() > wndWidth)
			velocity.x = -defVelocity;

		if (top() < 0)
			velocity.y = defVelocity;
		else if (bottom() > wndHeight)
			velocity.y = -defVelocity;
	}
};


class Paddle
{
public:
	static const sf::Color defColor;
	static constexpr float defWidth{ 60.f };
	static constexpr float defHeight{ 20.f };
	static constexpr float defVelocity{ 8.f };

	sf::RectangleShape shape;
	sf::Vector2f velocity;

	Paddle(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setSize({ defWidth, defHeight });
		shape.setFillColor(defColor);
		shape.setOrigin(defWidth / 2.f, defHeight / 2.f);
	}

	void update(){
		processPlayerInput();
		shape.move(velocity);
	}

	void draw(sf::RenderWindow& mTarget){
		mTarget.draw(shape);
	}

	float x() const noexcept{ return shape.getPosition().x; }
	float y() const noexcept{ return shape.getPosition().y; }
	float width() const noexcept{ return shape.getSize().x; }
	float height() const noexcept{ return shape.getSize().y; }
	float left() const noexcept{ return x() - width() / 2.f; }
	float right() const noexcept{ return x() + width() / 2.f; }
	float top() const noexcept{ return y() - height() / 2.f; }
	float bottom() const noexcept{ return y() + height() / 2.f; }

private:
	void processPlayerInput()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) 
			&& left() > 0)
		{
			velocity.x = -defVelocity;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
			&& right() < wndWidth)
		{
			velocity.x = defVelocity;
		}
		else{
			velocity.x = 0;
		}
	}

};
const sf::Color Ball::defColor{ sf::Color::Red };
const sf::Color Paddle::defColor{ sf::Color::Red };


int main(int argc, char *argv[])
{
	Ball ball{ wndWidth / 2., wndHeight / 2.f };

	Paddle paddle{ wndWidth / 2, wndHeight - 50 };

	sf::RenderWindow window{ { wndWidth, wndHeight }, "Arkanoid - 2" };
	window.setFramerateLimit(60);

	while (true)
	{
		window.clear(sf::Color::Black);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			break;

		ball.update();
		paddle.update();

		ball.draw(window);
		paddle.draw(window);
		
		window.display();
	}
	
	
	return 0;
}