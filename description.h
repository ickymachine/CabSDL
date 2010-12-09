// $Id$
/**
 *  @file description.h
 *  Handles matching of game names with descriptions (readable game names).
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Handles matching of game name with descriptions from file descriptions.txt.

#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#include <map>
#include <string>
#include <list>

class Description {
public:
	/**
	 Default constructor.  Reads data into the map from file descriptions.txt
	 */
	Description();
	/**
	 Finds the description associated with game name
	 
	 @param[in]	game_name the name of the game to match
	 @return associated description
	 */
	std::string Name(const std::string& game_name);
	/**
	 Lists all the descriptions
	 
	 @return list<string> containing all the descriptions loaded from descriptions.txt
	 */
	std::list<std::string> List();
private:
	/**
	 Container for the game name / description associations.  Key = name; Data = description
	 */
	std::map<std::string, std::string> descriptions;
};

#endif
