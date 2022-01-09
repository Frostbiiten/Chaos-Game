#pragma once
#include "random.hpp"
#include <SFML/Graphics.hpp>
#include <numbers>

inline sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, const float& t)
{
	return a + t * (b - a);
}

class ChaosShape
{
	effolkronium::random_local rand;
	std::vector<sf::Vector2f> vertices;
	sf::VertexArray sampleArray;
	std::size_t sampleCount;

	sf::Vector2f xBounds;
	sf::Vector2f yBounds;

public:
	float blend;
	ChaosShape(float scale, std::size_t samples, std::size_t points = 3)
	{
		rand = effolkronium::random_local{};
		sampleCount = samples;
		blend = 0.5f;

		xBounds = sf::Vector2f(-scale, scale) / 2.f;
		yBounds = xBounds;

		// Generate vertices of polygon
		vertices.reserve(points);
		float step = std::numbers::pi * 2.f / points;
		for (std::size_t x = 0; x < points; ++x)
		{
			float radians = step * x - std::numbers::pi / 2.f;
			vertices.push_back(sf::Vector2f(std::cosf(radians), std::sinf(radians)) * scale);
		}

		sampleArray = sf::VertexArray(sf::Points, samples);
	}

	void Draw(sf::RenderWindow& window)
	{
		// Generate random point in bounds
		sf::Vector2f pos(rand.get(xBounds.x, xBounds.y), rand.get(yBounds.x, xBounds.y));

		// Run for every vertex
		for (std::size_t x = 0; x < sampleCount; ++x)
		{
			// Choose a random vertex on the polygon to move towards
			std::size_t randPoint = rand.get(std::size_t(0), vertices.size() - 1);

			// Move towards the point and write to vert array
			pos = Lerp(pos, vertices[randPoint], blend);
			sampleArray[x] = pos;
		}

		// Draw all points to window
		window.draw(&sampleArray[0], sampleCount, sf::Points);
	}

	// Chosen jump point cannot be previous point
	void DrawVariantA(sf::RenderWindow& window)
	{
		// Generate random point in bounds
		sf::Vector2f pos(rand.get(xBounds.x, xBounds.y), rand.get(yBounds.x, xBounds.y));

		// Run for every vertex
		std::size_t prevPoint = vertices.size();
		for (std::size_t x = 0; x < sampleArray.getVertexCount(); ++x)
		{
			// Choose a random vertex on the polygon to move towards
			std::size_t randPoint = rand.get(std::size_t(0), vertices.size() - 1);
			if (randPoint == prevPoint) continue;
			prevPoint = randPoint;

			// Move towards the point and write to vert array
			pos = Lerp(pos, vertices[randPoint], blend);
			sampleArray[x] = pos;
		}

		// Draw all points to window
		window.draw(&sampleArray[0], sampleArray.getVertexCount(), sf::Points);
	}

	// Chosen jump point cannot be  to the right of last jump point
	void DrawVariantB(sf::RenderWindow& window)
	{
		// Generate random point in bounds
		sf::Vector2f pos(rand.get(xBounds.x, xBounds.y), rand.get(yBounds.x, xBounds.y));

		// Run for every vertex
		std::size_t prevPoint = vertices.size();
		for (std::size_t x = 0; x < sampleArray.getVertexCount(); ++x)
		{
			// Choose a random vertex on the polygon to move towards
			std::size_t randPoint = rand.get(std::size_t(0), vertices.size() - 1);
			if (randPoint == prevPoint + 1) continue;
			prevPoint = randPoint;

			// Move towards the point and write to vert array
			pos = Lerp(pos, vertices[randPoint], blend);
			sampleArray[x] = pos;
		}

		// Draw all points to window
		window.draw(&sampleArray[0], sampleArray.getVertexCount(), sf::Points);
	}
};

