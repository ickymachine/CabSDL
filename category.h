/*
 *  category.h
 *  CabSDL
 *
 *  Created by ickymachine on 11/29/10.
 *
 */

#ifndef CATEGORY_H
#define CATEGORY_H

#include <map>
#include <string>
#include <list>

class Category {
public:
	Category();
	~Category();
	std::string GetCategory(std::string gamename);
	std::list<std::string> List();
	void Print();
	std::list<std::string> GetMatches(std::string category);
private:
	std::map<std::string, std::string> game_categories;	//Key = game name; Data = category
	std::multimap<std::string, std::string> category_games; //Key = category; Data = game name
	std::list<std::string> category_types; //A list of all the available categories
};

#endif