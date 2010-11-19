/*
 *  gamechooser.cpp
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "gamechooser.h"
#include <iostream>
#include <SDL_ttf/SDL_ttf.h>
#include <dirent.h>

GameChooser::GameChooser() {
	
}

GameChooser::~GameChooser() {

}

void GameChooser::InitFont(char* filename, int size) {
	font = TTF_OpenFont(filename,size);
	if (!font) {
		printf("TTF_OpenFont: %s ",TTF_GetError());
	}
	else {
		printf("Font loaded succesfully.\n");
	}
}

void GameChooser::Display(SDL_Surface * display) {
	//Scale the image down
	game.ScaleImage(display->w-(display->w/5), display->h-(display->h/5));
	
	//Create a SDL_Surface* to hold the images
	SDL_Surface* image;
	SDL_Rect destination;
	
	//Generate the image for the current game
	image = game.GetImage();
	//Blit the current game image to the screen
	//try to center the image on the display
	destination.x = 10;
	destination.y = 10;
	SDL_BlitSurface(image, NULL, display, &destination);	
	SDL_FreeSurface(image);
		
	//Display the Rom name to the screen
	SDL_Color color={255,255,255};	//White
	SDL_Surface* text_surface;
	//Check if the font loaded properly
	if (font == NULL) {
		cout<<"Failed to load font!"<<endl;
	}
	else {
		if (!(text_surface = TTF_RenderText_Solid(font, game.GetName().c_str(), color))) {
			//Handle Error
			cout<<"Couldn't create text_surface!"<<endl;
		}
		else {
			destination.x = 10;
			destination.y = display->h-20;
			SDL_BlitSurface(text_surface, NULL, display, &destination);
			SDL_FreeSurface(text_surface);
		}
	}
}

void GameChooser::Update(string gamename) {
	game.GenerateImage(gamename);
}

void GameChooser::CloseFont() {
	TTF_CloseFont(font);
}