/*
 *  description.cpp
 *  CabSDL
 *
 *  Created by ickymachine on 12/2/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "description.h"
#include <fstream>

#ifdef __APPLE__
#include "mac.h"
#endif

Description::Description() {
	//Load the category information from the local file categories.txt
	std::string filepath = "descriptions.txt";
#ifdef __APPLE__
	filepath = App::ResourcesPath()+"/descriptions.txt";
#endif
	std::ifstream descfile(filepath.c_str());
	if ( ! descfile.good()) {
		//File Does Not Exist
		printf("Could not open file descriptions.txt");
	}
	//Parse the file
	if (descfile.is_open()) {
		std::string line;
		while (descfile.good()) {
			getline(descfile,line);
			
			//Check if the line is empty/newline
			if (line.compare("\n") == 0) continue;
			
			std::string game = line.substr(0,line.find("="));
			int pos = line.find("=");
			std::string desc = line.substr(pos+1,line.length()-1-pos);
			//Add the key,data pair to the map
			descriptions.insert(std::pair<std::string,std::string>(game,desc));
		}
		descfile.close();
	}	
}

std::string Description::Name(const std::string& game_name) {
	//Located the description for game_name
	std::map<std::string, std::string>::iterator it = descriptions.find(game_name);
	if (it != descriptions.end()) {
		//Found the key match
		return it->second;
	}
	return game_name;
}