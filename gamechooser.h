/*
 *  gamechooser.h
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SDL.h"
#include "SDL_ttf/SDL_ttf.h"
#include <list>

#include "gameimage.h"

class GameChooser {
public:
	GameChooser(); //default constructor
	~GameChooser(); //default destructor
	void Display(SDL_Surface * display);
	void Update(list<string> games);
	void InitFont(char* filename, int size);
private:
	GameImage current;
	GameImage next;
	GameImage previous;
	TTF_Font* font;
};