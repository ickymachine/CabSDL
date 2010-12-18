/*
 *  cabui.cpp
 *  CabSDL
 *
 *  Created by ickymachine on 12/2/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "cabui.h"

CabUI::CabUI() {
	//Switch to bool to true
	running = true;
	
	//Initialize pointers to NULL for safety
	screen = NULL;
	font = NULL;
	joy = NULL;
	
	//Video Mode
	videoflags = SDL_SWSURFACE;
	
	//Default resolution is 640x480
	res_width = 480;
	res_height = 640;
	
	//Joystick defaults
	joy_button_select = 0;
	joy_button_sort = 0;
	
	//Category list sizing
	category_list_size = 10;
	
	//Empty the searchterm, initialize the currently selected game and categories, set mode to LIST
	searchterm = "";
	selectedgame = 0;
	selectedcategory = 0;
	status = LIST;
	executable_arguments = "";
	
	//Initialize SDL to support video and joystick
	Uint32 initflags = SDL_INIT_VIDEO|SDL_INIT_JOYSTICK;  /* See documentation for details */
	
	/* Initialize the SDL library */
	if ( SDL_Init(initflags) < 0 ) {
		std::cerr<<"Couldn't initialize SDL: "<<SDL_GetError()<<std::endl;
		exit(1);	//Exit the program with code 1
	}
	//Enable Unicode Support
	SDL_EnableUNICODE(1);
	
	//Check for Joystick, open if available
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	if (SDL_NumJoysticks() > 0) {
		joy=SDL_JoystickOpen(0);
		
		if (joy) {
			printf("Open Joystick 0\n");
			printf("Name: %s\n", SDL_JoystickName(0));
			printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
			printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
			printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
		}
		else
			printf("Couldn't open Joystick 0\n");
	}	
}

int CabUI::Execute() {	
	//Initialize variables
	if (Init() < 0) {
		printf("Counldn't initialize CabSDL, exiting.");
		return Error::FAILED_TO_INITIALIZE;
	}
	
	SDL_Event event;
	
	while (running) {
		if (SDL_PollEvent(&event)) {
			OnEvent(&event);
		}
		Render();
		SDL_Delay(1);
	}
	
	Cleanup();
	
	return Error::NO_ERROR;
}

void CabUI::Render() {
	CabDisplay::BlankDisplay(screen);
	RenderGameInfo();
	RenderGameImage();
	RenderDisplayList();
	
	//If necessary, render the CategoryDialog
	if ( (status == SORT) || (status == SEARCH && previous_status == SORT) ) {
		RenderCategoryDialog();
	}
	
	//Flip the screen buffer
	CabDisplay::UpdateDisplay(screen);
}

void CabUI::RenderDisplayList() {
	//Display the list
	CabDisplay::DisplayList(game_display_list, selectedgame, font, screen, x_list, y_list);
}

void CabUI::RenderGameImage() {
	//Display the game image
	SDL_Surface* image = GameImage::GenerateImage(location.GetImages()+game_list.GetGame());
	CabDisplay::DisplayImage(image,screen,x_image,y_image);	
}

void CabUI::RenderGameInfo() {
	bool highlight = (status == SEARCH);
	//Display the game name
	CabDisplay::DisplayText(game_list.GetGame(),font,screen,x_gamename,y_gamename);
	//Display the game category
	CabDisplay::DisplayText(categories.GetCategory(game_list.GetGame()),font,screen,x_category,y_category);
	//Display the search term
	CabDisplay::DisplayText("SEARCH: "+searchterm,font,screen, x_search, y_search,highlight);
}

void CabUI::RenderCategoryDialog() {
	//Get the entries to display
	std::list<std::string> cat_list;
	for (int i = 0; i < category_list_size; i++) {
		cat_list.push_back(available_categories.get());
		available_categories.move(1);
	}
	available_categories.move(0-category_list_size);
	//Display the dialog
	CabDisplay::DisplayCategoryBox(cat_list,selectedcategory,font,screen,screen->w/2, screen->h/2);
}

std::list<std::string> CabUI::CreateDisplayList(int size) {
	std::list<std::string> games_for_list = game_list.GetList(size);
	std::list<std::string> rtn;
	
	//Poll the text to determine the proper width
	int fontheight;
	int fontwidth;
	int widthmax = 0;
	
	for (std::list<std::string>::iterator i = games_for_list.begin(); i != games_for_list.end(); i++) {
		rtn.push_back(descriptions.Name(*i));
		
		//Check the width of the font, finding the greatest value
		if (TTF_SizeText(font, descriptions.Name(*i).c_str(), &fontwidth, &fontheight) == 0) {
			if (fontwidth >= widthmax) {
				widthmax = fontwidth;
			} 
		}		
	}
	
	//Set the image size based on the width of the list
	CabDisplay::SetImageSize(screen->w-x_image-widthmax-10,screen->h-y_image-TTF_FontLineSkip(font)-10);
	return rtn;
}

void CabUI::OnEvent(SDL_Event* event) {
	Event::OnEvent(event);
}

void CabUI::OnExit() {
	running = false;
}

void CabUI::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){
	//Determine which key was pressed
	switch (status) {
		case LIST:
			switch (sym) {
				case SDLK_ESCAPE:
					OnExit();
					break;
				case SDLK_LEFT:
					Move(-1);
					break;
				case SDLK_RIGHT:
					Move(1);
					break;
				case SDLK_UP:
					Move(0-display_list_size);
					break;
				case SDLK_DOWN:
					Move(display_list_size);
					break;
				case SDLK_SPACE:
					Launch();
					break;
				case SDLK_BACKSPACE:
					searchterm = "";
					break;
				case SDLK_TAB:
					previous_status = status;
					status = SORT;
					RenderCategoryDialog();
					break;
				default:
					//Assume search initiated
					previous_status = status;
					status = SEARCH;
					Search(sym);
					break;
			}			
			break;
		case SORT:
			switch (sym) {
				case SDLK_ESCAPE:
					status = LIST;
					break;
				case SDLK_SPACE:
					Sort();
					break;
				case SDLK_LEFT:
					Move(-1);
					break;
				case SDLK_RIGHT:
					Move(1);
					break;
				case SDLK_UP:
					Move(0-display_list_size);
					break;
				case SDLK_DOWN:
					Move(display_list_size);
					break;
				default:
					previous_status = status;
					status = SEARCH;
					Search(sym);
					break;
			}
			break;
		case SEARCH:
			switch (sym) {
				case SDLK_ESCAPE:
				case SDLK_LEFT:
				case SDLK_RIGHT:
				case SDLK_UP:
				case SDLK_DOWN:
					status = previous_status;
					searchterm = "";
					break;
				case SDLK_BACKSPACE:
					searchterm = "";
					break;
				default:
					Search(sym);
					break;
			}
			break;
		default:
			//DO NOTHING
			break;
	}	
}

void CabUI::OnJoyButtonDown(Uint8 which, Uint8 button) {
	switch (status) {
		case LIST:
			if (button == joy_button_sort) {
				previous_status = status;
				status = SORT;
				RenderCategoryDialog();
				break;
			}
			if (button == joy_button_escape) {
				OnExit();
				break;
			}
			if (button == joy_button_select) {
				Launch();
				break;
			}
			break;
		case SORT:
			if (button == joy_button_select) {
				Sort();
				break;
			}
			break;
		default:
			//DO NOTHING
			break;
	}		
}

void CabUI::OnJoyHat(Uint8 which, Uint8 hat, Uint8 value) {
	switch (status) {
		case LIST:
		case SORT:
			switch (hat) {
				case SDL_HAT_LEFT:
					Move(-1);
					break;
				case SDL_HAT_RIGHT:
					Move(1);
					break;
				case SDL_HAT_UP:
					Move(0-display_list_size);
					break;
				case SDL_HAT_DOWN:
					Move(display_list_size);
					break;
			}			
			break;
		case SEARCH:
			switch (hat) {
				case SDL_HAT_LEFT:
				case SDL_HAT_RIGHT:
				case SDL_HAT_UP:
				case SDL_HAT_DOWN:
					status = previous_status;
					break;
			}
			break;
		default:
			//DO NOTHING
			break;
	}
}

void CabUI::OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value){
	if (value < 3200 || value > -3200) return; //In the dead zone
	switch (status) {
		case LIST:
		case SORT:
			switch (axis) {
				case 0: //Left-Right
					//Left
					if (value < 0) {
						Move(-1);
					}
					else {
						Move(1);
					}
					break;
				case 1: //Up-Down
					//Up
					if (value < 0) {
						Move(0-display_list_size);
					}
					else {
						Move(display_list_size);
					}
					break;
			}			
			break;
		case SEARCH:
			status = previous_status;
			break;
		default:
			//DO NOTHING
			break;
	}
}

void CabUI::Move(int distance) {
	switch (status) {
		case LIST: 
		{
			searchterm="";
			game_list.MovePosition(distance);
			switch (distance) {
				case -1:
					if (selectedgame == 0) {
						selectedgame = display_list_size-1;
						game_display_list = CreateDisplayList(0-display_list_size);
					}
					else {
						selectedgame--;
					}
					break;
				case 1:
					if (selectedgame == display_list_size-1) {
						selectedgame = 0;
						game_display_list = CreateDisplayList(display_list_size);
					}
					else {
						selectedgame++;
					}
					break;
				default:
					game_display_list = CreateDisplayList(display_list_size);
					break;
			}
		}
			break;
		case SORT:
			{
				if (abs(distance) > 1) {
					int cat_size = (distance > 1 ? 10:-10);
					selectedcategory=0;
					available_categories.move(cat_size);
				}
				else {
					selectedcategory=0;
					available_categories.move(distance);
				}
			}
			break;
		default:
			//DO NOTHING
			break;
	}
}

void CabUI::Launch() {
	//Reset the search string since a game was launched
	searchterm = "";
	//Switch out of fullscreen
	screen = SDL_SetVideoMode(res_width,res_height, 0, SDL_SWSURFACE);
	//Execute the command-line program
	system(ConstructExecutableCall());
	//Switch back to fullscreen
	screen = SDL_SetVideoMode(res_width,res_height, 0, videoflags);
	//Render the screen elements
	Render();
}

void CabUI::Sort() {
	//Check if Category == ALL
	if ((available_categories.get()).compare("ALL") == 0) {
		game_list.Restore();
		display_list_size = res_height/TTF_FontLineSkip(font)-2;
	}
	else {
		//Filter the game_list keeping only the games that match the selected category
		game_list.Filter(categories.GetMatches(available_categories.get()));
		if (game_list.Size() < display_list_size) {
			display_list_size = game_list.Size();
		} 
		else {
			display_list_size = res_height/TTF_FontLineSkip(font)-2;
		}
	}
	//Update the display list to show the sorted games
	game_display_list = CreateDisplayList(display_list_size);
	status = LIST;	
	selectedcategory = 0;
	selectedgame = 0;
	searchterm = "";
}

void CabUI::Search(Uint16 unicode) {
	//Add the character to the search term
	// If the Unicode value is less than 0x80 then it can be converted to ASCCI text using (char)unicode.
	if( unicode < 0x80 && unicode > 0 ){
		//Add the character to the search term
		searchterm.push_back(tolower((char)unicode));
	}
	switch (previous_status) {
		case LIST:
		{
			//Search the descriptions to locate the closes match
			std::string closest_game_name = descriptions.Search(searchterm);			
			//Search the game list to find the closest match
			game_list.Search(closest_game_name);
			//Update the display list to show the closest match
			game_display_list = CreateDisplayList(display_list_size);
			selectedgame = 0;
		}
			break;
		case SORT:
			//Search the categories list to find the closest match
			available_categories.search(searchterm);
			break;
		default:
			break;
	}	
}

const char* CabUI::ConstructExecutableCall() {
	string call = "";
	//First is the command
	call+=location.GetCommand()+" ";
	//Add any parameters
	call+=executable_arguments+" ";
	//Add the game name to the end of the call
	call+=game_list.GetGame()+".zip";
	return call.c_str();
}

int CabUI::FileExists(const char* path) {
	ifstream check_file(path);
	if (!check_file.good()) {
		//File does not exist
		check_file.close();
		return Error::FILE_DOES_NOT_EXIST;
	}
	check_file.close();
	return Error::NO_ERROR;
}

int CabUI::Init() {
	// Initial the SDL_TTF
	if (TTF_Init() < 0)
	{
		std::cerr<<"Couldn't initialize SDL TTF: "<<SDL_GetError()<<std::endl;
		exit(1);
	}	
	
	//Process the config file
	if (ProcessConfigFile() != Error::NO_ERROR) {
		std::cerr<<"Invalid Config file"<<std::endl;
		return Error::INIT_FAILED;
	}
	
	//Setup the positioning of the UI elements based on the resolution
	int fontheight = TTF_FontLineSkip(font);
	x_image = res_width/50;
	y_image = res_height/50;
	x_list = res_width-5;
	y_list = 5;
	x_gamename = res_width/50;
	y_gamename = res_height-5-fontheight;
	x_category = res_width/50*8;
	y_category = res_height-5-fontheight;
	x_search = res_width/50*20;
	y_search = res_height-5-fontheight;
	display_list_size = res_height/TTF_FontLineSkip(font)-2;
	
	//Ensure that the size of the game list does not exceed the list size
	game_list.Initialize(location.GetGames());
	if (game_list.Size() < display_list_size) {
		//limit display list to the size of the game list
		display_list_size = game_list.Size();
	}
	
	//Initialize the list of available categories
	available_categories.set(categories.List());
	
	//Dynamically size dialog box based on largest string
	CabDisplay::DetermineDialogSize(available_categories.list(), font);
	
	/* Set video mode */
	Uint8  video_bpp = 0;	//Use the current display bitsperpixel
	screen=SDL_SetVideoMode(res_width,res_height, video_bpp, videoflags);
	if (screen == NULL) {
		std::cerr<<"Couldn't set "<<video_bpp<<" video mode: "<<SDL_GetError()<<std::endl;
		SDL_Quit();
		exit(2);	//Exit the program with code 2
	}
	
	//Initialize the game_display_list
	game_display_list = CreateDisplayList(display_list_size);
	
	//Enable running switch
	running = true;
	
	//Display the components
	Render();
	
	return Error::NO_ERROR;
}

int CabUI::ProcessConfigFile() {
	//Create a new ConfigFile object to read the configuration
	ConfigFile* cfg = NULL;
	
	//Check if the default config file exists
	if (FileExists(CabSDL::CONFIG_PATH) == Error::FILE_DOES_NOT_EXIST) {
		//Check the local path
		if (FileExists(CabSDL::CONFIG_PATH_LOCAL) == Error::FILE_DOES_NOT_EXIST) {
			//No valid condif file found
			std::cerr<<"No valid config file found at "<<CabSDL::CONFIG_PATH<<" or "<<CabSDL::CONFIG_PATH_LOCAL<<std::endl;
			return Error::NO_CONFIG;
		}
		else {
			cfg = new ConfigFile(CabSDL::CONFIG_PATH_LOCAL);
		}
	}
	else {
		cfg = new ConfigFile(CabSDL::CONFIG_PATH);
	}
	
	//Validate the values from the config file
	if ( location.SetGames(cfg->getvalue<string>("game_path")) == Error::EMPTY_STRING) {
		location.SetGames(CabSDL::GAME_PATH); //Set to default
	}
	if ( location.SetFont(cfg->getvalue<string>("font_path")) == Error::EMPTY_STRING) {
		location.SetFont(CabSDL::FONT); //Set to default
	}
	if ( location.SetCommand(cfg->getvalue<string>("command")) == Error::EMPTY_STRING) {
		location.SetCommand(CabSDL::PROGRAM); //Set to default
	}
	if ( location.SetImages(cfg->getvalue<string>("image_path")) == Error::EMPTY_STRING) {
		location.SetImages(CabSDL::IMAGE_PATH); //Set to default
	}
	
	//Ensure that the specified locations exist
	if (!FileExists(location.GetGames().c_str()) == Error::FILE_DOES_NOT_EXIST) {
		std::cerr<<"ERROR; Game directory"<<location.GetGames()<<" does not exist"<<std::endl;
		return Error::FILE_DOES_NOT_EXIST;
	}
	if (!FileExists(location.GetFont().c_str()) == Error::FILE_DOES_NOT_EXIST) {
		std::cerr<<"ERROR; Font file "<<location.GetFont()<<" does not exist"<<std::endl;
		return Error::FILE_DOES_NOT_EXIST;
	}
	if (!FileExists(location.GetCommand().c_str()) == Error::FILE_DOES_NOT_EXIST) {
		std::cerr<<"ERROR; Command file "<<location.GetCommand()<<" does not exist"<<std::endl;
		return Error::FILE_DOES_NOT_EXIST;
	}
	if (!FileExists(location.GetImages().c_str()) == Error::FILE_DOES_NOT_EXIST) {
		std::cerr<<"ERROR; Images directory "<<location.GetImages()<<" does not exist"<<std::endl;
		return Error::FILE_DOES_NOT_EXIST;
	}
	
	//Set joystick bindings
	if (int jselect = cfg->getvalue<int>("joy_select")) {
		joy_button_select = jselect;
	}
	if (int jsort = cfg->getvalue<int>("joy_sort")) {
		joy_button_sort = jsort;
	}
	if (int jescape = cfg->getvalue<int>("joy_escape")) {
		joy_button_escape = jescape;
	}
	
	//Check if mamedb.org downloading should be disabled
	std::string mamedbswitch = cfg->getvalue<std::string>("mamedb.org");
	if (mamedbswitch.compare("") != 0) {
		if (mamedbswitch.compare("true") == 0) {
			MameDB::Enable();
		}
	}

	//Load the SDL font
	int fontsize = cfg->getvalue<int>("font_size");
	font = TTF_OpenFont((char*)location.GetFont().c_str(),(fontsize > 0 ? fontsize:16));
	if (!font) {
		std::cerr<<"TTF_OpenFont: "<<TTF_GetError()<<std::endl;
		exit(1);
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
	
	//Check if there are any parameters for the executable
	int index = 0;
	//Convert index to a string
	std::ostringstream sin;
	sin<<index;
	string arg = "param"+sin.str();
	string param = cfg->getvalue<string>(arg);
	//Get all of the parameters from the config
	while (param != "") {
		executable_arguments+=param+" ";
		arg = "value"+sin.str();
		//Check if there is an associated value
		string value = cfg->getvalue<string>(arg);
		if (value != "") {
			executable_arguments+=value+" ";
		}
		index++;
		sin.str("");
		sin<<index;
		arg = "param"+sin.str();
		param = cfg->getvalue<string>(arg);
	}	
	
	//Delete the pointer
	if (cfg != NULL) {
		delete(cfg);
	}
	
	//Everything loaded successfully
	return Error::NO_ERROR;
}

int CabUI::Cleanup() {
	//Free the screen
	SDL_FreeSurface(screen);
	//Make sure the game_chooser font is closed
	TTF_CloseFont(font);
	TTF_Quit();
	//If the joystick is opened, close it
	if (SDL_JoystickOpened(0)) {
		SDL_JoystickClose(joy);
	}
	/* Clean up the SDL library */
	SDL_Quit();
	return Error::NO_ERROR; //No error
}
