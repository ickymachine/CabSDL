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
	/**
	 Find the game name associated with a description
	 
	 @param[in]	description	String value containing the description to search for
	 @return associated game name
	 */
	std::string Desc(const std::string& desc);
	/**
	 Search the map for a game name that partly matches a description
	 
	 @param[in]		search_term		String value to compare to descriptions
	 @return		associated game name
	 */
	std::string Search(const std::string& search_term);
private:
	/**
	 Container for the game name / description associations.  Key = name; Data = description
	 */
	std::map<std::string, std::string> descriptions;
	/**
	 Container for description / game name association.  Key = description; Data = name
	 */
	std::map<std::string, std::string> names;
	/**
	 Container for description / game name association.  All lowercase for faster searching.  Key = description(lowercase); Data = name
	 */
	std::map<std::string, std::string> lowercase_names;
};

#endif
