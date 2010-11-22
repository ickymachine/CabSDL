/*
 *  locations.h
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

//UNIX
#if defined (__unix__)
//Set Default Path to unix normal .config folder
#define CONFIG_PATH "~/.config/cabsdl/config.cfg" // ~/.config/cabsdl/config.cfg
#define CONFIG_PATH_LOCAL "config.cfg" //Alternate location to look if normal config file is missing
#define GAME_PATH "~/.cabsdl/roms/"
#define FONT "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf"
#define IMAGE_PATH "~/.cabsdl/titles"
#define PROGRAM "/usr/games/mame"
#endif

//MAC
#if defined (__APPLE__)
#define CONFIG_PATH "~/.config/cabsdl/config.cfg"
#define CONFIG_PATH_LOCAL "config.cfg" //Alternate location to look if normal config file is missing
#define GAME_PATH "~/.cabsdl/roms/"
#define FONT "/Library/Fonts/Arial.ttf"
#define IMAGE_PATH "~/.cabsdl/titles"
#define PROGRAM "mame"
#endif

//WINDOWS
#if defined (_WIN32) || (WIN32) || (windows)
#define CONFIG_PATH ".\\config\\config.cfg"
#define GAME_PATH ".\\roms"
#define FONT "?"
#define IMAGE_PATH ".\\titles"
#endif 

#ifndef LOCATIONS_H
#define LOCATIONS_H

#include <string>

using namespace std;

class Locations {
public: 
	Locations();
	~Locations();
	int ParseConfig();
	string GetGames();
	string GetFont();
	string GetCommand();
	string GetImages();
	void SetGames(string name);
	void SetFont(string name);
	void SetCommand(string name);
	void SetImages(string name);
private:
	string games;
	string images;
	string font;
	string command;
};

#endif