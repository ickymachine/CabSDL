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

//x,y position is at the top left corner of the text
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
			cout<<"CabDisplay::DisplayText; Couldn't create text_surface!"<<endl;
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

int CabDisplay::DisplayImage(SDL_Surface* image, SDL_Surface* display, int x, int y) {
	SDL_Rect destination;
	//Blit the current game image to the screen
	//try to center the image on the display
	destination.x = x;
	destination.y = y;
	SDL_BlitSurface(image, NULL, display, &destination);	
	SDL_FreeSurface(image);
	return 0;
}

int CabDisplay::BlankDisplay(SDL_Surface* display) {
	//Clear the display
	SDL_FillRect(display, NULL, 0x000000);
	return 0;
}


int CabDisplay::DisplayList(list<string> games, int selected, TTF_Font* font, SDL_Surface* display, int x, int y) {
	SDL_Rect destination;
	
	//Display the list of game names to the screen
	SDL_Surface* text_surface;
	//Check if the font loaded properly
	if (font == NULL) {
		cout<<"CabDisplay::DisplayList; Failed to load font!"<<endl;
		return -1;
	}
	else {
		list<string>::iterator games_pos = games.begin();
		//Set top of list position
		destination.x = x;
		destination.y = y;
		//Setup font colors
		SDL_Color n_color={255,255,255};
		SDL_Color s_color={255,255,0};
		SDL_Color* d_color;
		int displaypos = 1;
		while (games_pos != games.end()) {
			if (displaypos == selected) {
				d_color = &s_color;
			}
			else {
				d_color = &n_color;
			}
			//Generate the game name from the GameImage path string
			if (!(text_surface = TTF_RenderText_Solid(font, games_pos->c_str(), *d_color))) {
				//Handle Error
				cout<<"CabDisplay::DisplayList; Couldn't create text_surface!"<<endl;
				return -1;
			}
			else {
				destination.x = x-text_surface->w;
				SDL_BlitSurface(text_surface, NULL, display, &destination);
				SDL_FreeSurface(text_surface);
			}
			destination.y += TTF_FontLineSkip(font);
			games_pos++;
			displaypos++;
		}
	}
	return 0;	
}

int CabDisplay::DisplayCategoryBox(list<string> categories, int selected, TTF_Font* font, SDL_Surface* display, int x, int y) {
	//Setup the size of the dialog box
	int height = 20+10*TTF_FontLineSkip(font);
	//Poll all the categories to determine the proper width of the dialog box
	int fontheight;
	int fontwidth;
	int width = 0;
	for (list<string>::iterator it = categories.begin(); it != categories.end(); it++) {
		if (TTF_SizeText(font, it->c_str(), &fontwidth, &fontheight) == 0) {
			if (fontwidth > width) width = fontwidth+20;
		}
	}
	int xpos = x-width/2;
	int ypos = y-height/2;
	
	//Create a black box for the dialog
	SDL_Rect destination;
	destination.x = xpos;
	destination.y = ypos;
	destination.w = width;
	destination.h = height;
	
	//Create the dialog box
	SDL_FillRect(display, &destination, 0x000000);
	
	//Display the list of categories to the screen
	//Only display 10 items from the list
	list<string> catdisplay;
	list<string>::iterator it = categories.begin();
	advance(it,selected-1);
	for (int i = 0; i < 10; i++) {
		if (it != categories.end()) {
			catdisplay.push_back(*it);
			it++;
		}
	}
	
	//Setup the positioning of the list
	xpos = x+width/2-10;
	ypos = y-height/2+10;
	if (CabDisplay::DisplayList(catdisplay,1,font,display,xpos,ypos) == -1) {
		//Error in displaying the list
		return -1;
	}
	return 0;
}
