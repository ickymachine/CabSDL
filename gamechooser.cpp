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
<<<<<<< HEAD
#include "dirent.h"
=======
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2

GameChooser::GameChooser() {
	
}

GameChooser::~GameChooser() {
<<<<<<< HEAD

}

void GameChooser::InitFont(char* filename, int size) {
=======
	TTF_CloseFont(font);
}

void GameChooser::InitFont(char* filename, int size) {
	//Open the font
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
	font = TTF_OpenFont(filename,size);
	if (!font) {
		printf("TTF_OpenFont: %s ",TTF_GetError());
	}
<<<<<<< HEAD
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
=======
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
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
	SDL_Surface* text_surface;
	//Check if the font loaded properly
	if (font == NULL) {
		cout<<"Failed to load font!"<<endl;
	}
	else {
<<<<<<< HEAD
		if (!(text_surface = TTF_RenderText_Solid(font, game.GetName().c_str(), color))) {
=======
		if (!(text_surface = TTF_RenderText_Solid(font, current.GetName().c_str(), color))) {
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
			//Handle Error
			cout<<"Couldn't create text_surface!"<<endl;
		}
		else {
<<<<<<< HEAD
			destination.x = 10;
			destination.y = display->h-20;
=======
			destination.x = 0;
			destination.y = 0;
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
			SDL_BlitSurface(text_surface, NULL, display, &destination);
			SDL_FreeSurface(text_surface);
		}
	}
}

<<<<<<< HEAD
void GameChooser::Update(string gamename) {
	game.GenerateImage(gamename);
}

void GameChooser::CloseFont() {
	TTF_CloseFont(font);
=======
void GameChooser::Update(list<string> games) {
	//Create an iterator to navigate the list
	list<string>::iterator pos = games.begin();
	
	previous.GenerateImage(*pos);
	++pos;
	current.GenerateImage(*pos);
	++pos;
	next.GenerateImage(*pos);
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
}