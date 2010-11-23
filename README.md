# CabSDL

CabSDL is a MAME frontend that is ideal for fullscreen applications, like your
full-size cabinet or htpc application. It features an optional search, and will
soon support easy filtering when a keyboard isn't available, like in a cabinet.

# Features

 * Automatic ROM detection
 * Automatic ROM title thumbnail detection
 * Automatic ROM title thumbnail download for missing titles
 * Search functionality

# Build Instructions:

## Ubuntu

g++ -Wall -I /usr/include/SDL *.cpp ./configfile/configfile.cpp <path-to-libcurl> -lSDL_image -lSDL_ttf -lSDL  -o cabsdl

## OS X

g++ -framework SDL -framework SDL_ttf -framework SDL_image -framework Cocoa -I /opt/local/include/SDL *.cpp configfile/configfile.cpp "/Library/Application Support/Developer/Shared/Xcode/Project Templates/SDL Application/SDLMain.m" <path-to-libcurl> -o cabsdl

## Windows

Yeah, right.
