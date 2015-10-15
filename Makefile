# build an executable called ag from main.cpp
libs_for_mac = -framework CoreAudio -framework AudioUnit -framework Carbon\
-framework AudioToolbox -framework CoreServices libs/mac/libportaudio.a\
-framework SFML -framework sfml-graphics -framework sfml-window -framework\
sfml-system
libs_for_unix = -lrt -pthread -lasound -ljack libs/unix/libportaudio.so\
-lsfml-graphics -lsfml-window -lsfml-system -ludev
libs_for_pi = -lrt -lasound -lwiringPi libs/rpi/libportaudio.a -DUSE_LEDS
defs_for_pi = -DUSE_LEDS
defs_for_unix = -DUSE_GUI
defs_for_mac = -DUSE_GUI
libs = $(libs_for_unix) -lpthread -lfftw3 -lm
defs = $(defs_for_unix)
inc = -I/usr/local/include -L/usr/local/lib

obj = SoundManager.o LedManager.o GraphicsManager.o ColorManager.o

main: main.cpp $(obj)
	g++ -g -Wall -o ag main.cpp $(obj) $(inc) $(libs) $(defs) -std=c++0x
ColorManager.o: ColorManager.cpp ColorManager.h
	g++ -c ColorManager.cpp -std=c++0x $(defs)

LedManager.o: LedManager.cpp LedManager.h
	g++ -c LedManager.cpp -std=c++0x $(defs)

SoundManager.o: SoundManager.cpp SoundManager.h
	g++ -c SoundManager.cpp -std=c++0x $(defs)

GraphicsManager.o: GraphicsManager.cpp GraphicsManager.h
	g++ -c GraphicsManager.cpp -std=c++0x $(defs)

clean:
	@rm -f 	ag *.o
