
/* Simple program:  Create a blank window, wait for keypress, quit.

   Please see the SDL documentation for details on using the SDL API:
   /Developer/Documentation/SDL/docs.html
*/
   
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <list>
#include <string>

#include "SDL_ttf.h"
#include "SDL.h"

#include "cabui.h"


int main(int argc, char *argv[])
{
	int			done;
	SDL_Event	event;
	CabUI		cabui;
	
	//Initialize variables
	if (cabui.Init() < 0) {
		printf("Counldn't initialize CabSDL, exiting.");
		exit(1);
	}
	
	//Initial Screen Diplay
	cabui.Update();
	
	//Poll for events from the use
	done = 0;
	while ( !done ) {
		/* Check for events */
		if ( SDL_PollEvent(&event) ) {	//SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_MOUSEMOTION:
					break;
				case SDL_MOUSEBUTTONDOWN:
					break;
				case SDL_KEYDOWN:
					done = cabui.HandleKeypress(event);
					break;
				case SDL_QUIT:
					done = 1;
					break;
				default:
					break;
			}
		}
		else {
			//No event so sleep to prevent 100% cpu utilization
			SDL_Delay(1);
		}
	}
	
	//Run cleanup on any pointers
	cabui.Cleanup();
	
	/* Clean up the SDL library */
	SDL_Quit();
	return(0);
}
