#ifndef led_def
#define led_def

#include <wiringPi.h>
#include <softPwm.h>
#include "ColorManager.h"

class LedManager {
public:
	void setLedColors(Color channel1, Color channel2);
	void ledInit();
};

#endif
