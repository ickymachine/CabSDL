/*
 *  gameimage.h
 *  CabGL
 *
 *  Created by Matthew Capodarco on 11/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <SDL/SDL.h>
#include <string>

using namespace std;

#ifndef GAMEIMAGE
#define GAMEIMAGE

class GameImage {
public:
	GameImage();
	~GameImage();
	void GenerateImage(string game_name);
	SDL_Surface* GetImage();
	string GetName();
	void ScaleImage(uint width, uint height);
private:
	SDL_Surface* LoadImageFile(char * filename);
	string FindImageFile(string game);
	SDL_Surface* image;
	string name;
	SDL_Color GetPixel(int x, int y);
};

#endif