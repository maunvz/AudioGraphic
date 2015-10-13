# build an executable called ag from main.cpp

main: main.cpp ColorManager.o SoundManager.o LedManager.o
	g++ -g -Wall -o ag main.cpp ColorManager.o SoundManager.o LedManager.o -lpthread -lfftw3 -lm libs/libportaudio.a -lrt -lasound -lwiringPi -std=c++0x
  #-lrt -lasound -lwiringPi //RASPBERRY PI
	#-framework CoreAudio -framework AudioUnit -framework Carbon -framework AudioToolbox -framework CoreServices //MAC
ColorManager.o: ColorManager.cpp ColorManager.h
	g++ -c ColorManager.cpp -std=c++0x

LedManager.o: LedManager.cpp LedManager.h
	g++ -c LedManager.cpp -std=c++0x

SoundManager.o: SoundManager.cpp SoundManager.h
	g++ -c SoundManager.cpp -std=c++0x

clean:
	@rm -f 	ag *.o
