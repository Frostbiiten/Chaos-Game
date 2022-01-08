#include <SfML/Graphics.hpp>
#include <iostream>
#include "random.hpp"

inline sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, const float& t)
{
	return a + t * (b - a);
}

float blend = 0.5f;
std::size_t deltaTimeMs;
sf::Clock deltaTimeClock;

sf::Vector2f circlePoint(float degrees)
{
	float radians = degrees / 57.2958f;
	return sf::Vector2f(cosf(radians), sinf(radians));
}

struct ChaosTriangle
{
	float size;
	effolkronium::random_local rand;

	/*
	sf::Vector2f pointA;
	sf::Vector2f pointB;
	sf::Vector2f pointC;
	sf::Vector2f pointD;
	sf::Vector2f pointE;
	sf::Vector2f pointF;
	*/
	std::vector<sf::Vector2f> points;

	sf::CircleShape point;

	sf::VertexArray vertArray;

	ChaosTriangle(float size, std::size_t samples, std::size_t points = 3)
	{
		this->size = size;
		rand = effolkronium::random_local{};

		//pointA = sf::Vector2f(-0.866, -0.5) * size;
		//pointB = sf::Vector2f(0.866, -0.5) * size;
		//pointC = sf::Vector2f(0.0, 1.0) * size;
		/*
		pointA = sf::Vector2f(1, 0);
		pointB = sf::Vector2f(-1, 0);
		pointC = sf::Vector2f(0.5, 0.86602540378);
		pointD = sf::Vector2f(-0.5, 0.86602540378);
		pointE = sf::Vector2f(-0.5, -0.86602540378);
		pointF = sf::Vector2f(0.5, -0.86602540378);
		*/
		this->points.reserve(points);
		float step = 360.f / points;
		for (std::size_t x = 0; x < points; ++x)
		{
			if(x % 2 == 0)
				this->points.push_back(circlePoint(step * x));
			else
				this->points.push_back(circlePoint(step * x) * 2.f);
		}

		point.setPointCount(3);
		point.setRadius(0.4);
		point.setFillColor(sf::Color::White);
		vertArray = sf::VertexArray(sf::Points, samples);
	}

	void Draw(sf::RenderWindow& window)
	{
		sf::Vector2f pos(rand.get(-size, size), rand.get(-size, size));
		for (std::size_t x = 0; x < vertArray.getVertexCount(); ++x)
		{
			std::size_t randPoint = rand.get(std::size_t(0), points.size() - 1);
			pos = lerp(pos, points[randPoint], blend);

			/*
			if (randPoint == 0) pos = lerp(pos, pointA, blend);
			else if (randPoint == 1) pos = lerp(pos, pointB, blend);
			else if (randPoint == 2) pos = lerp(pos, pointC, blend);
			else if (randPoint == 3) pos = lerp(pos, pointD, blend);
			else if (randPoint == 4) pos = lerp(pos, pointE, blend);
			else pos = lerp(pos, pointF, blend);
			*/

			vertArray[x] = pos;
			//point.setPosition(pos);
			//window.draw(point);
		}

		window.draw(&vertArray[0], vertArray.getVertexCount(), sf::Points);
	}
};

float baseCameraMovementSpeed = 0.002f;

sf::View ProcessCameraMovement(sf::View cameraView)
{
	//Directional movement
	sf::Vector2f inputDirection;
	inputDirection.x += sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	inputDirection.x -= sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	inputDirection.y += sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	inputDirection.y -= sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	cameraView.move(inputDirection * baseCameraMovementSpeed * cameraView.getSize().x * (float)deltaTimeMs);

	//Zoom
	float zoomDir = 0.f;
	zoomDir += sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
	zoomDir -= sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
	cameraView.zoom(1.f + zoomDir * baseCameraMovementSpeed * deltaTimeMs);

	return cameraView;
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "Order from chaos");
	ChaosTriangle tri(300, 100000, 12);
	sf::View v = window.getView();
	v.setCenter(sf::Vector2f(0, 0));
	window.setView(v);
	while (window.isOpen())
	{
		sf::Event event;
		while(window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
			if (event.type == event.KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
					break;
				}
			}
		}

		window.setView(ProcessCameraMovement(window.getView()));

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) blend += 0.01f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) blend -= 0.01f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) blend = 0.001f;

		window.clear(sf::Color::Black);
		tri.Draw(window);

		window.display();
		deltaTimeMs = deltaTimeClock.restart().asMilliseconds();
	}
	return 0;
}