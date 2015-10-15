#ifndef graphics_def
#define graphics_def

#include <SFML/Graphics.hpp>

class GraphicsManager {
private:
	sf::RenderWindow *window;
public:
	GraphicsManager();
	void destroy();
	void draw(double *channels, double *num);
	bool manageWindow();
};

#endif
