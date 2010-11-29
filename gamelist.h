/*
 *  gamelist.h
 *  CabGL
 *
 *  Created by Matthew Capodarco on 11/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GAMELIST_H
#define GAMELIST_H

#include <string>
#include <list>
#include "category.h"

using namespace std;

class GameList {
public:
	GameList();	//default constructor
	void Copy(GameList* copy);
	~GameList();
	int Initialize(string path);	//generate the rom list based on the directory provided
	string GetGame();
	list<string> GetList(int num);	//Get a list of num games
	list<string> GetList();	//return the entire list
	list<string> Sort(string category, Category* categories);
	void MovePosition(int num, int dir);
	void PrintList();
	void Search(string gamename);
	int Size();
private:
	list<string> games;
	list<string>::iterator _current;
};

#endif