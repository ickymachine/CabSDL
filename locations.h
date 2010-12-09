// $Id$
/**
 *  @file locations.h
 *  Container for all paths needed by CabSDL
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Container for paths to the various assets required by CabSDL

#include <string>
#include "constants.h"

#ifndef LOCATIONS_H
#define LOCATIONS_H

class Locations {
public: 
	/**
	 Default Constructor
	 */
	Locations();
	/**
	 Default Destructor
	 */
	~Locations();
	/**
	 Get the path to the game .zip files
	 
	 @return string containing the path
	 */
	std::string GetGames() const;
	/**
	 Get the path to the font .ttf file
	 
	 @return string containing the path
	 */
	std::string GetFont() const;
	/**
	 Get the path to the binary
	 
	 @return string containing the path
	 */
	std::string GetCommand() const;
	/**
	 Get the path to the game .png files
	 
	 @return string containing the path
	 */
	std::string GetImages() const;
	/**
	 Set the path to the game .zip files
	 
	 @param [in]	name	string value containing the path
	 @return Error Code: 0 No error; -1 Error
	 */
	int SetGames(const std::string& name);
	/**
	 Set the path to the font .ttf file
	 
	 @param [in]	name	string value containing the path
	 @return Error Code: 0 No error; -1 Error
	 */
	int SetFont(const std::string& name);
	/**
	 Set the path to the binary
	 
	 @param [in]	name	string value containing the path
	 @return Error Code: 0 No error; -1 Error
	 */
	int SetCommand(const std::string& name);
	/**
	 Set the path to the game .png files
	 
	 @param [in]	name	string value containing the path
	 @return Error Code: 0 No error; -1 Error
	 */
	int SetImages(const std::string& name);
private:
	/**
	 Path to game .zip files
	 */
	std::string games;
	/** 
	 Path to game .png files
	 */
	std::string images;
	/**
	 Path to font .ttf file
	 */
	std::string font;
	/**
	 Path to binary
	 */
	std::string command;
};

#endif
