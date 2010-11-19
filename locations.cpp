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

const char* CONFIG_PATH = "config.xml";

Locations::Locations() {

}

Locations::Locations(string gname, string fname, string cname) {
	games = gname;
	fonts = fname;
	command = cname;
}

Locations::~Locations() {
	
}

void Locations::SetGames(string name) {
	games = name;
}

void Locations::SetFonts(string name) {
	fonts = name;
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

string Locations::GetFonts() {
	return fonts;
}

string Locations::GetGames() {
	return games;
}

string Locations::GetImages() {
	return images;
}

void Locations::ParseConfig() {
	//Try to open the config file
	ifstream config (CONFIG_PATH);
	if (config.is_open()) {
		string line;
		//First line is games directory
		getline(config,line);
		SetGames(line);
		//Second line is fonts
		getline(config,line);
		SetFonts(line);
		//Third line is command
		getline(config,line);
		SetCommand(line);
		//Fourth line is image directory
		getline(config,line);
		SetImages(line);
	}
}