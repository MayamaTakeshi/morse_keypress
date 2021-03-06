
all:
	g++ -g main.cpp keywatcher.cpp morse.cpp audioplayer.cpp -lX11 -lportaudio -lpthread -o morse_keypress
