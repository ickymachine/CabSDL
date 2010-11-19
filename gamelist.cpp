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
<<<<<<< HEAD
	_current = games.begin();
=======
	current = games.begin();
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
}

GameList::~GameList() {
//	delete current;
	//Delete the game list 
//	games.~list();
}

int GameList::Initialize(string path) {
<<<<<<< HEAD
	//Clear out the list
	games.clear();
=======
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
	//Check if the path exists
	DIR *dir = opendir(path.c_str());
	if (dir) {
		struct dirent *ent;
		while ((ent = readdir(dir)) != NULL)
		{
			string entry (ent->d_name);
			if (entry.find(".zip") != string::npos) {
				games.push_back(entry);
<<<<<<< HEAD
//				cout <<"Added "<<ent->d_name<<" to GameList"<<endl;
			}
		}
		//Set the current current, next, previous games
		_current = games.begin();
=======
				cout <<"Added "<<ent->d_name<<" to GameList"<<endl;
			}
		}
		//Set the current pointer to the first entry, previous to last
		current = games.begin();
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
		return 0;
	}
	else {
		cout << "Could not open: "<<path<<" for reading."<<endl;
	}	
	return 1; //Assume failure
}

<<<<<<< HEAD
string GameList::GetGame() {
	
	return *_current;
=======
string GameList::GetGame(int pos, int direction) {
	for (int i = 0; i < pos; i++) {
		if (direction == 1) {
			++current;
		}
		else {
			--current;
		}

	}
	string currentGame = *current;
	return currentGame;
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
}
void GameList::MovePosition(uint num, int dir) {
	if (dir > 0) {
		for (int i = 0; i < num; i++) {
<<<<<<< HEAD
			if (_current == --games.end()) {
				_current = games.begin();
			}
			else {
				_current++;
			}
=======
			current++;
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
		}
	}
	else if (dir < 0) {
		for (int i = 0; i < num; i++) {
<<<<<<< HEAD
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
=======
			current--;
		}
	}
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
}