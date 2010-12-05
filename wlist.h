/*
 *  wlist.h
 *  CabSDL
 *
 *  Created by ickymachine on 11/30/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef WLIST_H
#define WLIST_H

#include <list>
#include <string>
#include "stdlib.h"

class wlist {
public:
	wlist() {
		data.clear();
		position = 0;
	}
	~wlist() {
		data.clear();
	}
	void clear() {
		data.clear();
	}
	void push_back(const std::string& item) {
		data.push_back(item);
	}
	void sort() {
		data.sort();
	}
	void set(const std::list<std::string>& values) {
		data.clear();
		data = values;
		data.sort();
		position = 0;
	}
	void move(int amount) {
		position+=amount;
	}
	std::string get() {
		std::list<std::string>::iterator i = data.begin();
		int dir = (position > 0 ? 1:-1);
		int vel = abs(position);
		for (int j = 0; j < vel; j++) {
			//Forward
			if (dir == 1) {
				i++;
				if (i == data.end()) i = data.begin();
			}
			//Backward
			else {
				if (i == data.begin()) {
					i = data.end();
					i--;
				}
				else {
					i--;
				}
			}
		}
		return *i;
	}
	std::list<std::string> list() {
		std::list<std::string> rtn;
		rtn.clear();
		for (std::list<std::string>::iterator i = data.begin(); i != data.end(); i++) {
			rtn.push_back(*i);
		}
		return rtn;
	}
	void search(const std::string& searchterm) {
		std::list<std::string>::iterator i = data.begin();
		int done = 0;
		int pos = 0;
		while (i != data.end() && done != 1) {
			if ((i->substr(0,searchterm.size())).compare(searchterm) == 0) {
				//Found a match
				done = 1;
				//Move to the found game
				position = pos;
			}
			i++;
			pos++;
		}
		//Didn't find the title
	}
	int size() {
		return data.size();
	}
private:
	std::list<std::string> data;
	int position;
};

#endif
