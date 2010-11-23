# Build Instructions:

## Ubuntu

g++ -Wall -I /usr/include/SDL *.cpp ./configfile/configfile.cpp -lSDL_image -lSDL_ttf -lSDL -o cabsdl

## OS X

g++ -framework SDL -framework SDL_ttf -framework SDL_image -framework Cocoa -I /opt/local/include/SDL *.cpp configfile/configfile.cpp "/Library/Application Support/Developer/Shared/Xcode/Project Templates/SDL Application/SDLMain.m" -o cabsdl

## Windows

Yeah, right.