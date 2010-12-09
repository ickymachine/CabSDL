// $Id$
/**
 *  @file gameimage.h
 *  Provides functions to load a png file and generate an SDL_Surface
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Functions to load a png image file, generate a SDL_Surface.  Uses resize++, by David Olsen, to scale the image. 

#ifndef GAMEIMAGE_H
#define GAMEIMAGE_H

#include <stdio.h>
#include "SDL.h"
#include <string>

using namespace std;

class GameImage {
public:
	/**
	 Load a png file, game_name.png, and generate a SDL_Surface
	 
	 @param[in]	game_name String object containing the name of the game to open
	 @return SDL_Surface* of the loaded image file.
	 */
	static SDL_Surface* GenerateImage(const string& game_name);
	/**
	 Scale a SDL_Surface to size widthxheight
	 
	 @param[in,out]	*image	SDL_Surface to be scaled
	 @param[in]		width	The new width of the image
	 @param[in]		height	The new height of the image
	 @return SDL_Surface* to the scaled image
	 */
	static SDL_Surface* ScaleImage(SDL_Surface* image, int width, int height);
};

#endif