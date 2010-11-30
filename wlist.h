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
#include <math.h>

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
	void push_back(std::string item) {
		data.push_back(item);
	}
	void sort() {
		data.sort();
	}
	void set(std::list<std::string> values) {
		data = values;
		position = 0;
	}
	void move(int amount) {
		position+=amount;
	}
	std::string get() {
		std::list<std::string>::iterator it = data.begin();
		int dir = (position > 0 ? 1:-1);
		int vel = abs(position);
		for (int i = 0; i < vel; i++) {
			if (dir == 1) {
				if (it == data.end()) {
					it = data.begin();
				}		
				else {
					it++;
				}
			}
			else {
				if (it == data.begin()) {
					it = data.end();
					it--;
				}
				else {
					it--;
				}
			}
		}
		return *it;
	}
	std::list<std::string> list() {
		std::list<std::string> rtn;
		rtn.clear();
		for (std::list<std::string>::iterator i = data.begin(); i != data.end(); i++) {
			rtn.push_back(*i);
		}
		return rtn;
	}
	void search(std::string searchterm) {
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
