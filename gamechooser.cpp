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
#include "SDL_ttf/SDL_ttf.h"

GameChooser::GameChooser() {
	
}

GameChooser::~GameChooser() {
	TTF_CloseFont(font);
}

void GameChooser::InitFont(char* filename, int size) {
	//Open the font
	font = TTF_OpenFont(filename,size);
	if (!font) {
		printf("TTF_OpenFont: %s ",TTF_GetError());
	}
}

void GameChooser::Display(SDL_Surface * display) {
	//Generate the image for the current game
	SDL_Surface* current_image = current.GetImage();
	//Blit the image to the screen
	SDL_Rect destination;
	//try to center the image on the display
	destination.x = (display->w)/2-(current_image->w)/2;
	destination.y = (display->h)/2-(current_image->h)/2;
	SDL_BlitSurface(current_image, NULL, display, &destination);	
	SDL_FreeSurface(current_image);
	//Display the Rom name to the screen
	SDL_Color color={100,100,100};	//White
	SDL_Surface* text_surface;
	//Check if the font loaded properly
	if (font == NULL) {
		cout<<"Failed to load font!"<<endl;
	}
	else {
		if (!(text_surface = TTF_RenderText_Solid(font, current.GetName().c_str(), color))) {
			//Handle Error
			cout<<"Couldn't create text_surface!"<<endl;
		}
		else {
			destination.x = 0;
			destination.y = 0;
			SDL_BlitSurface(text_surface, NULL, display, &destination);
			SDL_FreeSurface(text_surface);
		}
	}
}

void GameChooser::Update(list<string> games) {
	//Create an iterator to navigate the list
	list<string>::iterator pos = games.begin();
	
	previous.GenerateImage(*pos);
	++pos;
	current.GenerateImage(*pos);
	++pos;
	next.GenerateImage(*pos);
}