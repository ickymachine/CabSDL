/*
 *  gamelist.h
 *  CabGL
 *
 *  Created by Matthew Capodarco on 11/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include <list>

using namespace std;

class GameList {
public:
	GameList();	//default constructor
	~GameList();
	int Initialize(string path);	//generate the rom list based on the directory provided
<<<<<<< HEAD
	string GetGame();
	void MovePosition(uint num, int dir);
	void PrintList();
private:
	list<string> games;
	list<string>::iterator _current;
=======
	string GetGame(int pos = 0, int direction = 1);
	void MovePosition(uint num, int dir);
private:
	list<string> games;
	list<string>::iterator current;
>>>>>>> d8b522b21e89d85d310aa08577fdbc011e9507a2
};