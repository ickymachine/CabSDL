/*
 *  cabdisplay.cpp
 *  CabSDL
 *
 *  Created by ickymachine on 11/21/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "cabdisplay.h"
#include "gameimage.h"
#include "constants.h"
#include <iostream>
#include <math.h>

//Initialize static members
int CabDisplay::dialog_height = 0;
int CabDisplay::dialog_width = 0;
int CabDisplay::image_height = 0;
int CabDisplay::image_width = 0;
SDL_Color CabDisplay::background_color = {0,0,0,0};
SDL_Color CabDisplay::normal_text_color = {255,255,255,0};
SDL_Color CabDisplay::highlighted_text_color = {255,255,0,0};

//x,y position is at the top left corner of the text
int CabDisplay::DisplayText(const string& text, TTF_Font* font, SDL_Surface* display, int x, int y, int highlight) {
	SDL_Rect destination;
	//Display the Rom name to the screen
	SDL_Surface* text_surface;
	//Set the correct color
	SDL_Color color = (highlight ? CabDisplay::highlighted_text_color:CabDisplay::normal_text_color);
	//Check if the font loaded properly
	if (font == NULL) {
		std::cerr<<"Failed to load font!"<<std::endl;
		return Error::FAILED_TO_LOAD_FONT;
	}
	else {
		//Generate the game name from the GameImage path string
		if (!(text_surface = TTF_RenderText_Solid(font, text.c_str(), color))) {
			//Handle Error
			std::cerr<<"Couldn't create text_surface!"<<std::endl;
			return Error::FAILED_TO_CREATE_SURFACE;
		}
		else {
			destination.x = x;
			destination.y = y;
			destination.w = text_surface->w;
			destination.h = text_surface->h;
			SDL_BlitSurface(text_surface, NULL, display, &destination);
			SDL_FreeSurface(text_surface);
		}
	}
	return Error::NO_ERROR;
}

int CabDisplay::UpdateDisplay(SDL_Surface* display) {
	//Update the screen
	SDL_Flip(display);
	return Error::NO_ERROR;
}

int CabDisplay::DisplayImage(SDL_Surface* image, SDL_Surface* display, int x, int y) {
	SDL_Rect destination;
	//Blit the current game image to the screen
	//try to center the image on the display
	destination.x = x;
	destination.y = y;
	image = GameImage::ScaleImage(image, image_width, image_height);
	SDL_BlitSurface(image, NULL, display, &destination);	
	SDL_FreeSurface(image);
	return Error::NO_ERROR;
}

int CabDisplay::BlankDisplay(SDL_Surface* display) {
	//Clear the display
	SDL_FillRect(display, NULL, SDL_MapRGB(display->format,
										   CabDisplay::background_color.r,
										   CabDisplay::background_color.g,
										   CabDisplay::background_color.b));
	return Error::NO_ERROR;
}


int CabDisplay::DisplayList(const list<string>& games, const int selected, 
							TTF_Font* font, SDL_Surface* display, int x, int y) {
	SDL_Rect destination;
	
	//Display the list of game names to the screen
	SDL_Surface* text_surface;
	//Check if the font loaded properly
	if (font == NULL) {
		std::cerr<<"CabDisplay::DisplayList; Failed to load font!"<<std::endl;
		return Error::FAILED_TO_LOAD_FONT;
	}
	else {
		list<string>::const_iterator games_pos = games.begin();
		//Set top of list position
		destination.x = x;
		destination.y = y;
		//Setup font colors
		SDL_Color d_color;
		int displaypos = 0;
		while (games_pos != games.end()) {
			if (displaypos == selected) {
				d_color = CabDisplay::highlighted_text_color;
			}
			else {
				d_color = CabDisplay::normal_text_color;
			}
			//Generate the game name from the GameImage path string
			if (!(text_surface = TTF_RenderText_Solid(font, games_pos->c_str(), d_color))) {
				//Handle Error
				std::cerr<<"CabDisplay::DisplayList; Couldn't create text_surface!"<<std::endl;
				return Error::FAILED_TO_CREATE_SURFACE;
			}
			else {
				destination.x = x-text_surface->w;
				destination.w = text_surface->w;
				destination.h = text_surface->h;
				//Blit the new list
				SDL_BlitSurface(text_surface, NULL, display, &destination);
				SDL_FreeSurface(text_surface);
			}
			destination.y += TTF_FontLineSkip(font);
			games_pos++;
			displaypos++;
		}
	}
	return Error::NO_ERROR;	
}

int CabDisplay::DisplayCategoryBox(const list<string>& categories, 
								   const int selected, TTF_Font* font, SDL_Surface* display, int x, int y) {
	int xpos = x-CabDisplay::dialog_width/2;
	int ypos = y-CabDisplay::dialog_height/2;
	
	//Create a black box for the dialog
	SDL_Rect destination;
	destination.x = xpos-5;
	destination.y = ypos-5;
	destination.w = CabDisplay::dialog_width;
	destination.h = CabDisplay::dialog_height;
	
	//Create the dialog box
	SDL_FillRect(display, &destination, SDL_MapRGB(display->format,
												   CabDisplay::normal_text_color.r,
												   CabDisplay::normal_text_color.g,
												   CabDisplay::normal_text_color.b) );
	destination.x = xpos;
	destination.y = ypos;
	destination.w = CabDisplay::dialog_width-10;
	destination.h = CabDisplay::dialog_height-10;
	SDL_FillRect(display, &destination, 0x000000);
	
	//Display the list of categories to the screen
	//Setup the positioning of the list
	xpos = x+CabDisplay::dialog_width/2-15;
	ypos = y-CabDisplay::dialog_height/2+5;
	if (CabDisplay::DisplayList(categories,selected,font,display,xpos,ypos) == Error::FAILED_TO_CREATE_SURFACE) {
		//Error in displaying the list
		return Error::FAILED_TO_CREATE_SURFACE;
	}
	//Executed without error
	return Error::NO_ERROR;
}

int CabDisplay::DisplayAlertDialog(const string& alert, TTF_Font* font, SDL_Surface* display, int x, int y) {
	//Determine the proper size of the dialog
	int fontheight;
	int fontwidth;
	
	if (TTF_SizeText(font, alert.c_str(), &fontwidth, &fontheight) != 0) {
		//Error
		return Error::FAILED_TO_CREATE_ALERT_DIALOG;
	}
	
	//Add border around the text
	fontheight += 20;
	fontwidth += 20;
	
	int xpos = x-fontwidth/2;
	int ypos = y-fontheight/2;
	
	//Create a black box for the dialog
	SDL_Rect destination;
	destination.x = xpos-5;
	destination.y = ypos-5;
	destination.w = fontwidth;
	destination.h = fontheight;
	
	//Create the dialog box
	SDL_FillRect(display, &destination, SDL_MapRGB(display->format,
												   CabDisplay::normal_text_color.r,
												   CabDisplay::normal_text_color.g,
												   CabDisplay::normal_text_color.b) );
	destination.x = xpos;
	destination.y = ypos;
	destination.w = fontwidth-10;
	destination.h = fontheight-10;
	SDL_FillRect(display, &destination, SDL_MapRGB(display->format,
												   CabDisplay::background_color.r,
												   CabDisplay::background_color.g,
												   CabDisplay::background_color.b) );
	
	//Display the text to the screen
	//Setup the positioning of the text
	xpos += 5;
	ypos += 5;
	if (CabDisplay::DisplayText(alert, font, display, xpos, ypos, true) != Error::NO_ERROR) {
		//Error in displaying the text to the surface
		return Error::FAILED_TO_CREATE_ALERT_DIALOG;
	}
	
	CabDisplay::UpdateDisplay(display);
	
	//Wait for input from the user to dismiss dialog
	int done = false;
	SDL_Event event;
	while (!done) {
		if (SDL_WaitEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
				case SDL_JOYBUTTONDOWN:
					done = true;
					break;
				default:
					//DO NOTHING
					break;
			}
		}
	}
	
	//Executed without error
	return Error::NO_ERROR;
}

int CabDisplay::DetermineDialogSize(const list<string>& text, TTF_Font* font) {
	//Setup the size of the dialog box
	CabDisplay::dialog_height = 20+10*TTF_FontLineSkip(font);
	//Poll all the text to determine the proper width of the dialog box
	int fontheight;
	int fontwidth;
	for (list<string>::const_iterator it = text.begin(); it != text.end(); it++) {
		if (TTF_SizeText(font, it->c_str(), &fontwidth, &fontheight) == 0) {
			if (fontwidth >= CabDisplay::dialog_width) {
				CabDisplay::dialog_width = fontwidth+20;
			} 
		}
	}
	//Executed without error
	return Error::NO_ERROR;
}

int CabDisplay::SetImageSize(int width, int height) {
	CabDisplay::image_width = width;
	CabDisplay::image_height = height;
	return Error::NO_ERROR;
}
