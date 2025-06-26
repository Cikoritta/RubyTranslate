#include "InfoBox.h"

void InfoBox::drawShadowBlock(sf::RenderWindow& window)
{
	shadowBlock.setPosition(block.getPosition().x + 2.0f, block.getPosition().y);

	window.draw(shadowBlock);

	shadowBlock.setPosition(block.getPosition().x, block.getPosition().y + 2.0f);

	window.draw(shadowBlock);

	shadowBlock.setPosition(block.getPosition().x + 2.0f, block.getPosition().y + 2.0f);

	window.draw(shadowBlock);
}

void InfoBox::drawShadow(sf::RenderWindow& window)
{
	shadow.setPosition(text.getPosition().x + 2.0f, text.getPosition().y);

	window.draw(shadow);

	shadow.setPosition(text.getPosition().x, text.getPosition().y + 2.0f);

	window.draw(shadow);

	shadow.setPosition(text.getPosition().x + 2.0f, text.getPosition().y + 2.0f);

	window.draw(shadow);
}

InfoBox::InfoBox()
{
	texture.loadFromFile("Data/info.png");

	sprite.setTexture(texture);


	font.loadFromFile("Data/font.ttf");

	text.setFont(font);
	text.setFillColor(sf::Color(96U, 96U, 96U));
	text.setCharacterSize(33U);
	text.setLineSpacing(0.76f);

	block.setFont(font);
	block.setFillColor(sf::Color(96U, 96U, 96U));
	block.setCharacterSize(33U);
	block.setLineSpacing(0.76f);

	shadow.setFont(font);
	shadow.setFillColor(sf::Color(208U, 208U, 200U));
	shadow.setCharacterSize(33U);
	shadow.setLineSpacing(0.76f);

	shadowBlock.setFont(font);
	shadowBlock.setFillColor(sf::Color(208U, 208U, 200U));
	shadowBlock.setCharacterSize(33U);
	shadowBlock.setLineSpacing(0.76f);
}

void InfoBox::setString(const wstring& string)
{

}

void InfoBox::setInfo(sf::Uint32 block, sf::Uint32 ofBlock, sf::Uint32 text, sf::Uint32 ofText)
{
	this->text.setString(std::to_wstring(block) + L"\n" + std::to_wstring(ofBlock));
	shadow.setString(this->text.getString());

	this->block.setString(std::to_wstring(text) + L"\n" + std::to_wstring(ofText));
	shadowBlock.setString(this->block.getString());
}

void InfoBox::update()
{

}

void InfoBox::setPosition(const float x, const float y)
{
	sprite.setPosition(x, y);

	text.setPosition(roundf(sprite.getPosition().x + 82.0f), roundf(sprite.getPosition().y + 3.0f));

	block.setPosition(roundf(sprite.getPosition().x + 180.0f), roundf(sprite.getPosition().y + 3.0f));
}

void InfoBox::draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	drawShadow(window);

	drawShadowBlock(window);

	window.draw(text);

	window.draw(block);
}
