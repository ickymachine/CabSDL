/*
 *  locations.h
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include "constants.h"

#ifndef LOCATIONS_H
#define LOCATIONS_H

class Locations {
public: 
	Locations();
	~Locations();
	std::string GetGames();
	std::string GetFont();
	std::string GetCommand();
	std::string GetImages();
	int SetGames(std::string name);
	int SetFont(std::string name);
	int SetCommand(std::string name);
	int SetImages(std::string name);
private:
	std::string games;
	std::string images;
	std::string font;
	std::string command;
};

#endif
