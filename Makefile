
all:
	g++ -g main.cpp keywatcher.cpp morse.cpp morseplayer.cpp -lX11 -lportaudio -lpthread -o morse_keypress
