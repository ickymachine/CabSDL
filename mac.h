// $Id$
/**
 *  @file mac.h
 *  Allow access to items in the Resources path of the .app package
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Provide access to items contained in the Resources path of the .app package.


#ifndef MAC_H
#define MAC_H

#include "CoreFoundation/CoreFoundation.h"
#include <string>

class App {
public:
	/**
		Return the path to the Resources folder in the .app package
	 
		@return String containing the path to .app/Contents/Resources
	 */
	static std::string ResourcesPath();		
};

#endif