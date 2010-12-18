/*
 // $Id$
 /**
 *  @file cabdisplay.h
 *  Provides static member functions to generate SDL_Surfaces and blit them to the screen.
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Group of static member function to generate the graphical content displayed on the main SDL_Surface.

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include <string>
#include <list>

#ifndef CABDISPLAY_H
#define CABDISPLAY_H

using namespace std;

class CabDisplay {
public:
	/**
		Display an image file to the display with top/left corner at position x,y
	 
		@param[in,out]	*image SDL_Surface of a loaded png image file.
		@param[in,out]	*display SDL_Surface to display the image on.
		@param[in]	x Integer position to display the image
		@param[in]	y Integer position to display the image
		@return Error code: 0 No error
	 */
	static int DisplayImage(SDL_Surface* image, SDL_Surface* display, int x, int y);
	
	/**
	 Display text to the screen with desired font with top/left corner at position x,y
	 
	 @param[in]	text String value to be displayed
	 @param[in,out]	*font The ttf font used to render the text
	 @param[in,out]	*display SDL_Surface to render the text on
	 @param[in]	x Integer position to display the text
	 @param[in] y Integer position to display the text
	 @param[in] highlight boolean to set whether the text should be highlight color
	 @return Error code: 0 No error
	 */
	static int DisplayText(const string& text, TTF_Font* font, SDL_Surface* display, int x, int yint, int highlight = false);
	
	/**
	 Fill the screen using a black rectangle.
	 
	 @param[in,out]	*display SDL_Surface to blank.
	 @return Error code: 0 No error
	 */
	
	static int BlankDisplay(SDL_Surface* display);
	/**
	 Flip the screen buffer
	 
	 @param[in,out]	*display SDL_Surface to flip.
	 @return Error code: 0 No error
	 */
	static int UpdateDisplay(SDL_Surface* display);
	
	/**
	 Display a list of text to the screen with top/left corner at position x,y
	 
	 @param[in]		games List of strings to display
	 @param[in,out]	*font TTF_Font used to render the text.
	 @param[in,out]	*display SDL_Surface to display the text on.
	 @param[in]	x Integer position to display the image
	 @param[in]	y Integer position to display the image
	 @return Error code: 0 No error; -1 Failed to load font; -2 Failed to render text
	 */
	static int DisplayList(const list<string>& games, const int selected, TTF_Font* font, SDL_Surface* display, int x, int y);
	
	/**
	 Display a dialog box containing a list of categories centered at position x,y
	 
	 @param[in]		categories List of string values containing the available categories
	 @param[in]		selected The index value of the selected category
	 @param[in,out]	*font TTF_Font used to render the text.
	 @param[in,out]	*display SDL_Surface to display the dialog on.
	 @param[in]	x Integer position to display the image
	 @param[in]	y Integer position to display the image
	 @return Error code: 0 No error; -1 Could not generate list surface
	 */	
	static int DisplayCategoryBox(const list<string>& categories, const int selected, TTF_Font* font, SDL_Surface* display, int x, int y);
		
	/**
	 Determine the size of the dialog box needed to display the list of text
	 
	 @param[in]	text List of string values to be displayed in the dialog.
	 @param[in,out]	*font TTF_Font that will be used to render the text.
	 @return Error code: 0 No error
	 */
	static int DetermineDialogSize(const list<string>& text, TTF_Font* font);
	
	/**
	 Set the height and width of the image that is displayed using DisplayImage
	 
	 @param[in]	width Integer width of the image
	 @param[in]	height Integer height of the image
	 @return Error code: 0 No error
	 */
	static int SetImageSize(int width, int height);
	/**
	 Display a dialog box to the screen with an alert message
	 
	 @param[in]		&alert		String value to be displayed in the dialog
	 @param[in]		*font		TTF_Font used to render the text
	 @param[in,out] *display	SDL_Surface to render the dialog and text on
	 @param[in]		x			Position on the display, left
	 @param[in]		y			Position of the display, top
	 */
	static int DisplayAlertDialog(const string& alert, TTF_Font* font, SDL_Surface* display, int x, int y);
private:
	
	/**
	 The width of the category dialog box
	 */
	static int dialog_width;
	/**
	 The height of the category dialog box
	 */
	static int dialog_height;
	/**
	 The value to scale the image with to
	 */
	static int image_width;
	/**
	 The value to scale the image height to
	 */
	static int image_height;
	/**
	 The RGB color of the background
	 */
	static SDL_Color background_color;
	/**
	 The RGB color of normal text
	 */
	static SDL_Color normal_text_color;
	/**
	 The RBG color of highlighted text
	 */
	static SDL_Color highlighted_text_color;
};

#endif