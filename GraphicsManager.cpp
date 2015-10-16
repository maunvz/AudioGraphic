#include "GraphicsManager.h"
#include <string.h>
#include <unistd.h>

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 200
#define CHANNEL_NUM 30
#define DISPLAY_NUM 27
#define SMOOTHING 10

GraphicsManager::GraphicsManager() {
	/* Create window */
	window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
																"Equalizer",
																sf::Style::Close);
	channels = new double[CHANNEL_NUM];
	channels_target = new double[CHANNEL_NUM];
	/* Drawing things */
	font = new sf::Font();
	if (!font->loadFromFile("Resources/osr.ttf")){
		printf("Could not load font\n");
	}

	/* Create render thread */
	canDraw = true;
	window->setActive(false);
	drawThread = new sf::Thread(&GraphicsManager::drawLoop, this);
	drawThread->launch();
}

/* Runs on a seperate thread and only writes, so no mutex needed */
void GraphicsManager::setLevels(double *channels, double *num) {
	/* Currently there are 30 channels */
	updateMutex.lock();
	memcpy(this->channels_target, channels, CHANNEL_NUM*sizeof(double));
	updateMutex.unlock();
}

void GraphicsManager::setColors(Color color_0, Color color_1){
	channel_0 = color_0;
	channel_1 = color_1;
	/* show the color (add two squares?) use 3 windows? */
	/* particle cannon display */
}

void GraphicsManager::drawLoop(){
	while(canDraw){
		updateMutex.lock();
		for(int i=0; i<CHANNEL_NUM; i++){
			channels[i] = channels_target[i]-channels[i]/(double)SMOOTHING;
		}
		updateMutex.unlock();
		draw();
		usleep(1000*20);
	}
}

/* pos is 0-29, value is 0-1 */
void GraphicsManager::drawBar(int pos, float value, sf::RectangleShape *rectangle){
	Color c = channel_0;
	/* For now simple rectangles that fill up width exactly, no padding
	 * or style of any kind */
	float width = (float)WINDOW_WIDTH/(float)DISPLAY_NUM;
 	float height = value * (float)WINDOW_HEIGHT;
 	rectangle->setFillColor(sf::Color(c.r*255,c.g*255,c.b*255,255)); /* interpolate color from end to end */
 	rectangle->setSize(sf::Vector2f(width, height));
 	rectangle->setPosition(width*pos, WINDOW_HEIGHT-height);
 	window->draw(*rectangle);
}

/* Only reads from channels and num so no mutex needed */
void GraphicsManager::draw(){
	window->clear(sf::Color(0,0,0,255));
	/* Draw things here */
	sf::RectangleShape rectangle;
	for(int i=CHANNEL_NUM-DISPLAY_NUM; i<CHANNEL_NUM; i++){
		drawBar(i-(CHANNEL_NUM-DISPLAY_NUM), channels[i]/80.0, &rectangle);
	}

	sf::Text text;
	text.setFont(*font);
	text.setString(debug);
	text.setCharacterSize(24);
	text.setColor(sf::Color::White);
	window->draw(text);

	/* End draw */
	window->display();
}

void GraphicsManager::setDebug(char *str){
	strncpy(debug, str, 1024);
}

bool GraphicsManager::manageWindow() {
	sf::Event event;
	 while (window->pollEvent(event)) {
		 if (event.type == sf::Event::Closed) {
			 canDraw = false;
			 drawThread->wait();
			 window->close();
			 return false;
		 }
	 }
	 return true;
}

void GraphicsManager::destroy() {
	free(window);
	free(drawThread);
	free(channels);
	free(channels_target);
	free(font);
}
