/*
 *  description.h
 *  CabSDL
 *
 *  Created by ickymachine on 12/2/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <map>
#include <string>
#include <list>

class Description {
public:
	Description();
	std::string Name(const std::string& game_name);
	std::list<std::string> List();
private:
	std::map<std::string, std::string> descriptions;	//key = name; data = description
};

#endif
