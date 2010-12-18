// $Id$
/**
 *  @file constants.h
 *  Namespace to hold all the constansts for CabSDL
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Namespace CabSDL to declare and initialize any constant values used by CabSDL.  Includes default path locations.

#ifndef CABSDL_CONSTANTS
#define CABSDL_CONSTANTS

#include <string>

namespace Error {
	//Define error codes
	static int NO_ERROR = 0;
	static int FILE_DOES_NOT_EXIST = -1;
	static int FAILED_TO_LOAD_FONT = -2;
	static int FAILED_TO_CREATE_SURFACE = -3;
	static int EMPTY_STRING = -4;
	static int NO_CONFIG = -5;
	static int INIT_FAILED = -6;
	static int FAILED_TO_CREATE_ALERT_DIALOG = -7;
	static int FAILED_TO_INITIALIZE = -8;
}

namespace CabSDL {
	//POSIX
#if defined (__unix__) || (__APPLE__)
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
	
	//Generate the path of the user home directory
	static uid_t uid = getuid();
	static passwd* p = getpwuid(uid);
	static std::string home_path   = (p == NULL ? "":p->pw_dir);
	static std::string config_path = home_path+ "/.config/cabsdl/config.cfg";
	static std::string game_path   = home_path+ "/.config/cabsdl/config.cfg";
	static std::string image_path  = home_path+ "/.config/cabsdl/titles";
	//convert them to const char*
//	const char* cp = config_path.c_str();
//	const char* gp = game_path.c_str();
//	const char* ip = image_path.c_str();
	
	//Initialize the constants
	static const char* CONFIG_PATH_LOCAL = "config.cfg"; //Alternate location to look if normal config file is missing
	static const char* CONFIG_PATH = config_path.c_str();
	static const char* GAME_PATH = game_path.c_str();
	static const char* IMAGE_PATH = image_path.c_str();
#endif
	
	//UNIX
#if defined (__unix__)
	static const char* FONT = "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf";
	static const char* PROGRAM = "/usr/games/mame";
#endif
	
	//MAC
#if defined (__APPLE__)
	static const char* FONT = "/Library/Fonts/Arial.ttf";
	static const char* PROGRAM = "mame";
#endif
	
	//WINDOWS
#if defined (_WIN32) || (WIN32) || (windows)
	static const char* CONFIG_PATH = "config\\config.cfg"; //relative to home directory
	static const char* CONFIG_PATH_LOCAL = "config.cfg"; //Alternate location to look if normal config file is missing
	static const char* GAME_PATH = "roms";
	static const char* FONT = "fonts\\Arial.ttf";
	static const char* IMAGE_PATH = "titles";
	static const char* PROGRAM = "mame";
#endif 
	
}

#endif