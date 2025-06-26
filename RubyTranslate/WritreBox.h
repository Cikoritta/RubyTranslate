#pragma once
#include "Box.h"

class WritreBox : public Box
{
	wstring		writingText;

public:

	WritreBox();


	void		setString(const wstring& string) override;

	wstring		getString();

	void		update() override;

	void		event(sf::Event& event);

	void		draw(sf::RenderWindow& window) override;

	void		setPosition(const float x, const float y) override;
};
