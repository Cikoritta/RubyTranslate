#include "Box.h"

void Box::setPosition(const float x, const float y)
{
	sprite.setPosition(x, y);

	text.setPosition(roundf(sprite.getPosition().x + 38.0f), roundf(sprite.getPosition().y + 1.0f));
}

void Box::drawShadow(sf::RenderWindow& window)
{
	shadow.setPosition(text.getPosition().x + 3.0f, text.getPosition().y);

	window.draw(shadow);

	shadow.setPosition(text.getPosition().x, text.getPosition().y + 3.0f);

	window.draw(shadow);

	shadow.setPosition(text.getPosition().x + 3.0f, text.getPosition().y + 3.0f);

	window.draw(shadow);
}