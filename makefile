# Makefile for CabSDL
CC=g++
CFLAGS=-c -Wall -I /usr/include/curl
LDFLAGS= /usr/lib/libcurl.so.4.2.0 
SDL_CFLAGS= $(shell sdl-config --cflags)
SDL_LDFLAGS= $(shell sdl-config --libs) -lSDL_ttf -lSDL_image
SOURCES= configfile/configfile.cpp cabdisplay.cpp cabui.cpp category.cpp constants.h description.cpp event.cpp constants.h gameimage.cpp gamelist.cpp locations.cpp main.cpp mamedb.cpp resize++.cpp wlist.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cabsdl

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(SDL_LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $< -o $@

clean:
	rm cabsdl $(OBJECTS)
