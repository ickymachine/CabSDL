/*
 *  locations.h
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>

using namespace std;

class Locations {
public: 
	Locations();
	~Locations();
	Locations(string gname, string fname, string cname);
	void SetGames(string name);
	void SetFonts(string name);
	void SetCommand(string name);
	void SetImages(string name);
	void ParseConfig();
	string GetGames();
	string GetFonts();
	string GetCommand();
	string GetImages();
private:
	string games;
	string images;
	string fonts;
	string command;
};