#include "ColorManager.h"
#include "LedManager.h"
#include <math.h>
#include <unistd.h>

#include <iostream>
#include <stdio.h>

#define SMOOTHING 8.0

ColorManager::ColorManager(LedManager *ledManager){
	offset = 0;
	this->ledManager = ledManager;
	std::thread t(&ColorManager::update, this);
	t.detach();
}

void ColorManager::update() {
	while(true){
		updateMutex.lock();
		smoothColorTo(&current_0, &target_0);
		smoothColorTo(&current_1, &target_1);
		ledManager->setLedColors(current_0, current_1);
		updateMutex.unlock();
		usleep(20000);
	}
}

void ColorManager::setColors(Color channel_0, Color channel_1) {
	updateMutex.lock();
	if(channel_0.a < 0.05) channel_0.a = 0.05;
	if(channel_1.a < 0.05) channel_1.a = 0.05;
	if(channel_0.a > 1.1) channel_0.a = 1.1;
	if(channel_1.a > 1.1) channel_1.a = 1.1;
	offset += .007;
	target_0 = channel_0;
	target_1 = channel_1;
	updateMutex.unlock();
}

void ColorManager::smoothColorTo(Color *currentc, Color *targetc){
	int offset = (int)this->offset;
	double *current = (double*)currentc;
	double *target = (double*)targetc;
	for(int i=0; i<3; i++) {
		if(current[(i+offset)%3] == 0) current[(i+offset)%3] = target[i];

		double speed = fabs(current[(i+offset)%3]-target[i])/SMOOTHING;
		if(current[(i+offset)%3] < target[i]){
				current[(i+offset)%3] += speed;
		} else if (current[(i+offset)%3] > target[i]){
				current[(i+offset)%3] -= speed;
		}
	}

	if(current[3] == 0) current[3] = target[3];
	double speed = fabs(current[3]-target[3])/SMOOTHING;
	if(current[3] < target[3]){
			current[3] += speed;
	} else if (current[3] > target[3]){
			current[3] -= speed;
	}

}

Color ColorManager::getColor0() {
	return current_0;
}

Color ColorManager::getColor1() {
	return current_1;
}
