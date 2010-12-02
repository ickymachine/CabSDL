/*
 *  locations.cpp
 *  CabSDL
 *
 *  Created by ickymachine on 11/18/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "locations.h"
#include <iostream>
#include <fstream>

//GLOBALS
extern const char* CONFIG_PATH;
extern const char* CONFIG_PATH_LOCAL;
extern const char* GAME_PATH;
extern const char* FONT;
extern const char* IMAGE_PATH;
extern const char* PROGRAM;

using namespace std;

Locations::Locations() {

}

Locations::~Locations() {
	
}

int Locations::SetGames(std::string name) {
	if (name != "") {
		games = name;
		return 0;
	}
	return -1;
}

int Locations::SetFont(std::string name) {
	if (name != "") {
		font = name;
		return 0;
	}
	return -1;
}

int Locations::SetCommand(std::string name) {
	if (name != "") {
		command = name;
		return 0;
	}
	return -1;
}

int Locations::SetImages(std::string name) {
	if (name != "") {
		images = name;
		return 0;
	}
	return -1;
}

std::string Locations::GetCommand() {
	return command;
}

std::string Locations::GetFont() {
	return font;
}

std::string Locations::GetGames() {
	return games;
}

std::string Locations::GetImages() {
	return images;
}
