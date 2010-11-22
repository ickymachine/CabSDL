/*
 *  locations.cpp
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "locations.h"
#include <iostream>
#include <fstream>

//UNIX
#if defined (__unix__)
//Set Default Path to unix normal .config folder
const char* CONFIG_PATH = "~/.config/cabsdl/config.xml"; // ~/.config/cabsdl/config.xml
const char* CONFIG_PATH_LOCAL = "config.xml"; //Alternate location to look if normal config file is missing
const char* GAME_PATH = "~/.cabsdl/roms/";
const char* FONT = "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf";
const char* IMAGE_PATH = "~/.cabsdl/titles";
const char* COMMAND = "/usr/games/mame";
#endif


//MAC
#if defined (__APPLE__)
const char* CONFIG_PATH = "~/.config/cabsdl/config.xml";
const char* CONFIG_PATH_LOCAL = "config.xml"; //Alternate location to look if normal config file is missing
const char* GAME_PATH = "~/.cabsdl/roms/";
const char* FONT = "/Library/Fonts/Arial.ttf";
const char* IMAGE_PATH = "~/.cabsdl/titles";
const char* COMMAND = "mame";
#endif

//WINDOWS
#if defined (_WIN32) || (WIN32) || (windows)
const char* CONFIG_PATH = ".\\config\\config.xml";
const char* GAME_PATH = ".\\roms";
const char* FONT = "?";
const char* IMAGE_PATH = ".\\titles";
#endif 

Locations::Locations() {
	SetGames(GAME_PATH);
	SetFont(FONT);
	SetCommand(COMMAND);
	SetImages(IMAGE_PATH);
}

Locations::~Locations() {
	
}

void Locations::SetGames(string name) {
	games = name;
}

void Locations::SetFont(string name) {
	font = name;
}

void Locations::SetCommand(string name) {
	command = name;
}

void Locations::SetImages(string name) {
	images = name;
}

string Locations::GetCommand() {
	return command;
}

string Locations::GetFont() {
	return font;
}

string Locations::GetGames() {
	return games;
}

string Locations::GetImages() {
	return images;
}

int Locations::ParseConfig() {
	//Try to open the config file
	ifstream config (CONFIG_PATH);
	if (config.is_open()) {
		cout<<"Opened config file in default location"<<endl;
		string line;
		//First line is games directory
		getline(config,line);
		SetGames(line);
		//Second line is fonts
		getline(config,line);
		SetFont(line);
		//Third line is command
		getline(config,line);
		SetCommand(line);
		//Fourth line is image directory
		getline(config,line);
		SetImages(line);
		return 0;
	}
	//Couldn't open default path try file in local directory
	else {
		config.open(CONFIG_PATH_LOCAL);
		if (config.is_open()) {
			cout<<"Opened local config file"<<endl;
			string line;
			//First line is games directory
			getline(config,line);
			SetGames(line);
			//Second line is fonts
			getline(config,line);
			SetFont(line);
			//Third line is command
			getline(config,line);
			SetCommand(line);
			//Fourth line is image directory
			getline(config,line);
			SetImages(line);
			return 0;
		}
		else {
			cout<<"No valid config file!"<<endl;
			return -1;
		}
	}
}