# build an executable called ag from main.cpp
libs_for_mac = -framework CoreAudio -framework AudioUnit -framework Carbon -framework AudioToolbox -framework CoreServices libs/mac/libportaudio.a
libs_for_unix = -lrt -lasound libs/unix/libportaudio.a
libs_for_pi = -lrt -lasound -lwiringPi libs/rpi/libportaudio.a

main: main.cpp ColorManager.o SoundManager.o LedManager.o
	g++ -g -Wall -o ag main.cpp ColorManager.o SoundManager.o LedManager.o -lpthread -lfftw3 -lm -std=c++0x $(libs_for_mac)
ColorManager.o: ColorManager.cpp ColorManager.h
	g++ -c ColorManager.cpp -std=c++0x

LedManager.o: LedManager.cpp LedManager.h
	g++ -c LedManager.cpp -std=c++0x

SoundManager.o: SoundManager.cpp SoundManager.h
	g++ -c SoundManager.cpp -std=c++0x

clean:
	@rm -f 	ag *.o
