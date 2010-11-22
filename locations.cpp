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

Locations::Locations() {
	SetGames(GAME_PATH);
	SetFont(FONT);
	SetCommand(PROGRAM);
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