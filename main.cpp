
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

#include "SDL_ttf.h"
#include "SDL.h"

#include <string>
#include "gamelist.h"
#include "gameimage.h"
#include "locations.h"
#include "cabdisplay.h"

#include "configfile/configfile.h"

using namespace std;

//Globals
GameList game_list;
SDL_Surface* screen;
Locations location;
TTF_Font* font;
string searchterm;
int selectedgame;
list<string> game_display_list;
ConfigFile* cfg;
Uint32 videoflags;
int res_width;
int res_height;
int display_list_size = 20;

/*
static void DisplayText(string text, SDL_Surface* display) {
	SDL_Rect destination;
	//Display the Rom name to the screen
	SDL_Color color={255,255,255};	//White
	SDL_Surface* text_surface;
	//Check if the font loaded properly
	if (font == NULL) {
		cout<<"Failed to load font!"<<endl;
	}
	else {
		//Generate the game name from the GameImage path string
		if (!(text_surface = TTF_RenderText_Solid(font, text.c_str(), color))) {
			//Handle Error
			cout<<"Couldn't create text_surface!"<<endl;
		}
		else {
			destination.x = display->w/2;
			destination.y = display->h-20;
			SDL_BlitSurface(text_surface, NULL, display, &destination);
			SDL_FreeSurface(text_surface);
		}
	}
	
}
*/

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
			cout<<"Searching: "<<searchterm<<endl;
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
//		cout<<"DISPLAY LIST WRAPPING BACK"<<endl;
	}
	else if (selectedgame == display_list_size && key->sym == SDLK_RIGHT) {
		game_display_list = game_list.GetList(display_list_size);
		selectedgame = 1;
//		cout<<"DISPLAY LIST WRAPPING FORWARD"<<endl;
	}
	else if (key->sym == SDLK_UP || key->sym == SDLK_DOWN) {
		game_display_list = game_list.GetList(display_list_size);
		selectedgame = 1;
//		cout<<"DISPLAY LIST JUMPING"<<endl;
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
			cout<<ConstructExecutableCall()<<endl;
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

static int Init() {
	//Initialize our variables
	//	location.SetGames("/Applications/Games/MAME/v138/roms");
	//	location.SetFonts("/Library/Fonts/Arial.ttf");
	//	location.SetCommand("/Applications/Games/MAME/sdlmame0140-x86_64/mame64 -rompath /Applications/Games/MAME/v138/roms ");

	//Load the config file
	cfg = new ConfigFile(CONFIG_PATH_LOCAL);
	cfg->dump();
	location.SetGames(cfg->getvalue<string>("game_path"));
	location.SetFont(cfg->getvalue<string>("font_path"));
	location.SetCommand(cfg->getvalue<string>("command"));
	cout<<cfg->getvalue<string>("command")<<endl;
	location.SetImages(cfg->getvalue<string>("image_path"));
	
	//Check if there are any settings for SDL
	if (int h = cfg->getvalue<int>("height")) {
		res_height = h;
	}
	if (int w = cfg->getvalue<int>("width")) {
		res_width = w;
	}
	if (cfg->getvalue<string>("fullscreen") == "true") {
		cout<<"VIDEOFLAGS "<<videoflags<<endl;
		videoflags = videoflags|SDL_FULLSCREEN;
		cout<<"SET FOR FULLSCREEN "<<videoflags<<endl;
	}
	
	/*
	if (location.ParseConfig() == -1) {
		cout<<"No valid config file, exiting program."<<endl;
		exit(1);
	}
	*/
	
	game_list.Initialize(location.GetGames());
	if (game_list.Size() < display_list_size) {
		//limit display list to the size of the game list
		display_list_size = game_list.Size();
	}
	//	game_list.PrintList();
	searchterm = "";
	selectedgame = 1;
	game_display_list = game_list.GetList(display_list_size);
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
	Init();
	
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
	}
	else {
		printf("Font loaded succesfully.\n");
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
	
	//Make sure the game_chooser font is closed
	TTF_CloseFont(font);
	
	/* Clean up the SDL library */
	SDL_Quit();
	TTF_Quit();
	return(0);
}
