#include "invaders.hh"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(640, 400), "SpaceInvaders");
	window.setKeyRepeatEnabled(false);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	sf::Texture texturesprites;
	if (!texturesprites.loadFromFile("sprites.png")) {
		std::cerr << "Fichier sprites.png introuvable.\n";
		return 1;
	}
	sf::Sprite sinvader(texturesprites, sf::IntRect(0,0,11,8)), sjoueur(texturesprites, sf::IntRect(11,0,9,8));
	sinvader.setScale(2,2); sjoueur.setScale(2,2);

	jeu j;
	for (unsigned int y=0; y < 4; ++y)
		for (unsigned int i=0; i < 10; ++i)
			j.ajouter(invader(position(i*20,20+y*20), 10+i*20, 320-10*20+i*20, direction::droite));

	while (window.isOpen()) {
		j.tourdejeu();
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Left)
					j.directionjoueur(direction::gauche);
				else if (event.key.code == sf::Keyboard::Right)
					j.directionjoueur(direction::droite);
				else if (event.key.code == sf::Keyboard::Space)
					j.tirjoueur();
			}
		}
		window.clear(sf::Color::Black);
		for (auto const & i : j.invaders()) {
			sinvader.setPosition(i.pos().x()*2, i.pos().y()*2);
			window.draw(sinvader);
		}
		sf::RectangleShape rproj(sf::Vector2f(2,2));
		rproj.setFillColor(sf::Color::Red);
		for (auto const & i : j.projectiles()) {
			rproj.setPosition(i.pos().x()*2, i.pos().y()*2);
			window.draw(rproj);
		}
		sjoueur.setScale(static_cast<float>(j.joueur().tai().w())/9*2, 2.0);
		sjoueur.setPosition(j.joueur().pos().x()*2, j.joueur().pos().y()*2);
		window.draw(sjoueur);
		window.display();
	}
	return 0;
}
