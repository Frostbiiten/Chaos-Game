#pragma once
#include <SFML/Graphics.hpp>
#include "ChaosShape.h"


namespace Manager
{
	std::size_t deltaTimeMs;
	sf::Clock deltaTimeClock;

	float baseCameraMovementSpeed = 0.002f;
	const sf::Vector2u resolution(800, 800);

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

	void Init()
	{
		sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Chaos Game");
		window.setView(sf::View(sf::Vector2f(), sf::Vector2f(resolution.x, resolution.y)));
		ChaosShape shape(resolution.x / 2.f, 100000, 3);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
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

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) shape.blend += 0.0001f * deltaTimeMs;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) shape.blend -= 0.0001f * deltaTimeMs;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) shape.blend = 0.5f;

			window.clear(sf::Color::Black);
			shape.Draw(window);
			window.display();
			deltaTimeMs = deltaTimeClock.restart().asMilliseconds();
		}
	}
}
