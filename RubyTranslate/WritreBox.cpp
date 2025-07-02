#include "WritreBox.h"


WritreBox::WritreBox()
{
	texture.loadFromFile("Data/writeBox.png");

	sprite.setTexture(texture);


	font.loadFromFile("Data/font.ttf");

	text.setFont(font);
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(44U);
	text.setPosition(roundf(sprite.getPosition().x + 30.0f), roundf(sprite.getPosition().y + 1.0f));

	shadow.setFont(font);
	shadow.setFillColor(sf::Color(216U, 208U, 216U));
	shadow.setCharacterSize(44U);
}


void WritreBox::setString(const wstring& string)
{
	writingText = string;

	text.setString(writingText);

	shadow.setString(writingText);
}

wstring WritreBox::getString()
{
    return writingText;
}


void WritreBox::update()
{
     
}

void WritreBox::event(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::F1)
            writingText += L"\\p";

        if (event.key.code == sf::Keyboard::F2)
            writingText += L"\\l";

        if (event.key.code == sf::Keyboard::F3)
            writingText += L"$";

        if (event.key.code == sf::Keyboard::F4)
            writingText += L"{PLAYER}";

        if (event.key.code == sf::Keyboard::F5)
            writingText += L"{STR_VAR_1}";

        if (event.key.code == sf::Keyboard::F6)
            writingText += L"{STR_VAR_2}";

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
        {
            if (event.key.code == sf::Keyboard::V)
            {
                writingText += sf::Clipboard::getString();
            }
        }      

        if (event.key.code == sf::Keyboard::Numpad1)
            writingText += L"“";

        if (event.key.code == sf::Keyboard::Numpad2)
            writingText += L"”";

        setString(writingText);
    }

    if (event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode == 8)
        {
            if (!writingText.empty())
                writingText.pop_back();
        }
        else if (event.text.unicode == 13)
        {
            writingText += L"\n";
        }
        else if (event.text.unicode >= 32 && event.text.unicode != 127)
        {
            writingText += static_cast<wchar_t>(event.text.unicode);
        }

        setString(writingText);
    }
}

void WritreBox::draw(sf::RenderWindow& window)
{
	window.draw(sprite);

	drawShadow(window);

	window.draw(text);
}

void WritreBox::setPosition(const float x, const float y)
{
    sprite.setPosition(x, y);

    text.setPosition(roundf(sprite.getPosition().x + 25.0f), roundf(sprite.getPosition().y + 1.0f));
}
