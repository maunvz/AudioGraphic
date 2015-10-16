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
	/* Create render thread */
	canDraw = true;
	window->setActive(false);
	drawThread = new sf::Thread(&GraphicsManager::drawLoop, this);
	drawThread->launch();
	channels = new double[CHANNEL_NUM];
	channels_target = new double[CHANNEL_NUM];
}

/* Runs on a seperate thread and only writes, so no mutex needed */
void GraphicsManager::setLevels(double *channels, double *num) {
	/* Currently there are 30 channels */
	memcpy(this->channels_target, channels, CHANNEL_NUM*sizeof(double));
}

void GraphicsManager::setColors(Color color_0, Color color_1){
	/* show the color (add two squares?) use 3 windows? */
	/* particle cannon display */
}

void GraphicsManager::drawLoop(){
	while(canDraw){
		for(int i=0; i<CHANNEL_NUM; i++){
			channels[i] = channels_target[i]-channels[i]/(double)SMOOTHING;
		}
		draw();
		usleep(1000*20);
	}
}

/* pos is 0-29, value is 0-1 */
void GraphicsManager::drawBar(int pos, float value){
	/* For now simple rectangles that fill up width exactly, no padding
	 * or style of any kind */
	float width = (float)WINDOW_WIDTH/(float)DISPLAY_NUM;
 	float height = value * (float)WINDOW_HEIGHT;
 	sf::RectangleShape rectangle;
 	rectangle.setFillColor(sf::Color::Red);
 	rectangle.setSize(sf::Vector2f(width, height));
 	rectangle.setPosition(width*pos, WINDOW_HEIGHT-height);
 	window->draw(rectangle);
}

/* Only reads from channels and num so no mutex needed */
void GraphicsManager::draw(){
	window->clear(sf::Color(0,0,0,255));
	/* Draw things here */
	for(int i=0; i<CHANNEL_NUM; i++){
		drawBar(i, channels[i+CHANNEL_NUM-DISPLAY_NUM]/80.0);
	}
	/* End draw */
	window->display();
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
}
