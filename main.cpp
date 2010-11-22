
/* Simple program:  Create a blank window, wait for keypress, quit.

   Please see the SDL documentation for details on using the SDL API:
   /Developer/Documentation/SDL/docs.html
*/
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "SDL_ttf.h"
#include "SDL.h"

#include <string>
#include "gamelist.h"
#include "gameimage.h"
#include "locations.h"
#include "cabdisplay.h"

using namespace std;

//Globals
string command;
GameList game_list;
//GameChooser game_chooser;
SDL_Surface* screen;
Locations location;
TTF_Font* font;
string searchterm;
//ConfigFile cfg;

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



static void Update() {
/*
	//Clear the display
	SDL_FillRect(screen, NULL, 0x000000);
	//Display the currently selected game
	game_chooser.Update(GetImagePath());
	game_chooser.Display(screen);
	DisplayText("SEARCH: "+searchterm,screen);
	//Update the screen
	SDL_Flip(screen);
*/
	CabDisplay::BlankDisplay(screen);
	//Display the game image
	CabDisplay::DisplayImage(GameImage::ScaleImage(GameImage::GenerateImage(GetImagePath()), 400, 400),screen);
	//Display the game name
	CabDisplay::DisplayText(game_list.GetGame(),font,screen,0,screen->h-20);
	//Display the search term
	CabDisplay::DisplayText("SEARCH: "+searchterm,font,screen, (screen->w/2), screen->h-20);
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
			Update();
			break;
		case SDLK_RIGHT:
			game_list.MovePosition(1,1);
			searchterm = "";
			Update();
			break;
		case SDLK_UP:
			game_list.MovePosition(3,-1);
			searchterm = "";
			Update();
			break;
		case SDLK_DOWN:
			game_list.MovePosition(3,1);
			searchterm = "";
			Update();
			break;
		case SDLK_SPACE:
			command = location.GetCommand()+game_list.GetGame()+".zip";
			searchterm = "";
			//Execute the command-line program
			system(command.c_str());			
			break;
		case SDLK_BACKSPACE:
			searchterm = "";
			Update();
			break;
		default:
			//Assume search initiated
			EnterSearchTerm(&event.key);
			game_list.Search(searchterm);
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
	if (location.ParseConfig() == -1) {
		cout<<"No valid config file, exiting program."<<endl;
		exit(1);
	}
	
	game_list.Initialize(location.GetGames());
	//	game_list.PrintList();
	command = "";
	searchterm = "";
	return 0;
}

int main(int argc, char *argv[])
{
	Uint32 initflags = SDL_INIT_VIDEO;  /* See documentation for details */
	Uint8  video_bpp = 0;
	Uint32 videoflags = SDL_SWSURFACE;
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
//	game_chooser.InitFont((char*)location.GetFonts().c_str(),16);
	font = TTF_OpenFont((char*)location.GetFont().c_str(),16);
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
	screen=SDL_SetVideoMode(640,480, video_bpp, videoflags);
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
