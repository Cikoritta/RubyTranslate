#pragma once
#include <SFML/Graphics.hpp>


using string = std::string;

using wstring = std::wstring;


class Box
{
protected:

	sf::Texture		texture;

	sf::Sprite		sprite;


	sf::Font		font;

	sf::Text		text;

	sf::Text		shadow;


	virtual void	drawShadow(sf::RenderWindow& window);

public:

	virtual void	setPosition(const float x, const float y);

	virtual void	setString(const wstring& string) = 0;


	virtual void	update() = 0;

	virtual void	draw(sf::RenderWindow& window) = 0;
};