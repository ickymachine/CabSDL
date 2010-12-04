/*
 *  gamelist.h
 *  CabGL
 *
 *  Created by ickymachine on 11/16/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef GAMELIST_H
#define GAMELIST_H

#include <string>
#include <list>
#include "category.h"
#include "wlist.h"

using namespace std;

class GameList {
public:
	GameList();	//default constructor
	void Copy(const list<string>& copy);
	~GameList();
	int Initialize(const string& path);	//generate the rom list based on the directory provided
	string GetGame();
	list<string> GetList(int num);	//Get a list of num games
	list<string> GetList();	//return the entire list
	void MovePosition(int num);
	void Search(const string& gamename);
	int Size();
	void Filter(const list<string>& keep);
	void Restore();
private:
	//list<string> games;
	//list<string>::iterator _current;
	wlist games;
	wlist games_full;
};

#endif