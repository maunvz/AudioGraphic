#ifndef color_def
#define color_def

#include <thread>
#include <mutex>

class LedManager;
class GraphicsManager;

typedef struct {
	double r;
	double g;
	double b;
	double a;
} Color;

class ColorManager {
private:
	LedManager *ledManager;
	GraphicsManager *graphicsManager;
	Color current_0;
	Color current_1;
	Color target_0;
	Color target_1;
	double offset;
	std::mutex updateMutex;
	void update();
	void smoothColorTo(Color *current, Color *target);
public:
	ColorManager(LedManager *ledManager, GraphicsManager *graphicsManager);
	void setColors(Color channel1, Color channel2);
	Color getColor0();
	Color getColor1();
};

#endif
