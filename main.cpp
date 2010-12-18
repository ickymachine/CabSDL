
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
	CabUI		cabui;
	
	return(cabui.Execute());
}
