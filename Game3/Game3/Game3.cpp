#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>
using namespace std;

constexpr unsigned int wndWidth{ 800 }, wndHeight{ 600 };


struct Rectangle
{
	sf::RectangleShape shape;
	
	float x() const noexcept{ return shape.getPosition().x; }
	float y() const noexcept{ return shape.getPosition().y; }
	float width() const noexcept{ return shape.getSize().x; }
	float height() const noexcept{ return shape.getSize().y; }
	float left() const noexcept{ return x() - width() / 2.f; }
	float right() const noexcept{ return x() + width() / 2.f; }
	float top() const noexcept{ return y() - height() / 2.f; }
	float bottom() const noexcept{ return y() + height() / 2.f; }
};

struct Circle
{
	sf::CircleShape shape;
	
	float x() const noexcept{ return shape.getPosition().x; }
	float y() const noexcept{ return shape.getPosition().y; }
	float radius() const noexcept { return shape.getRadius();}
	float left() const noexcept  { return x() - radius(); }
	float right() const noexcept { return x() + radius(); }
	float top() const noexcept   { return y() - radius(); }
	float bottom() const noexcept{ return y() + radius(); }

};


class Ball : public Circle
{
public:
	static const sf::Color defColor;
	static constexpr float defRadius{ 10.f };
	static constexpr float defVelocity{ 4.f };

	sf::Vector2f velocity{ -defVelocity, -defVelocity };

	Ball(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setRadius(defRadius);
		shape.setFillColor(defColor);
		shape.setOrigin(defRadius, defRadius);
	}
	
	void update(){
		shape.move(velocity);
		solveBoundCollisions();
	}

	void draw(sf::RenderWindow& mTarget){
		mTarget.draw(shape);
	}

private:
	void solveBoundCollisions() noexcept{
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


class Paddle : public Rectangle
{
public:
	static const sf::Color defColor;
	static constexpr float defWidth{ 60.f };
	static constexpr float defHeight{ 20.f };
	static constexpr float defVelocity{ 8.f };

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

class Brick : public Rectangle
{
public:
	static const sf::Color defColor;
	static constexpr float defWidth{60.f};
	static constexpr float defHeight{20.f};
	static constexpr float defVelocity{8.f};
	
	bool destroyed{false};
	
	Brick(float mX, float mY)
	{
		shape.setPosition(mX, mY);
		shape.setSize({ defWidth, defHeight });
		shape.setFillColor(defColor);
		shape.setOrigin(defWidth / 2.f, defHeight / 2.f);
	}
	
	void update()
	{
	}
	void draw(sf::RenderWindow& mTarget){
		mTarget.draw(shape);
	}
	
};
const sf::Color Ball::defColor   { sf::Color::Red };
const sf::Color Paddle::defColor { sf::Color::Red };
const sf::Color Brick::defColor  { sf::Color::Yellow};


// peresechenie 2x ob*ektov
template <typename T1, typename T2>
bool isIntersecting(const T1& lft, const T2& rgt) noexcept
{
	return lft.right() >= rgt.left()
		&& lft.left() <= rgt.right()
		&& lft.bottom() >= rgt.top()
		&& lft.top() <= rgt.bottom();
}

void solvePaddleBallCollision(const Paddle& mPaddle, Ball& mBall) noexcept
{
	if (!isIntersecting(mPaddle, mBall)) return;
	
	mBall.velocity.y = -Ball::defVelocity;
	
// Direction ball
	mBall.velocity.x = mBall.x() < mPaddle.x() ?
		-Ball::defVelocity : Ball::defVelocity;
}

void solveBrickBallCollision(Brick& mBrick, Ball& mBall) noexcept
{
	if (!isIntersecting(mBrick, mBall)) return;
	
	mBrick.destroyed = true;
	
	float overlapLeft{ mBall.right() - mBrick.left() };
	float overlapRight{ mBrick.right() - mBall.left() };
	float overlapTop{ mBall.bottom() - mBrick.top() };
	float overlapBottom{ mBrick.bottom() - mBall.top() };
	
	bool ballFromLeft(std::abs(overlapLeft) < std::abs(overlapRight));
	bool ballFromTop(std::abs(overlapTop) < std::abs(overlapBottom));

	float minOverlapX{ ballFromLeft ? overlapLeft :  overlapRight};
	float minOverlapY{ ballFromTop ? overlapTop :  overlapBottom};
	
	if (std::abs(minOverlapX) < std::abs(minOverlapY))
	{
		mBall.velocity.x = ballFromLeft ? -Ball::defVelocity : Ball::defVelocity;
	}
	else{
		mBall.velocity.y = ballFromTop ? -Ball::defVelocity : Ball::defVelocity;
	}
		
}

int main(int argc, char *argv[])
{
	constexpr int brkCountX{11}, brkCountY{5};
	constexpr int brkStartColumn{1}, brkStartRow{2};
	constexpr float brkSpacing{3.f}, brkOffsetX{22.f};
	
	Ball ball{ wndWidth / 2., wndHeight / 2.f };
	Paddle paddle{ wndWidth / 2, wndHeight - 50 };
	std::vector<Brick> bricks;
	
	for (int iX{ 0 }; iX < brkCountX; ++iX)
		for (int iY{ 0 }; iY < brkCountY; ++iY)
		{
			float x{ (iX + brkStartColumn) * (Brick::defWidth + brkSpacing)};
			float y{ (iY + brkStartRow) * (Brick::defHeight + brkSpacing)};
			
			bricks.emplace_back(brkOffsetX + x, y);
		}
	
	sf::RenderWindow window{ { wndWidth, wndHeight }, "Arkanoid - 2" };
	window.setFramerateLimit(60);

	while (true)
	{
		window.clear(sf::Color::Black);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			break;
		
		ball.update();
		paddle.update();

		for (auto& brick : bricks)
		{
			brick.update();
			solveBrickBallCollision(brick, ball);
		}
		bricks.erase(std::remove_if(std::begin(bricks), std::end(bricks),
				[](const auto& mBrick) { return mBrick.destroyed; }),
			std::end(bricks)); 	
		
		solvePaddleBallCollision(paddle, ball);
		
		ball.draw(window);
		paddle.draw(window);
		for (auto& brick : bricks)
			brick.draw(window);
		
		window.display();
	}
	
	
	return 0;
}