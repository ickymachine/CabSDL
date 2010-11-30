/*
 *  gameimage.h
 *  CabGL
 *
 *  Created by Matthew Capodarco on 11/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
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
	static SDL_Surface* GenerateImage(string game_name);
	static SDL_Surface* ScaleImage(SDL_Surface* image, int width, int height);
	static void GenerateImageGL(string game_name);
private:
//	SDL_Surface* LoadImageFile(char * filename);
//	string FindImageFile(string game);
//	SDL_Surface* image;
//	string name;
	static SDL_Color GetPixel(SDL_Surface* image, int x, int y);
	static void DownloadImage(string file_name);
	static size_t WriteData(void *ptr, size_t size, size_t nmemb, FILE *stream);
};

#endif