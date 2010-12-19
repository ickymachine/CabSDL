/*
 *  gamelist.cpp
 *  CabGL
 *
 *  Created by ickymachine on 11/16/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "gamelist.h"
#include <dirent.h>
#include <iostream>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "constants.h"

#ifdef __APPLE__
#include "mac.h"
#endif

GameList::GameList(const string& path) {
	//Load the category information from the local file descriptions.txt
	std::string filepath = "descriptions.txt";
#ifdef __APPLE__
	filepath = App::ResourcesPath()+"/descriptions.txt";
#endif
	std::ifstream descfile(filepath.c_str());
	if ( !descfile.good() ) {
		//File Does Not Exist
		std::cerr<<"Could not open file descriptions.txt"<<std::endl;
		exit(Error::FILE_DOES_NOT_EXIST);
	}
	//Parse the file
	if ( descfile.is_open() ) {
		std::string line;
		while (descfile.good()) {
			getline(descfile,line);
			//Check if the line is empty/newline
			if (line.compare("\n") == 0) continue;
			//Split the line into game name, description
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
			l_names.insert(std::pair<std::string,std::string>(desc,game));
		}
		descfile.close();
	}	
	
	//Check what games are available in the game path
	//Check if the path exists
	DIR* dir = NULL;
	if (dir = opendir(path.c_str())) {
		struct dirent *ent;
		while ((ent = readdir(dir)) != NULL)
		{
			string entry (ent->d_name);
			if (entry.find(".zip") != string::npos) {
				//clip off the extension
				games_full.push_back(entry.substr(0,entry.find(".zip")));
			}
		}
		games_full.sort();
		games.set(games_full.list());
		closedir(dir);
	}
	else {
		std::cerr<<"Could not open: "<<path<<" for reading."<<std::endl;
		exit(Error::FILE_DOES_NOT_EXIST);
	}

	
}

GameList::~GameList() {

}

string GameList::GetGame() {
	return games.get();
}

void GameList::MovePosition(int num) {
	games.move(num);
}

void GameList::Search(const string& description) {
	//Search through the descriptions map and find the closest match
	std::string closest_game;
	std::map<std::string, std::string>::iterator it = l_names.lower_bound(description);
	if (it != l_names.end()) {
		closest_game = it->second;
		//Now search the games list and move to the closest_game
		games.search(closest_game);
	}
}

list<string> GameList::GetList(int num) {
	list<string> rtn;
	rtn.clear();
	int dir = (num > 0 ? 1:-1);
	int vel = abs(num);
	for (int i = 0; i < vel; i++) {
		//Get the description
		string game_name = games.get();
		map<string,string>::iterator it = descriptions.find(game_name);
		if (dir == 1) {
			if ( it != descriptions.end() ) {
				rtn.push_back(it->second);
			}
			else {
				rtn.push_back(game_name);
			}
		}
		else {
			if ( it != descriptions.end() ) {
				rtn.push_front(it->second);
			}
			else {
				rtn.push_front(game_name);
			}
		} 
		games.move(dir);
	}
	games.move(0-num);
	return rtn;
}

list<string> GameList::GetList() {
	list<string> all_games = games.list();
	list<string> rtn;
	map<string,string>::iterator m_it;
	for (list<string>::iterator it = all_games.begin(); it != all_games.end(); it++) {
		m_it = descriptions.find(*it);
		if (m_it != descriptions.end()) {
			rtn.push_back(m_it->second);
		}
	}
	return rtn;
}

int GameList::Size() {
	return games.size();
}

void GameList::Filter(const list<string>& keep) {
	games.set(keep);
}

void GameList::Restore() {
	games.set(games_full.list());
}
