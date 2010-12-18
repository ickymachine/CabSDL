/*
 *  gamelist.cpp
 *  CabGL
 *
 *  Created by ickymachine on 11/16/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "gamelist.h"
#include <dirent.h>
#include <iostream>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"

GameList::GameList() {
	games.clear();
	games_full.clear();
}

void GameList::Copy(const list<string>& copy) {
	games.clear();
	games_full.clear();
	games.set(copy);
	games_full.set(copy);
}

GameList::~GameList() {
	//Delete the game list 
	games.clear();
}

int GameList::Initialize(const string& path) {
	//Clear out the list
	games_full.clear();
	//Check if the path exists
	DIR* dir = NULL;
	if (dir = opendir(path.c_str())) {
		struct dirent *ent;
		while ((ent = readdir(dir)) != NULL)
		{
			string entry (ent->d_name);
			if (entry.find(".zip") != string::npos) {
				//clip off the extension
				games_full.push_back(entry.substr(0,entry.find(".zip")));
			}
		}
		games_full.sort();
		games.set(games_full.list());
		closedir(dir);
		return Error::NO_ERROR;
	}
	std::cerr<<"Could not open: "<<path<<" for reading."<<std::endl;
	return Error::FILE_DOES_NOT_EXIST;
}

string GameList::GetGame() {
	return games.get();
}

void GameList::MovePosition(int num) {
	games.move(num);
}

void GameList::Search(const string& gamename) {
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
	games.move(0-num);
	return rtn;
}

list<string> GameList::GetList() {
	return games.list();
}

int GameList::Size() {
	return games.size();
}

void GameList::Filter(const list<string>& keep) {
	games.set(keep);
}

void GameList::Restore() {
	games.set(games_full.list());
}
