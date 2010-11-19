
/* Simple program:  Create a blank window, wait for keypress, quit.

   Please see the SDL documentation for details on using the SDL API:
   /Developer/Documentation/SDL/docs.html
*/
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include <SDL_ttf/SDL_ttf.h>
#include <SDL.h>

#include <string.h>
#include "gamelist.h"
#include "gameimage.h"
#include "gamechooser.h"

static string command;
static GameList game_list;
static GameChooser game_chooser;
SDL_Surface *screen;

static void UpdateDisplay() {
	//Clear the display
	SDL_FillRect(screen, NULL, 0x000000);
	//Display the currently selected game
	game_chooser.Update(game_list.GetGame());
	game_chooser.Display(screen);
	//Update the screen
	SDL_Flip(screen);
}

static int HandleKeypress(SDL_Event event) {
	//Determine which key was pressed
	switch (event.key.keysym.sym) {
		case SDLK_ESCAPE:
			return 1;
			break;
		case SDLK_LEFT:
			game_list.MovePosition(1,-1);
			UpdateDisplay();
			break;
		case SDLK_RIGHT:
			game_list.MovePosition(1,1);		
			UpdateDisplay();
			break;
		case SDLK_UP:
			game_list.MovePosition(3,-1);
			UpdateDisplay();
			break;
		case SDLK_DOWN:
			game_list.MovePosition(3,1);	
			UpdateDisplay();
			break;
		case SDLK_SPACE:
			command = "/Applications/Games/MAME/sdlmame0140-x86_64/mame64 -rompath /Applications/Games/MAME/v138/roms "+game_list.GetGame();
			//Execute the command-line program
			system(command.c_str());			
			break;
		default:
			// DO NOTHING 
			break;
	}
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
	game_list.Initialize("/Applications/Games/MAME/v138/roms");
	game_list.PrintList();
	command = "";

	// Initial the SDL_TTF
	if (TTF_Init() < 0)
	{
		printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
		exit(1);
	}
	
	//Load the SDL font
	game_chooser.InitFont("/Library/Fonts/Arial.ttf",16);
	
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
	
	//Initial Screen Diplay
//	UpdateDisplay();
	
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
	game_chooser.CloseFont();
	
	/* Clean up the SDL library */
	SDL_Quit();
	TTF_Quit();
	return(0);
}
