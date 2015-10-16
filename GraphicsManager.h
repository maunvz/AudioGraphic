#ifndef graphics_def
#define graphics_def

#include <SFML/Graphics.hpp>
#include "ColorManager.h"

class GraphicsManager {
private:
	sf::RenderWindow *window;
	sf::Thread *drawThread;
	double *channels;
	double *channels_target;
	void drawLoop();
	bool canDraw;
	void drawBar(int pos, float value);
public:
	GraphicsManager();
	void destroy();
	void setLevels(double *channels, double *num);
	void setColors(Color channel1, Color channel2);
	void draw();
	bool manageWindow();
};

#endif
