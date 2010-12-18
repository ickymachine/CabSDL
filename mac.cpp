/*
 *  mac.cpp
 *  CabSDL
 *
 *  Created by ickymachine on 12/1/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "mac.h"
#include <iostream>

std::string App::ResourcesPath() {
	std::string rtn;
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
	{
		// error!
		std::cerr<<"Error establishing path of .app Resources folder."<<std::endl;
	}
	CFRelease(resourcesURL);
	rtn = path;	
	return rtn;
}		
