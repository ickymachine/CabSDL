/*
 *  constants.cpp
 *  CabSDL
 *
 *  Created by Matthew Capodarco on 11/23/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "constants.h"
#include <string>

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
static const char* cp = config_path.c_str();
static const char* gp = game_path.c_str();
static const char* ip = image_path.c_str();

//Initialize the constants
extern const char* CONFIG_PATH_LOCAL = "config.cfg"; //Alternate location to look if normal config file is missing
extern const char* CONFIG_PATH = cp;
extern const char* GAME_PATH = gp;
extern const char* IMAGE_PATH = ip;
#endif

//UNIX
#if defined (__unix__)
extern const char* FONT = "/usr/share/fonts/truetype/msttcorefonts/Arial.ttf";
extern const char* PROGRAM = "/usr/games/mame";
#endif

//MAC
#if defined (__APPLE__)
extern const char* FONT = "/Library/Fonts/Arial.ttf";
extern const char* PROGRAM = "mame";
#endif

//WINDOWS
#if defined (_WIN32) || (WIN32) || (windows)
extern const char* CONFIG_PATH = "config\\config.cfg"; //relative to home directory
extern const char* CONFIG_PATH_LOCAL = "config.cfg"; //Alternate location to look if normal config file is missing
extern const char* GAME_PATH = "roms";
extern const char* FONT = "fonts\\Arial.ttf";
extern const char* IMAGE_PATH = "titles";
extern const char* PROGRAM = "mame";
#endif 
