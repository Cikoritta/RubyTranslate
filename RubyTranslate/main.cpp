#include "Body.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800U, 600U), "Ruby Translator", sf::Style::Close);


	window.setFramerateLimit(30U);

	sf::Image icon; icon.loadFromFile("Data/icon.png");

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


	Body rubyTranslator;


	rubyTranslator.start();


	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();


			rubyTranslator.events(event, window);
		}


		rubyTranslator.update();


		window.clear();

		rubyTranslator.draw(window);

		window.display();
	}


	return 0U;
}
