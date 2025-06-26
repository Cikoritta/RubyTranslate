#pragma once
#include "Parser.h"

class Body
{
	sf::Texture		backgroundTexture;

	sf::Sprite		background;


	Parser			parser;

public:

	Body() = default;

	void			start();

	void			update();

	void			events(sf::Event& event, sf::RenderWindow& window);

	void			draw(sf::RenderWindow& window);
};
