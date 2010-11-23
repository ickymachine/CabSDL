
/* Simple program:  Create a blank window, wait for keypress, quit.

   Please see the SDL documentation for details on using the SDL API:
   /Developer/Documentation/SDL/docs.html
*/
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "SDL_ttf.h"
#include "SDL.h"

#include <string>
#include "gamelist.h"
#include "gameimage.h"
#include "locations.h"
#include "cabdisplay.h"
#include "constants.h"

#include "configfile/configfile.h"

using namespace std;

//GLOBALS
extern const char* CONFIG_PATH;
extern const char* CONFIG_PATH_LOCAL;
extern const char* GAME_PATH;
extern const char* FONT;
extern const char* IMAGE_PATH;
extern const char* PROGRAM;

//Globals
GameList game_list;
Locations location;
string searchterm;
int selectedgame;
list<string> game_display_list;
ConfigFile* cfg;
int display_list_size = 20;

//SDL
SDL_Surface* screen;
TTF_Font* font;

//Video Mode
Uint32 videoflags;
int res_width;
int res_height;

static int FileExists(const char* path) {
	ifstream check_file(path);
	if (!check_file.good()) {
		//File does not exist
		check_file.close();
		return 0;
	}
	check_file.close();
	return 1;
}

static const char* ConstructExecutableCall() {
	string call = "";
	//First is the command
	call+=location.GetCommand()+" ";
	//Check if there are any parameters
	int index = 0;
	//Convert index to a string
	std::ostringstream sin;
	sin<<index;
	string arg = "param"+sin.str();
	cout<<arg<<endl;
	string param = cfg->getvalue<string>(arg);
	cout<<param<<endl;
	while (param != "") {
		call+=param+" ";
		arg = "value"+sin.str();
		cout<<arg<<endl;
		//Check if there is an associated value
		string value = cfg->getvalue<string>(arg);
		if (value != "") {
			call+=value+" ";
		}
		index++;
		sin.str("");
		sin<<index;
		arg = "param"+sin.str();
		param = cfg->getvalue<string>(arg);
	}
	//Add the game name to the end of the call
	call+=game_list.GetGame()+".zip";
	return call.c_str();
}

static void EnterSearchTerm(SDL_KeyboardEvent* key) {
	if( key->type == SDL_KEYDOWN ){
		// If the Unicode value is less than 0x80 then it can be converted to ASCCI text using (char)unicode.
		if( key->keysym.unicode < 0x80 && key->keysym.unicode > 0 ){
			//Add the character to the search term
			searchterm.push_back((char)key->keysym.unicode);
		}
	}
}

static string GetImagePath() {
	string out = location.GetImages()+game_list.GetGame();
	return out;
}

static int UpdateDisplayList(SDL_keysym* key) {
	//Check to see if list needs to be updated
	if (selectedgame == 1 && key->sym == SDLK_LEFT) {
		game_display_list = game_list.GetList(-display_list_size);
		selectedgame = display_list_size;
	}
	else if (selectedgame == display_list_size && key->sym == SDLK_RIGHT) {
		game_display_list = game_list.GetList(display_list_size);
		selectedgame = 1;
	}
	else if (key->sym == SDLK_UP || key->sym == SDLK_DOWN) {
		game_display_list = game_list.GetList(display_list_size);
		selectedgame = 1;
	}
	else if (key->sym == SDLK_RIGHT) {
			selectedgame++;
	}
	else if (key->sym == SDLK_LEFT) {
			selectedgame--;
	}
	else {
		//Must have been a search
		game_display_list = game_list.GetList(display_list_size);
		selectedgame = 1;
	}
	return 0;
}

static void Update() {
	CabDisplay::BlankDisplay(screen);
	//Display the game image
	CabDisplay::DisplayImage(GameImage::ScaleImage(GameImage::GenerateImage(GetImagePath()),static_cast<int>(res_width*(0.85)), static_cast<int>(res_height*(0.90))),screen);
	//Display the game name
	CabDisplay::DisplayText(game_list.GetGame(),font,screen,(screen->w/10),screen->h-20);
	//Display the search term
	CabDisplay::DisplayText("SEARCH: "+searchterm,font,screen, (screen->w/2), screen->h-20);
	//Display the list of games
	CabDisplay::DisplayList(game_display_list,selectedgame,font,screen,screen->w, 5);
	//Update the screen
	CabDisplay::UpdateDisplay(screen);
}

static int HandleKeypress(SDL_Event event) {
	//Determine which key was pressed
	switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			return 1;
			break;
		case SDLK_LEFT:
			game_list.MovePosition(1,-1);
			searchterm = "";
			UpdateDisplayList(&event.key.keysym);
			Update();
			break;
		case SDLK_RIGHT:
			game_list.MovePosition(1,1);
			searchterm = "";
			UpdateDisplayList(&event.key.keysym);
			Update();
			break;
		case SDLK_UP:
			game_list.MovePosition(display_list_size,-1);
			searchterm = "";
			UpdateDisplayList(&event.key.keysym);
			Update();
			break;
		case SDLK_DOWN:
			game_list.MovePosition(display_list_size,1);
			searchterm = "";
			UpdateDisplayList(&event.key.keysym);
			Update();
			break;
		case SDLK_SPACE: {
			searchterm = "";
			//Switch out of fullscreen
			screen = SDL_SetVideoMode(res_width,res_height, 0, SDL_SWSURFACE);
			//Execute the command-line program
			system(ConstructExecutableCall());
			//Switch back to fullscreen
			screen = SDL_SetVideoMode(res_width,res_height, 0, videoflags);
			Update();
			}
			break;
		case SDLK_BACKSPACE:
			searchterm = "";
			Update();
			break;
		default:
			//Assume search initiated
			EnterSearchTerm(&event.key);
			game_list.Search(searchterm);
			UpdateDisplayList(&event.key.keysym);
			Update();
			break;
	}
	return 0;
}

static int ProcessConfigFile() {
	//Check if the default config file exists
	if (!FileExists(CONFIG_PATH)) {
		//Check the local path
		if (!FileExists(CONFIG_PATH_LOCAL)) {
			//No valid condif file found
			cout<<"ERROR; No valid config file found at "<<CONFIG_PATH<<" or "<<CONFIG_PATH_LOCAL<<endl;
			return 1;
		}
		else {
			cfg = new ConfigFile(CONFIG_PATH_LOCAL);
		}
	}
	else {
		cfg = new ConfigFile(CONFIG_PATH);
	}
	//	cfg->dump();
	
	//Validate the values from the config file
	if ( location.SetGames(cfg->getvalue<string>("game_path")) == -1) {
		location.SetGames(GAME_PATH); //Set to default
	}
	if ( location.SetFont(cfg->getvalue<string>("font_path")) == -1) {
		location.SetFont(FONT); //Set to default
	}
	if ( location.SetCommand(cfg->getvalue<string>("command")) == -1) {
			location.SetCommand(PROGRAM); //Set to default
	}
	if ( location.SetImages(cfg->getvalue<string>("image_path")) == -1) {
		location.SetImages(IMAGE_PATH); //Set to default
	}
		
	if (!FileExists(location.GetGames().c_str())) {
		cout<<"ERROR; Game directory"<<location.GetGames()<<" does not exist"<<endl;
		return 0;
	}
	if (!FileExists(location.GetFont().c_str())) {
		cout<<"ERROR; Font file "<<location.GetFont()<<" does not exist"<<endl;
		return 0;
	}
	if (!FileExists(location.GetCommand().c_str())) {
		cout<<"ERROR; Command file "<<location.GetCommand()<<" does not exist"<<endl;
		return 0;
	}
	if (!FileExists(location.GetImages().c_str())) {
		cout<<"ERROR; Images directory "<<location.GetImages()<<" does not exist"<<endl;
		return 0;
	}
	
	//Everything loaded successfully
	return 1;
}

static int Init() {
	//Process the config file
	if (!ProcessConfigFile()) {
		cout<<"ERROR; Invalid Config file"<<endl;
		return -1;
	}
	
	//Check if there are any settings for SDL
	if (int h = cfg->getvalue<int>("height")) {
		res_height = h;
	}
	if (int w = cfg->getvalue<int>("width")) {
		res_width = w;
	}
	if (cfg->getvalue<string>("fullscreen") == "true") {
		videoflags = videoflags|SDL_FULLSCREEN;
	}
	
	game_list.Initialize(location.GetGames());
	if (game_list.Size() < display_list_size) {
		//limit display list to the size of the game list
		display_list_size = game_list.Size();
	}
	searchterm = "";
	selectedgame = 1;
	game_display_list = game_list.GetList(display_list_size);
	return 0;
}

static int Cleanup() {
	//Delete pointers
	delete cfg;
	return 0;
}

int main(int argc, char *argv[])
{
	Uint32 initflags = SDL_INIT_VIDEO;  /* See documentation for details */
	Uint8  video_bpp = 0;
	videoflags = SDL_SWSURFACE;
	res_width = 640;
	res_height = 480;
	
	int    done;
	SDL_Event event;
	
	//Initialize our variables
	if (Init() < 0) {
		printf("Counldn't initialize CabSDL, exiting.");
		exit(1);
	}
	
	// Initial the SDL_TTF
	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
		exit(1);
	}
	
	//Load the SDL font
	int fontsize = cfg->getvalue<int>("font_size");
	font = TTF_OpenFont((char*)location.GetFont().c_str(),(fontsize > 0 ? fontsize:16));
	if (!font) {
		printf("TTF_OpenFont: %s ",TTF_GetError());
		exit(1);
	}
	
	/* Initialize the SDL library */
	if ( SDL_Init(initflags) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",
			SDL_GetError());
		exit(1);
	}

	/* Set 640x480 video mode */
	screen=SDL_SetVideoMode(res_width,res_height, video_bpp, videoflags);
        if (screen == NULL) {
		fprintf(stderr, "Couldn't set 640x480x%d video mode: %s\n",
                        video_bpp, SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	
	//Enable Unicode Support
	SDL_EnableUNICODE(1);
	
	//Initial Screen Diplay
	Update();
	
	done = 0;
	while ( !done ) {

		/* Check for events */
		while ( SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_KEYDOWN:
					done = HandleKeypress(event);
					break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}
	}
	
	//Run cleanup on any pointers
	Cleanup();
	
	//Make sure the game_chooser font is closed
	TTF_CloseFont(font);
	
	/* Clean up the SDL library */
	SDL_Quit();
	TTF_Quit();
	return(0);
}
