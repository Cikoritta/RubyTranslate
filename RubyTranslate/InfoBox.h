#pragma once
#include "Box.h"

class InfoBox : public Box
{
	sf::Text	block;

	sf::Text	shadowBlock;


	void		drawShadowBlock(sf::RenderWindow& window);

	void		drawShadow(sf::RenderWindow& window) override;

public:

	InfoBox();


	void		setString(const wstring& string) override;

	void		setInfo(sf::Uint32 block, sf::Uint32 ofBlock, sf::Uint32 text, sf::Uint32 ofText);

	void		update() override;

	void		setPosition(const float x, const float y) override;

	void		draw(sf::RenderWindow& window) override;
};
