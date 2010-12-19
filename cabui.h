// $Id$
/**
 *  @file cabui.h
 *  Primary class for CabSDL
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Primary class for CabSDL, handles all data storage, user interaction, and display.

#ifndef CABUI_H
#define CABUI_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_joystick.h"
#include "stdio.h"
#include "stdlib.h"
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <sstream>

#include "gamelist.h"
#include "gameimage.h"
#include "locations.h"
#include "cabdisplay.h"
#include "constants.h"
#include "category.h"
#include "mamedb.h"
#include "wlist.h"
#include "event.h"

#include "resize++.h"

#include "configfile/configfile.h"

//Create an enumerated type to indicate the state of the display
enum DisplayState {
	LIST,
	SORT,
	SEARCH
};

class CabUI : public Event {
public:
	CabUI();
	int Execute();
private:
	
	//Rendering Methods
	void Render();
	void RenderDisplayList();
	void RenderGameImage();
	void RenderGameInfo();
	void RenderCategoryDialog();
	
	//Generate a new game list when reaching beginning/end, searching, or sorting
	std::list<std::string> CreateDisplayList(int size);
	
	//Initialize all the components
	int Init();
	//Process the configuration file
	int ProcessConfigFile();
	//Cleanup any pointers, TTF, Joysticks, and SDL
	int Cleanup();
	
	//
	const char* ConstructExecutableCall();
	
	//Check if the file at path exists
	int FileExists(const char* path);
	
		
	//Event Handling
	void OnEvent(SDL_Event* event);
	void OnExit();
	void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	void OnJoyButtonDown(Uint8 which, Uint8 button);
	void OnJoyHat(Uint8 which, Uint8 hat, Uint8 value);
	void OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value);
	
	//Move the list by distance
	void Move(int distance);
	//Sort the gamelist by the selected category
	void Sort();
	//Launch the selected game
	void Launch();
	//Search for searchterm in the target list
	void Search(Uint16 unicode);
	
	
	//SDL 
	SDL_Surface*	screen; //Main surface for drawing all components
	TTF_Font*		font; //Font to render text
	Uint32			videoflags; //Videoflags, primarily to set if fullscreen or not
	SDL_Joystick*	joy; //Player1 joystick
	
	//CabSDL
	GameList*		game_list;
	Category		categories;
	Locations		location;
	DisplayState	status;
	DisplayState	previous_status;
	std::string		searchterm;
	int				selectedgame;
	int				selectedcategory;
	int				display_list_size;
	int				category_list_size;
	wlist			available_categories;
	bool			running;
	std::list<std::string> game_display_list;
	std::string		executable_arguments;
	
	//Interface sizing and positioning
	int				res_width;
	int				res_height;
	
	int				x_image;
	int				y_image;
	
	int				x_list;
	int				y_list;
	
	int				x_gamename;
	int				y_gamename;
	
	int				x_category;
	int				y_category;
	
	int				x_search;
	int				y_search;
	
	//Joystick bindings
	Uint8			joy_button_select;
	Uint8			joy_button_sort;
	Uint8			joy_button_escape;
};


#endif
