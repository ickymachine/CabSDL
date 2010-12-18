/*
 *  category.cpp
 *  CabSDL
 *
 *  Created by ickymachine on 11/29/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "category.h"
#include <fstream>
#include <iostream>
#include <string>

#ifdef __APPLE__
#include "mac.h"
#endif

Category::Category() {
	//Zero out the map and list
	game_categories.clear();
	category_games.clear();
	category_types.clear();
	
	//Load the category information from the local file categories.txt
	std::string filepath = "categories.txt";
#ifdef __APPLE__
	filepath = App::ResourcesPath()+"/categories.txt";
#endif
	std::ifstream catlist(filepath.c_str());
	if ( ! catlist.good()) {
		//File Does Not Exist
		std::cerr<<"Category; Could not open file categories.txt"<<std::endl;
	}
	//Parse the file
	if (catlist.is_open()) {
		std::string line;
		while (catlist.good()) {
			getline(catlist,line);
			
			//Check if the line is empty/newline
			if (line.compare("\n") == 0) continue;
			
			std::string game = line.substr(0,line.find("="));
			int pos = line.find("=");
			std::string cat = line.substr(pos+1,line.length()-2-pos);
			//Add the key,data pair to the map
			game_categories.insert(std::pair<std::string,std::string>(game,cat));
			
			//Check if the category is in the list
			if (category_types.empty()) {
				category_types.push_back(cat);
			}
			else {
				int found = 0;
				std::list<std::string>::iterator it = category_types.begin();
				while (it != category_types.end() && found != 1) {
					//Check if the category exists
					if (cat.compare(*it) == 0) {
						found = 1;
					}
					it++;
				}
				if (found == 0) {
					//The category was not found so add it to the list
					if (cat.length() > 0) {
						category_types.push_back(cat);
					}
				}
			}
		}
		
		category_types.sort();
		
		//Include the ALL category to reset the list of games
		category_types.push_front("ALL");

		catlist.close();
	}
	//construct the reverse map
	for (std::map<std::string,std::string>::iterator it = game_categories.begin(); it != game_categories.end(); it++) {
		category_games.insert(std::pair<std::string,std::string>(it->second,it->first));
	}
}

Category::~Category() {
	//Clear the map
	game_categories.clear();
	category_games.clear();
	category_types.clear();
}

std::string Category::GetCategory(const std::string& gamename) {
	//Declare iterator to search the map
	std::map<std::string, std::string>::iterator fnd = game_categories.find(gamename);
	
	if (fnd == game_categories.end()) {
		//Did not locate gamename in the map.  Assign N/A as category
		return "N/A";
	}
	
	//Return the Data value (Category name) from the itegerator
	std::string rtn = fnd->second;
	return rtn;
}

std::list<std::string> Category::List() {
	return category_types;
}

void Category::Print() {
	for (std::list<std::string>::iterator i = category_types.begin(); i != category_types.end(); i++) {
		std::cout<<*i<<std::endl;
	}
}

std::list<std::string> Category::GetMatches(const std::string& category) {
	//Find the range of elements that match the category
	std::pair<std::multimap<std::string,std::string>::iterator,std::multimap<std::string,std::string>::iterator> range;
	range = category_games.equal_range(category);
	//Push the values into a return list
	std::list<std::string> rtn;
	rtn.clear();
	for (std::multimap<std::string,std::string>::iterator i = range.first; i != range.second; i++) {
		rtn.push_back((*i).second);
		std::cout<<(*i).second<<std::endl;
	}
	return rtn;
}
