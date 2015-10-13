# build an executable called ag from main.cpp
libs_for_mac = -framework CoreAudio -framework AudioUnit -framework Carbon -framework AudioToolbox -framework CoreServices libs/mac/libportaudio.a
libs_for_unix = -lrt -pthread -lasound -ljack libs/unix/libportaudio.so
libs_for_pi = -lrt -lasound -lwiringPi libs/rpi/libportaudio.a -DUSE_LEDS
libs = $(libs_for_mac)
main: main.cpp ColorManager.o SoundManager.o LedManager.o
	g++ -g -Wall -o ag main.cpp $(libs) ColorManager.o SoundManager.o LedManager.o -lpthread -lfftw3 -lm -std=c++0x
ColorManager.o: ColorManager.cpp ColorManager.h
	g++ -c ColorManager.cpp -std=c++0x $(libs)

LedManager.o: LedManager.cpp LedManager.h
	g++ -c LedManager.cpp -std=c++0x $(libs)

SoundManager.o: SoundManager.cpp SoundManager.h
	g++ -c SoundManager.cpp -std=c++0x $(libs)

clean:
	@rm -f 	ag *.o
