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
#include <iostream>

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
		std::cerr<<"Could not open file descriptions.txt"<<std::endl;
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
			//Add the reverse key,data pair
			names.insert(std::pair<std::string,std::string>(desc,game));
			//Lowercase the description and add it to the reverse map
			for (int i = 0; i < desc.size(); i++) {
				desc[i] = tolower(desc[i]);
			}
			lowercase_names.insert(std::pair<std::string,std::string>(desc,game));
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

std::list<std::string> Description::List() {
	std::list<std::string> rtn;
	for (std::map<std::string, std::string>::iterator i = descriptions.begin(); i != descriptions.end(); i++) {
		rtn.push_back(i->second);
	}
	return rtn;
}

std::string Description::Desc(const std::string& desc) {
	//Located the description for game_name
	std::map<std::string, std::string>::iterator it = names.find(desc);
	if (it != names.end()) {
		//Found the key match
		return it->second;
	}
	return desc;
}

std::string Description::Search(const std::string& search_term) {
	std::string rtn = "";
	//Find the 1st closest match
	std::map<std::string, std::string>::iterator it = lowercase_names.lower_bound(search_term);
	if (it != lowercase_names.end())
		//Return the associated gamename
		rtn = it->second;
	return rtn;
}
