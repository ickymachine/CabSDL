// $Id$
/**
 *  @file mac.h
 *  Container for matching game name to category type based on the file catver.ini
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Provide a container for matching game name to category type using file categories.txt.

#ifndef CATEGORY_H
#define CATEGORY_H

#include <map>
#include <string>
#include <list>

class Category {
public:
	/**
		Default constructor
	 */
	Category();
	/**
		Default destructor
	 */
	~Category();
	/**
		Determines the category associated with a game name.
	 
		@param[in]	gamename String value containing the name of the game.
		@return The associated category or N/A if no match is found
	 */
	std::string GetCategory(const std::string& gamename);
	/**
		List all the categories found from the categories.txt file.
	 
		@return List of string values of categories.
	 */
	std::list<std::string> List();
	/**
		Print the list of categories to console output.
	 */
	void Print();
	/**
		Finds the list of games that match a specific category.
	 
		@param[in] category The category to match the list of games against
		@return List of game names that belong to category.
	 */
	std::list<std::string> GetMatches(const std::string& category);
private:
	/**
		Map of string values; key = game name, data = category
	 */
	std::map<std::string, std::string> game_categories;	//Key = game name; Data = category
	/**
		Map of string values; key = category, data = game name
	 */
	std::multimap<std::string, std::string> category_games; //Key = category; Data = game name
	/**
		List of string value; all available categories from categories.txt
	 */
	std::list<std::string> category_types; //A list of all the available categories
};

#endif