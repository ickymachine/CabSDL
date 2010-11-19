/*
 *  gameimage.cpp
 *  CabGL
 *
 *  Created by Matthew Capodarco on 11/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
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

using namespace std;

GameImage::GameImage() {
	
}

GameImage::~GameImage() {
	
}

SDL_Surface* GameImage::LoadImageFile(char * filename){
	SDL_Surface * sur = IMG_Load(filename);
	if (sur == NULL) {
		cout<<"Couldn't load image from file: "<<filename<<endl;
	}
	return sur;
}

void GameImage::GenerateImage(string game_name) {
	//Assign the image name
	name = game_name;
	//Locate the proper image
	string filename = FindImageFile(name);
	//Load the desired image
	image = LoadImageFile((char*)filename.c_str());
	//check if the image loading failed
	if (image == NULL) {
		//Load and display default image instead
		cout<<"ERROR; GameImage::GenerateImage; Couldn't load image at "<<filename<<endl;
		image = LoadImageFile("default.png");
		if (image == NULL) {
			//couldn't load default for some reason
			cout<<"ERROR; GameImage::Generate; Couldn't load default.png"<<endl;
			return;
		}
	}
}

string GameImage::FindImageFile(string game) {
	//Remove the file extension
	string name = game.substr(0,game.find("."));
	//add the location path to the name and append .png
	return name+".png";
}

SDL_Surface* GameImage::GetImage() {
	return image;
}

string GameImage::GetName() {
	return name;
}

void GameImage::ScaleImage(uint width, uint height) {
	if (!width || !height || image->w == 0 || image->h == 0) {
		cout<<"ERROR; GameImage::ScaleImage; Size must be greater than 0"<<endl;
		return;
	}
	//Create the scaled image using the format of the original
	SDL_Surface* scaled = SDL_CreateRGBSurface(image->flags, width, height, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
	double stretchx = (static_cast<double>(width)/static_cast<double>(image->w));
	double stretchy = (static_cast<double>(height)/static_cast<double>(image->h));
	SDL_Rect dest;
	SDL_Color pix;
	double whole;
	
	//Scale down both dimensions
	if (stretchx < 1 && stretchy < 1) {
		cout<<"Scaling down/down image "<<image->w<<" "<<image->h<<" by "<<stretchy<<","<<stretchx<<endl;
		int targetx = 0;
		int targety = 0;
		dest.w = 1;
		dest.h = 1;
		//Determine scaling factors
		int xint = static_cast<double>(image->w)/static_cast<double>(width);
		int yint = static_cast<double>(image->h)/static_cast<double>(height);
		//Loop through the scaled image dimensions
		for (int y = 0; y < height; y++) {
			dest.y = y;
			for (int x = 0; x < width; x++) {
				dest.x = x;
				pix = GetPixel(targetx,targety);
				SDL_FillRect(scaled, &dest, SDL_MapRGB(image->format, pix.r, pix.g, pix.b));
				targetx += (xint+(modf(static_cast<double>(x)*static_cast<double>(image->w)/static_cast<double>(width), &whole) > 0 ? 0:1));
				//check to see if out of bounds
				if (targetx >= image->w) {
					targetx = image->w-1;
				}
			}
			targetx = 0;
			targety += (yint+(modf(static_cast<double>(y)*static_cast<double>(image->h)/static_cast<double>(height), &whole) > 0 ? 0:1));
			//check to see if out of bounds
			if (targety >= image->h) {
				targety = image->h-1;
			}
		}
	}
	//Scale down x, scale up y
	else if (stretchx < 1) {
		cout<<"Scaling down/up image "<<image->w<<" "<<image->h<<" by "<<stretchy<<","<<stretchx<<endl;
		int targetx = 0;
		int xint = static_cast<double>(image->w)/static_cast<double>(width);
		dest.w = 1;
		for (int y = 0; y < image->h; y++) {
			dest.y = y*stretchy;
			dest.h = stretchy+(modf(static_cast<double>(y)*static_cast<double>(stretchy), &whole) > 0 ? 1:0);
			for (int x = 0; x < width; x++) {
				dest.x = x;
				pix = GetPixel(targetx,y);
				SDL_FillRect(scaled, &dest, SDL_MapRGB(image->format, pix.r, pix.g, pix.b));
				targetx += (xint+(modf(static_cast<double>(x)*static_cast<double>(image->w)/static_cast<double>(width), &whole) > 0 ? 0:1));
				if (targetx >= image->w) {
					targetx = image->w-1;	
				}
			}
			targetx = 0;		
		}
	}
	//Scaule up x, scale down y
	else if (stretchy < 1) {
		cout<<"Scaling up/down image "<<image->w<<" "<<image->h<<" by "<<stretchy<<","<<stretchx<<endl;
		int targety = 0;
		int yint = static_cast<double>(image->h)/static_cast<double>(height);
		dest.h = 1;
		for (int y = 0; y < height; y++) {
			dest.y = y;
			for (int x = 0; x < image->w; x++) {
				dest.x = x*stretchx;
				dest.w = stretchx+(modf(static_cast<double>(x)*static_cast<double>(stretchx), &whole) > 0 ? 1:0);
				pix = GetPixel(x,targety);
				SDL_FillRect(scaled, &dest, SDL_MapRGB(image->format, pix.r, pix.g, pix.b));
			}
			targety += (yint+(modf(static_cast<double>(y)*static_cast<double>(image->h)/static_cast<double>(height), &whole) > 0 ? 0:1));
			//check to see if out of bounds
			if (targety >= image->h) {
				targety = image->h-1;
			}
		}
	}
	//Scale up both dimensions
	else {
		cout<<"Scaling up/up image "<<image->w<<" "<<image->h<<" by "<<stretchy<<","<<stretchx<<endl;
		//Loop across the original image, drawing the scaled pixels
		for (int y = 0; y < image->h; y++) {
			dest.y = (y*stretchy);
			dest.h = stretchy+(modf(static_cast<double>(y)*static_cast<double>(stretchy), &whole) > 0 ? 1:0);
			for (int x = 0; x < image->w; x++) {
				dest.x = (x*stretchx);
				dest.w = stretchx+(modf(static_cast<double>(x)*static_cast<double>(stretchx), &whole) > 0 ? 1:0);
				pix = GetPixel(x,y);
				SDL_FillRect(scaled, &dest, SDL_MapRGB(image->format, pix.r, pix.g, pix.b));
				//cout<<dest.x<<" "<<dest.y<<endl;
			}
		}
	}
	//set the image to the scaled result
	image = scaled;
}

SDL_Color GameImage::GetPixel(int x, int y) {
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
//	SDL_GetRGB(*position, image->format, &color.r, &color.g, &color.b);
	//Unlock the image
	SDL_UnlockSurface(image);	
	return color;
}
