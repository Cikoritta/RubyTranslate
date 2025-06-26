#include "Body.h"

void Body::start()
{
	backgroundTexture.loadFromFile("Data/background.png");

	background.setTexture(backgroundTexture);


	parser.start();
}

void Body::update()
{
	parser.update();
}

void Body::events(sf::Event& event, sf::RenderWindow& window)
{
	parser.events(event, window);
}

void Body::draw(sf::RenderWindow& window)
{
	window.draw(background);

	parser.draw(window);
}
