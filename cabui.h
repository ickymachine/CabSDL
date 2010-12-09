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
#include "description.h"
#include "mamedb.h"

#include "resize++.h"

#include "configfile/configfile.h"

//Create an enumerated type to indicate the state of the display
enum DisplayState {
	LIST,
	SORT
};

class CabUI {
public:
	CabUI();
	
	int Cleanup();
	int Init();
	int HandleKeypress(SDL_Event* event);
	int HandleJoystick(SDL_Event* event);
	void Update();
private:
	std::list<std::string> CreateDisplayList(int size);
	int ProcessConfigFile();
	int UpdateDisplayList(SDL_keysym* key);
	std::string EnterText(SDL_KeyboardEvent* key);
	const char* ConstructExecutableCall();
	int FileExists(const char* path);
	void Move(SDL_Event* event, int distance);
	void Sort();
	void Launch();
	void Search(SDL_Event* event);
	
	//SDL 
	SDL_Surface*	screen;
	TTF_Font*		font;
	Uint32			videoflags;
	SDL_Joystick*	joy;
	
	//CabSDL
	GameList		game_list;
	Category		categories;
	Description		descriptions;
	ConfigFile*		cfg;
	Locations		location;
	DisplayState	status;
	std::string		searchterm;
	int				selectedgame;
	int				selectedcategory;
	int				display_list_size;
	std::list<std::string>	game_display_list;
	
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
};


#endif
