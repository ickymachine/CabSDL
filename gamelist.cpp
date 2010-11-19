/*
 *  gamelist.cpp
 *  CabGL
 *
 *  Created by Matthew Capodarco on 11/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "gamelist.h"
#include <dirent.h>
#include <iostream>
#include <sys/types.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

GameList::GameList() {
	games.clear();
	_current = games.begin();
}

GameList::~GameList() {
//	delete current;
	//Delete the game list 
//	games.~list();
}

int GameList::Initialize(string path) {
	//Clear out the list
	games.clear();
	//Check if the path exists
	DIR *dir = opendir(path.c_str());
	if (dir) {
		struct dirent *ent;
		while ((ent = readdir(dir)) != NULL)
		{
			string entry (ent->d_name);
			if (entry.find(".zip") != string::npos) {
				games.push_back(entry);
//				cout <<"Added "<<ent->d_name<<" to GameList"<<endl;
			}
		}
		//Set the current current, next, previous games
		_current = games.begin();
		return 0;
	}
	else {
		cout << "Could not open: "<<path<<" for reading."<<endl;
	}	
	return 1; //Assume failure
}

string GameList::GetGame() {
	
	return *_current;
}
void GameList::MovePosition(uint num, int dir) {
	if (dir > 0) {
		for (int i = 0; i < num; i++) {
			if (_current == --games.end()) {
				_current = games.begin();
			}
			else {
				_current++;
			}
		}
	}
	else if (dir < 0) {
		for (int i = 0; i < num; i++) {
			if (_current == games.begin()) {
				_current = --games.end();
			} else {
				_current--;
			}
		}
	}
}
void GameList::PrintList() {
	list<string>::iterator it;
	for (it = games.begin(); it != games.end(); it++) {
		cout<<*it<<endl;
	}
}