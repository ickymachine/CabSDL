/*
 *  category.cpp
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/29/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "category.h"
#include <fstream>
#include <iostream>

Category::Category() {
	//Zero out the map and list
	game_categories.clear();
	category_games.clear();
	category_types.clear();
	
	category_types.push_back("ALL");
	
	//Load the category information from the local file categories.txt
	std::ifstream catlist("categories.txt");
	if ( ! catlist.good()) {
		//File Does Not Exist
		printf("Category; Could not open file categories.txt");
	}
	//Parse the file
	if (catlist.is_open()) {
		std::string line;
		while (catlist.good()) {
			getline(catlist,line);
			std::string game = line.substr(0,line.find("="));
			int pos = line.find("=");
			std::string cat = line.substr(pos+1,line.length()-2-pos);
			//Add the key,data pair to the map
			game_categories.insert(std::pair<std::string,std::string>(game,cat));
//			std::cout<<"GAME "<<game<<" CAT "<<cat<<std::endl;
			
			//Check if the category is in the list
			if (category_types.empty()) {
				category_types.push_back(cat);
//				std::cout<<"Initial; Adding "<<cat<<" to list"<<std::endl;
			}
			else {
				int found = 0;
				std::list<std::string>::iterator it = category_types.begin();
				while (it != category_types.end() && found != 1) {
					//Check if the category exists
					if (cat.compare(*it) == 0) {
						found = 1;
//						std::cout<<"FOUND A MATCH FOR "<<cat<<" in "<<*it<<std::endl;
					}
					it++;
				}
				if (found == 0) {
					//The category was not found so add it to the list
					category_types.push_back(cat);
//					std::cout<<"Adding "<<cat<<" to list"<<std::endl;
				}
			}
		}
		catlist.close();
	}
	//construct the reverse map
	for (std::map<std::string,std::string>::iterator it = game_categories.begin(); it != game_categories.end(); it++) {
		game_categories.insert(std::pair<std::string,std::string>(it->second,it->first));
	}
}

Category::~Category() {
	//Clear the map
	game_categories.clear();
	category_games.clear();
	category_types.clear();
}

std::string Category::GetCategory(std::string gamename) {
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
