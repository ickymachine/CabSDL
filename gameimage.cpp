/*
 *  gameimage.cpp
 *  CabGL
 *
 *  Created by Matthew Capodarco on 11/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "gameimage.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_image/SDL_image.h>
#include <string>

using namespace std;

const char * location = "/Applications/Games/MAME/v138/titles/";

GameImage::GameImage() {
	
}

GameImage::~GameImage() {
	
}

SDL_Surface* GameImage::LoadImageFile(char * filename){
	SDL_Surface * sur = IMG_Load(filename);
	if (sur == NULL) {
		cout<<"Couldn't load image from file: "<<filename<<endl;
	}
	return sur;
}

void GameImage::GenerateImage(string game_name) {
	//Assign the image name
	name = game_name;
	//Locate the proper image
	string filename = FindImageFile(name);
	//Load the desired image
	image = LoadImageFile((char*)filename.c_str());
	//check if the image loading failed
	if (image == NULL) {
		//Load and display default image instead
		cout<<"ERROR; GameImage::Display; Couldn't load image at "<<filename<<endl;
		image = LoadImageFile("default.png");
		if (image == NULL) {
			//couldn't load default for some reason
			cout<<"ERROR; GameImage::Display; Couldn't load default.png"<<endl;
			return;
		}
	}
}

string GameImage::FindImageFile(string game) {
	//Remove the file extension
	string name = game.substr(0,game.find("."));
	//add the location path to the name and append .png
	return location+name+".png";
}

SDL_Surface* GameImage::GetImage() {
	return image;
}

string GameImage::GetName() {
	return name;
}
