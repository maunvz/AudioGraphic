#include "GraphicsManager.h"

GraphicsManager::GraphicsManager(){
	sf::ContextSettings settings;
	window = new sf::RenderWindow(sf::VideoMode(600, 200), "Equalizer", sf::Style::Close, settings);
}

void GraphicsManager::destroy(){
	free(window);
}

void GraphicsManager::draw(double *channels, double *num) {
	window->clear(sf::Color(0,0,0,255));
	window->display();
}

bool GraphicsManager::manageWindow() {
	sf::Event event;
	 while (window->pollEvent(event)) {
		 if (event.type == sf::Event::Closed) {
			 window->close();
			 return false;
		 }
	 }
	 return true;
}
