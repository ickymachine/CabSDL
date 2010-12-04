/*
 *  gameimage.h
 *  CabGL
 *
 *  Created by ickymachine on 11/16/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef GAMEIMAGE_H
#define GAMEIMAGE_H

#include <stdio.h>
#include "SDL.h"
#include <string>

using namespace std;

class GameImage {
public:
	static SDL_Surface* GenerateImage(const string& game_name);
	static SDL_Surface* ScaleImage(SDL_Surface* image, int width, int height);
private:
	static SDL_Color GetPixel(SDL_Surface* image, int x, int y);
};

#endif