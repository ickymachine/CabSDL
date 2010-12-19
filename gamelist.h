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
	 Construct the list of games pased on the supplied directory path
	 
	 @param[in]		&path		String.  Directory to game .zip files.
	 */
	GameList(const string& path);	
	/**
	 Default Destructor
	 */	
	~GameList();
	/**
	 Return the current game
	 
	 @return string value associate with the current game
	 */
	string GetGame();
	/**
	 Return a list of Descriptions with size num
	 
	 @param[in]	num Number of games to get
	 @return list<string> containing num items from the games list
	 */
	list<string> GetList(int num);	//Get a list of num games
	/**
	 Return the entire list of Descriptions
	 
	 @return list<string> games
	 */
	list<string> GetList();	//return the entire list
	/**
	 Advance the position in games by num
	 
	 @param[in]	num	Amount to advance position in games
	 */	 
	void MovePosition(int num);
	/**
	 Search for container to see if there is a match for the provided description
	 
	 @param[in]	gamename	String value to compare against
	 */
	void Search(const string& description);
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
	/**
	 Container for the game name / description associations.  Key = name; Data = description
	 */
	std::map<std::string, std::string> descriptions;
	/**
	 Container for description / game name association.  Key = description; Data = name
	 */
	std::map<std::string, std::string> names;
	/**
	 Container for description / game name association.  All lowercase for faster searching.  Key = description(lowercase); Data = name
	 */
	std::map<std::string, std::string> l_names;
	
};

#endif