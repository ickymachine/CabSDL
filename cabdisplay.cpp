/*
 *  cabdisplay.cpp
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/21/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "cabdisplay.h"
#include <iostream>

int CabDisplay::DisplayText(string text, TTF_Font* font, SDL_Surface* display, int x, int y) {
	SDL_Rect destination;
	//Display the Rom name to the screen
	SDL_Color color={255,255,255};	//White
	SDL_Surface* text_surface;
	//Check if the font loaded properly
	if (font == NULL) {
		cout<<"Failed to load font!"<<endl;
		return -1;
	}
	else {
		//Generate the game name from the GameImage path string
		if (!(text_surface = TTF_RenderText_Solid(font, text.c_str(), color))) {
			//Handle Error
			cout<<"Couldn't create text_surface!"<<endl;
			return -1;
		}
		else {
			destination.x = x;
			destination.y = y;
			SDL_BlitSurface(text_surface, NULL, display, &destination);
			SDL_FreeSurface(text_surface);
		}
	}
	return 0;
}

int CabDisplay::UpdateDisplay(SDL_Surface* display) {
	//Update the screen
	SDL_Flip(display);
	return 0;
}

int CabDisplay::DisplayImage(SDL_Surface* image, SDL_Surface* display) {
	SDL_Rect destination;
	//Blit the current game image to the screen
	//try to center the image on the display
	destination.x = 10;
	destination.y = 10;
	SDL_BlitSurface(image, NULL, display, &destination);	
	SDL_FreeSurface(image);
	return 0;
}

int CabDisplay::BlankDisplay(SDL_Surface* display) {
	//Clear the display
	SDL_FillRect(display, NULL, 0x000000);
	return 0;
}
