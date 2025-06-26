#include "TextBox.h"

TextBox::TextBox()
{
	texture.loadFromFile("Data/box.png");

	sprite.setTexture(texture);
	sprite.setScale(3.0f, 3.0f);


	font.loadFromFile("Data/font.ttf");

	text.setFont(font);
	text.setFillColor(sf::Color(96U, 96U, 96U));
	text.setCharacterSize(44U);
	text.setPosition(roundf(sprite.getPosition().x + 39.0f), roundf(sprite.getPosition().y + 1.0f));

	shadow.setFont(font);
	shadow.setFillColor(sf::Color(208U, 208U, 200U));
	shadow.setCharacterSize(44U);
}


void TextBox::setString(const wstring& string)
{
	text.setString(string);

	shadow.setString(string);
}

void TextBox::update()
{
	
}

void TextBox::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	
	drawShadow(window);

	window.draw(text);
}