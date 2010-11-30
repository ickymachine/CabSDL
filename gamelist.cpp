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
#include <math.h>
#include "category.h"

GameList::GameList() {
	games.clear();
	games_full.clear();
	//_current = games.begin();
}

void GameList::Copy(list<string> copy) {
	games.clear();
	games_full.clear();
	games.set(copy);
	games_full.set(copy);
}

GameList::~GameList() {
	//Delete the game list 
	games.clear();
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
				//clip off the extension
				games.push_back(entry.substr(0,entry.find(".zip")));
				games_full.push_back(entry.substr(0,entry.find(".zip")));
			}
		}
		//Set the current current, next, previous games
		games.sort();
		return 0;
	}
	else {
		cout << "GameList::Initialize; Could not open: "<<path<<" for reading."<<endl;
	}	
	return 1; //Assume failure
}

string GameList::GetGame() {
	return games.get();
}

void GameList::MovePosition(int num) {
	games.move(num);
}

void GameList::Search(string gamename) {
	games.search(gamename);
}

list<string> GameList::GetList(int num) {
	list<string> rtn;
	rtn.clear();
	int dir = (num > 0 ? 1:-1);
	int vel = abs(num);
	for (int i = 0; i < vel; i++) {
		if (dir == 1) {
			rtn.push_back(games.get());
		}
		else {
			rtn.push_front(games.get());
		} 
		games.move(dir);
	}
	games.move(-num);
	return rtn;
}

list<string> GameList::GetList() {
	return games.list();
}

int GameList::Size() {
	return games.size();
}

void GameList::Filter(list<string> keep) {
	games.set(keep);
}

void GameList::Restore() {
	games.set(games_full.list());
}
