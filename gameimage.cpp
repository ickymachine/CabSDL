/*
 *  gameimage.cpp
 *  CabGL
 *
 *  Created by ickymachine on 11/16/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "gameimage.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "SDL_image.h"
#include "SDL.h"
#include <string>
#include <math.h>
#include <fstream>
#include "resize++.h"
#include "mamedb.h"

#ifdef __APPLE__
#include "mac.h"
#endif

using namespace std;
 
SDL_Surface* GameImage::GenerateImage(const string& game_name) {
	//Create an SDL_Surface* to return
	SDL_Surface* image = NULL;
	//Locate the proper image
	string filename = game_name+".png";

	// See if the image exists
	ifstream game_image(filename.c_str());
	if ( ! game_image.good()) {
		MameDB::DownloadImage(game_name);
	}
	game_image.close();

	//Load the desired image
	image = IMG_Load((char*)filename.c_str());
	//check if the image loading failed
	if (image == NULL) {
		//Load and display default image instead
		std::string filepath = "default.png";
		cout<<"ERROR; GameImage::GenerateImage; Couldn't load image at "<<filename<<endl;
		
#ifdef __APPLE__
		filepath = App::ResourcesPath()+"/default.png";
#endif
		image = IMG_Load(filepath.c_str());
		if (image == NULL) {
			//couldn't load default for some reason
			cout<<"ERROR; GameImage::Generate; Couldn't load default.png"<<endl;
		}
	}
	return image;
}

SDL_Surface* GameImage::ScaleImage(SDL_Surface* image, int width, int height) {
	//set the image to the scaled result
	return SDL_Resize(image, width, height);;
}

SDL_Color GameImage::GetPixel(SDL_Surface* image, int x, int y) {
	//Lock the image before inspecting the pixels pointer
	SDL_LockSurface(image);
	SDL_Color color;
	Uint32 col = 0;
	char* position = (char*)image->pixels;
	//Move to the proper x,y coordinate
	position += (image->pitch * y);
	position += (image->format->BytesPerPixel * x);
	//Get the pixel data
	//copy pixel data
	memcpy (&col, position, image->format->BytesPerPixel);
	//Convert color
	SDL_GetRGB(col, image->format, &color.r, &color.g, &color.b);
	//Unlock the image
	SDL_UnlockSurface(image);
	return color;
}
