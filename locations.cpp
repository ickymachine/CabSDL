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
#include "constants.h"

Locations::Locations() {

}

Locations::~Locations() {
	
}

int Locations::SetGames(const std::string& name) {
	if (name != "") {
		games = name;
		return Error::NO_ERROR;
	}
	return Error::EMPTY_STRING;
}

int Locations::SetFont(const std::string& name) {
	if (name != "") {
		font = name;
		return Error::NO_ERROR;
	}
	return Error::EMPTY_STRING;
}

int Locations::SetCommand(const std::string& name) {
	if (name != "") {
		command = name;
		return Error::NO_ERROR;
	}
	return Error::EMPTY_STRING;
}

int Locations::SetImages(const std::string& name) {
	if (name != "") {
		images = name;
		return Error::NO_ERROR;
	}
	return Error::EMPTY_STRING;
}

std::string Locations::GetCommand() const {
	return command;
}

std::string Locations::GetFont() const {
	return font;
}

std::string Locations::GetGames() const {
	return games;
}

std::string Locations::GetImages() const {
	return images;
}
