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
	_current = games.begin();
}

void GameList::Copy(GameList* copy) {
	games.clear();
	games = copy->GetList();
	_current = games.begin();
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
			}
		}
		//Set the current current, next, previous games
		games.sort();
		_current = games.begin();
		return 0;
	}
	else {
		cout << "GameList::Initialize; Could not open: "<<path<<" for reading."<<endl;
	}	
	return 1; //Assume failure
}

string GameList::GetGame() {
	
	return *_current;
}

void GameList::MovePosition(int num, int dir) {
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

void GameList::Search(string gamename) {
	list<string>::iterator pos = games.begin();
	int done = 0;
	while (pos != games.end() && done != 1) {
		if ((pos->substr(0,gamename.size())).compare(gamename) == 0) {
			//Found a match
			done = 1;
			//Move the iterator to the found game
			_current = pos;
		}
		pos++;
	}
	//Didn't find the title
}

list<string> GameList::GetList(int num) {
	if (num > 0) {
		list<string>::iterator pos = _current;
		list<string> return_list;
		return_list.clear();
		for (int i = 0; i < num; i++) {
			if (pos == games.end()) pos = games.begin();
			return_list.push_back(*pos);
			++pos;
		}
		return return_list;
	}
	else {
		int vel = abs(num);
		list<string>::iterator pos = _current;
		for (int i = 0; i < vel; i++) {
			if (pos == games.begin()) {
				pos = games.end();
			}
			else {
				pos--;
			}
		}
		list<string> return_list;
		return_list.clear();
		for (int i = 0; i < vel; i++) {
			if (pos == games.end()) pos = games.begin();
			return_list.push_back(*pos);
			++pos;
		}
		return return_list;
	}
}

list<string> GameList::GetList() {
	list<string> rtn = games;
	return rtn;
}

int GameList::Size() {
	return games.size();
}

list<string> GameList::Sort(string category, Category* categories) {
	list<string> rtn;
	//Iterate through the entire list of games and add it to the return if it matches
	for (list<string>::iterator it = games.begin(); it != games.end(); it++) {
		if (category.compare(categories->GetCategory(*it)) == 0) {
			rtn.push_back(*it);
		}
		else {
			games.remove(*it);
		}
	}
	_current = games.begin();
	return rtn;
}
