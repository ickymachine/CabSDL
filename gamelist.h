// $Id$
/**
 *  @file gamelist.h
 *  A container of games.
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Container for a list of games.  Provides functions to generate a list of games based on the contents of a directory, search, and filter.

#ifndef GAMELIST_H
#define GAMELIST_H

#include <string>
#include <list>
#include "category.h"
#include "wlist.h"

using namespace std;

class GameList {
public:
	/**
	 Default Constructor
	 */
	GameList();	//default constructor
	/**
	 Default Destructor
	 */	
	~GameList();
	/**
	 Replace the list of games with copy
	 
	 @param[in] &copy	list<string> containing the new contents for games
	 */
	void Copy(const list<string>& copy);
	/**
	 Generates the list of games based on .zip files contained in the directory (path)
	 
	 @param[in]	path	Path to the list of .zip game files
	 @return Error code: 0 No errors; 1 Could not open path for reading
	 */
	int Initialize(const string& path);	//generate the rom list based on the directory provided
	/**
	 Return the current game
	 
	 @return string value associate with the current game
	 */
	string GetGame();
	/**
	 Return a list of games with size num
	 
	 @param[in]	num Number of games to get
	 @return list<string> containing num items from the games list
	 */
	list<string> GetList(int num);	//Get a list of num games
	/**
	 Return the entire list of games
	 
	 @return list<string> games
	 */
	list<string> GetList();	//return the entire list
	/**
	 Advance the position in games by num
	 
	 @param[in]	num	Amount to advance position in games
	 */	 
	void MovePosition(int num);
	/**
	 Search the games container to see if there is a match
	 
	 @param[in]	gamename	String value to compare against
	 */
	void Search(const string& gamename);
	/**
	 Returns the size of the games object, the number of games in the list
	 
	 @return The number of games in the list
	 */
	int Size();
	/**
	 Remove all games that are not included in the list keep
	 
	 @param[in]	keep	list<string> containing the game names to keep in the list
	 */
	void Filter(const list<string>& keep);
	/**
	 Restore games to the full list before any filter was applied
	 */
	void Restore();
private:
	/**
	 Container containing the list of game names of interest
	 */
	wlist games;
	/**
	 Container containing the entire unfiltered list of loaded game names
	 */
	wlist games_full;
};

#endif