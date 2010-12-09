// $Id$
/**
 *  @file wlist.h
 *  Provides a list of string objects that support wrapping around the beginning and end of the list.
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Wrapper around std::list to provide a list of strings with iterator indexing that will wrap around the begin/end of the list.

#ifndef WLIST_H
#define WLIST_H

#include <list>
#include <string>
#include "stdlib.h"

/**
 *	List of string objects.  Element access by move(int) function to position the index.
 *
 *	Position values that exceed the size of the list or are less than zero will wrap around to the beginning or end, respectively.
 *	
 */

class wlist {
public:
	/**
	   Default constructor.  Clear the list, set position to 0.
	 */
	wlist() {
		data.clear();
		position = 0;
	}
	/**
	   Destructor.  Clears the list.
	 */
	~wlist() {
		data.clear();
	}
	/**
	    Accessor for list.clear()
	 */
	void clear() {
		data.clear();
	}
	/**
		Accessor for list.push_back()
	 
		@param[in]	item String value to add to the end of the list.
	 */
	void push_back(const std::string& item) {
		data.push_back(item);
	}
	/**
		Accessor for list.sort()
	 */
	void sort() {
		data.sort();
	}
	/**
		Clears out the current contents of the list and replaces them with the passed values.  Resets position to 0.
	 
		@param[in]	values List of string values to replace the current data.
	 */
	void set(const std::list<std::string>& values) {
		data.clear();
		data = values;
		data.sort();
		position = 0;
	}
	/**
		Move the position indexing into the list.
	 
		@param[in]	amount Value added to position.
	 */
	void move(int amount) {
		position+=amount;
	}
	/**
		Return the string value from the list indexed by position.  If the list is empty return an empty string.
	 
		@return The string value at element position in the list
	 */
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
	/**
		Get the list of strings.
	 
		@return The member variable data, list of strings
	 */
	std::list<std::string> list() {
		return data;
	}
	/**
		Search the list for a match and move the position to that element.
		If the element is not found the position of the list is not changed
		
		@param[in]	searchterm String value to match against the content of the list
	 */
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
	/**
		Accessor to list size() function.
	 */
	int size() {
		return data.size();
	}
private:
	/**
		List of string objects
	 */
	std::list<std::string> data;
	/** 
		Index into the list
	 
	 	Values can be negative or exceed data.size(), will wrap around to --end() and begin().
	 */
	int position;
};

#endif
