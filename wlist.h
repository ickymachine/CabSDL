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
 *	List of string objects.  Element access by move(int) function to position the _current iterator.
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
		_current = data.begin();
		_prev_search = data.begin();
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
		Clears out the current contents of the list and replaces them with the passed values.  Resets _current to data.begin().
	 
		@param[in]	values List of string values to replace the current data.
	 */
	void set(const std::list<std::string>& values) {
		data.clear();
		data = values;
		_current = data.begin();
	}
	/**
		Move the _current iterator by amount.
	 
		@param[in]	amount Value to move _current.
	 */
	void move(int amount) {
		int dir = (amount > 0 ? 1:-1);
		int vel = abs(amount);
		for (int i = 0; i < vel; i++) {
			//Forward
			if (dir == 1) {
				_current++;
				if (_current == data.end()) _current = data.begin();
			}
			//Backward
			else {
				if (_current == data.begin()) _current = data.end();
				_current--;
			}
		}
	}
	/**
		Return the string value from _current.  If the list is empty return an empty string.
	 
		@return The string value at element position in the list
	 */
	std::string get() const {
		if (data.size() == 0) {
			return "";
		}
		//std::list<std::string>::const_iterator i = _current;
		return *_current;
	}
	/**
		Get the list of strings.
	 
		@return The member variable data, list of strings
	 */
	std::list<std::string> list() const {
		return data;
	}
	/**
		Search the list for a match and move the position to that element.
		If the element is not found the position of the list is not changed
		
		@param[in]	searchterm String value to match against the content of the list
	 */
	void search(const std::string& searchterm) {
		std::list<std::string>::iterator i = _prev_search;
		bool done = false;
		int count = 1;
		int max = data.size();
		while (!done) {
			//If the substring of the data matches searchterm, it's a match
			std::string cmp = i->substr(0,searchterm.size());
			//convert to lowercase
			for (int pos = 0; pos < cmp.size(); pos++) {
				cmp[pos] = tolower(cmp[pos]);
			}
			if ((cmp).compare(searchterm) == 0) {
				//Found a match
				done = true;
				//Move to the found game
				_current = i;
				//Set the found match to this game
				_prev_search = i;
			}
			i++;
			if (i == data.end()) {
				i = data.begin();
			}
			count++;
			if (count >= max) {
				//Didn't find a match, but searched the whole list
				done = true;
				_prev_search = i;
			}
		}
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
		Iterator pointing to the current element
	 
	 	Will wrap around to --end() and begin() if decremented or incremented past begin or end.
	 */
	std::list<std::string>::iterator _current;
	/**
	 Iterator pointing at the last found element in the list
	 */
	std::list<std::string>::iterator _prev_search;
};

#endif
