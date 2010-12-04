/*
 *  locations.h
 *  CabSDL
 *
 *  Created by ickymachine on 11/18/10.
 *  Copyright 2010. All rights reserved.
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
	std::string GetGames() const;
	std::string GetFont() const;
	std::string GetCommand() const;
	std::string GetImages() const;
	int SetGames(const std::string& name);
	int SetFont(const std::string& name);
	int SetCommand(const std::string& name);
	int SetImages(const std::string& name);
private:
	std::string games;
	std::string images;
	std::string font;
	std::string command;
};

#endif
