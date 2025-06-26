#pragma once
#include "Box.h"


class TextBox : public Box
{

public:

	TextBox();


	void			setString(const wstring& string) override;

	void			update() override;

	void			draw(sf::RenderWindow& window) override;
};
