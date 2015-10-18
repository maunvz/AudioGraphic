#ifndef graphics_def
#define graphics_def

#include <SFML/Graphics.hpp>
#include <mutex>
#include "ColorManager.h"

class GraphicsManager {
private:
	std::mutex updateMutex;
	sf::RenderWindow *window;
	sf::Thread *drawThread;
	double *channels;
	double *channels_target;
	void drawLoop();
	bool canDraw;
	void drawBar(int pos, float value, sf::RectangleShape *rectangle);
	void drawLabel(int pos, float value);
	Color channel_0;
	Color channel_1;
	sf::Font *font;
	char debug[1024];
public:
	GraphicsManager();
	void destroy();
	void setLevels(double *channels, double *num);
	void setColors(Color channel1, Color channel2);
	void draw();
	bool manageWindow();
	void setDebug(char* str);
};

#endif
