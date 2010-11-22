/*
 *  cabdisplay.h
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <string>
#include <list>

#ifndef CABDISPLAY_H
#define CABDISPLAY_H

using namespace std;

class CabDisplay {
public:
	static int DisplayImage(SDL_Surface* image, SDL_Surface* display);
	static int DisplayText(string text, TTF_Font* font, SDL_Surface* display, int x, int y);
	static int BlankDisplay(SDL_Surface* display);
	static int UpdateDisplay(SDL_Surface* display);
	static int DisplayList(list<string> games, int selected, TTF_Font* font, SDL_Surface* display, int x, int y);
};

#endif