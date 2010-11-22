/*
 *  locations.h
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

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
private:
	void SetGames(string name);
	void SetFont(string name);
	void SetCommand(string name);
	void SetImages(string name);
	string games;
	string images;
	string font;
	string command;
};

#endif