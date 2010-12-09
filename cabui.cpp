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
	//Video Mode
	videoflags = SDL_SWSURFACE;
	
	//Default resolution is 640x480
	res_width = 480;
	res_height = 640;
	
	//Joystick defaults
	joy_button_select = 0;
	joy_button_sort = 0;
}

int CabUI::FileExists(const char* path) {
	ifstream check_file(path);
	if (!check_file.good()) {
		//File does not exist
		check_file.close();
		return 0;
	}
	check_file.close();
	return 1;
}

const char* CabUI::ConstructExecutableCall() {
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
	//Get all of the parameters from the config
	while (param != "") {
		call+=param+" ";
		arg = "value"+sin.str();
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

std::string CabUI::EnterText(SDL_KeyboardEvent* key) {
	std::string rtn;
	if( key->type == SDL_KEYDOWN ){
		// If the Unicode value is less than 0x80 then it can be converted to ASCCI text using (char)unicode.
		if( key->keysym.unicode < 0x80 && key->keysym.unicode > 0 ){
			//Add the character to the search term
			rtn.push_back((char)key->keysym.unicode);
		}
	}
	return rtn;
}

int CabUI::UpdateDisplayList(SDL_keysym* key) {
	//Determine the proper list size
	int size = display_list_size;
	int small = game_list.Size();
	if (small < display_list_size) size = small;
	
	//Check to see if list needs to be updated
	
	//Beginning of list and hit left, move to the bottom of the list of previous items
	if (selectedgame == 0 && key->sym == SDLK_LEFT) {
		game_display_list = CreateDisplayList(-size);
		selectedgame = size-1;
	}
	//End of list and hit right, move to top of list of next items
	else if (selectedgame == size-1 && key->sym == SDLK_RIGHT) {
		game_display_list = CreateDisplayList(size);
		selectedgame = 0;
	}
	//Skip to the next page
	else if (key->sym == SDLK_UP || key->sym == SDLK_DOWN) {
		game_display_list = CreateDisplayList(size);
		selectedgame = 0;
	}
	//Move 1 down the list
	else if (key->sym == SDLK_RIGHT) {
		selectedgame++;
	}
	//Move 1 up the list
	else if (key->sym == SDLK_LEFT) {
		selectedgame--;
	}
	//User entered text as search
	else {
		//Must have been a search
		selectedgame = 0;
		game_display_list = CreateDisplayList(size);
	}
	return 0;	//No error
}

void CabUI::Update() {
	switch (status) {
		case LIST: {
			CabDisplay::BlankDisplay(screen);
			//Display the game image
			std::string image_path = location.GetImages()+game_list.GetGame();
			SDL_Surface* image = GameImage::GenerateImage(image_path);
			CabDisplay::DisplayImage(image,screen,x_image,y_image);
			//Display the game name
			CabDisplay::DisplayText(game_list.GetGame(),font,screen,x_gamename,y_gamename);
			//Display the game category
			CabDisplay::DisplayText(categories.GetCategory(game_list.GetGame()),font,screen,x_category,y_category);
			//Display the search term
			CabDisplay::DisplayText("SEARCH: "+searchterm,font,screen, x_search, y_search);
			//Display the list of games
			CabDisplay::DisplayList(game_display_list,selectedgame,font,screen, x_list, y_list);			
			break;
		}
		case SORT:
			//Display the dialog
			{
			std::list<std::string>* pass = new std::list<std::string>(categories.List());
			CabDisplay::DisplayCategoryBox(pass,selectedcategory,font,screen,screen->w/2, screen->h/2);
			delete(pass);
			}
			break;
		default:
			break;
	}
	//Draw the screen
	CabDisplay::UpdateDisplay(screen);
}

int CabUI::HandleJoystick(SDL_Event* event) {
	//Determine what type of Joystick event occured
	switch (event->type) {
		case SDL_JOYBUTTONDOWN:
			//Determine action based on the button pressed
			if (event->jbutton.button == joy_button_select) {
				switch (status) {
					case LIST: 
						Launch();
						break;
					case SORT:
						Sort();
						break;
					default:
						break;
				}
			}
			else if (event->jbutton.button == joy_button_sort) {
				status = SORT;
			}
			break;
		case SDL_JOYAXISMOTION:
			//Detemine 
			if ( (event->jaxis.value < -3200) || (event->jaxis.value > 3200) ) {
				if (event->jaxis.axis == 0) {
					//Left/Right
					if (event->jaxis.value > 0) {
						//Right
						Move(event,1);
					}
					else {
						//Left
						Move(event,0-1);						
					}
				}
				if (event->jaxis.axis == 1) {
					//Up/Down
					if (event->jaxis.value < 0) {
						//UP
						Move(event,0-display_list_size);
					}
					else {
						//Down
						Move(event,display_list_size);					
					}
				}
			}  
			break;
		case SDL_JOYHATMOTION:
			//DO NOTHING FOR NOW
			break;
		default:
			break;
	}
	Update();
	return 0;	//No error
}

void CabUI::Move(SDL_Event* event, int distance) {
	switch (status) {
		case LIST:
			game_list.MovePosition(distance);
			searchterm="";
			UpdateDisplayList(&event->key.keysym);
			break;
		case SORT:
			{
				int cat_size = (distance >= 0 ? 10:-10);
				selectedcategory+=cat_size;
			}
			break;
		default:
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
}

void CabUI::Sort() {
	//Sort games by category
	wlist available_categories;
	available_categories.set(categories.List());
	//Move to the correct category
	available_categories.move(selectedcategory);
	//Check if Category == ALL
	if ((available_categories.get()).compare("ALL") == 0) {
		game_list.Restore();
	}
	else {
		//Filter the game_list keeping only the games that match the selected category
		game_list.Filter(categories.GetMatches(available_categories.get()));
	}
	//Update the display list to show the sorted games
	game_display_list = CreateDisplayList(display_list_size);
	status = LIST;	
	selectedcategory = 0;
	selectedgame = 0;
	searchterm = "";
}

void CabUI::Search(SDL_Event* event) {
	switch (status) {
		case LIST:
			//Add the character to the search term
			searchterm += EnterText(&event->key);
			//Search the game list to find the closest match
			game_list.Search(searchterm);
			UpdateDisplayList(&event->key.keysym);					
			break;
		case SORT:
			//Do nothing for now
			break;
		default:
			break;
	}	
}
						 
int CabUI::HandleKeypress(SDL_Event* event) {
	//Determine which key was pressed
	switch (event->key.keysym.sym) {
		case SDLK_ESCAPE:
			return 1;
			break;
		case SDLK_LEFT:
			Move(event,0-1);
			break;
		case SDLK_RIGHT:
			Move(event,1);
			break;
		case SDLK_UP:
			Move(event,0-display_list_size);
			break;
		case SDLK_DOWN:
			Move(event,display_list_size);
			break;
		case SDLK_SPACE:
			switch (status) {
				case LIST: 
					Launch();
					break;
				case SORT:
					Sort();
					break;
				default:
					break;
			}
			break;
		case SDLK_BACKSPACE:
			searchterm = "";
			break;
		case SDLK_TAB:
			status = SORT;
			break;
		default:
			//Assume search initiated
			Search(event);
			break;
	}
	Update();
	return 0;
}

int CabUI::ProcessConfigFile() {
	//Check if the default config file exists
	if (!FileExists(CabSDL::CONFIG_PATH)) {
		//Check the local path
		if (!FileExists(CabSDL::CONFIG_PATH_LOCAL)) {
			//No valid condif file found
			cout<<"ERROR; No valid config file found at "<<CabSDL::CONFIG_PATH<<" or "<<CabSDL::CONFIG_PATH_LOCAL<<endl;
			return 1;
		}
		else {
			cfg = new ConfigFile(CabSDL::CONFIG_PATH_LOCAL);
		}
	}
	else {
		cfg = new ConfigFile(CabSDL::CONFIG_PATH);
	}
	//	cfg->dump();
	
	//Validate the values from the config file
	if ( location.SetGames(cfg->getvalue<string>("game_path")) == -1) {
		location.SetGames(CabSDL::GAME_PATH); //Set to default
	}
	if ( location.SetFont(cfg->getvalue<string>("font_path")) == -1) {
		location.SetFont(CabSDL::FONT); //Set to default
	}
	if ( location.SetCommand(cfg->getvalue<string>("command")) == -1) {
		location.SetCommand(CabSDL::PROGRAM); //Set to default
	}
	if ( location.SetImages(cfg->getvalue<string>("image_path")) == -1) {
		location.SetImages(CabSDL::IMAGE_PATH); //Set to default
	}
	
	//Ensure that the specified locations exist
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
	
	//Set joystick bindings
	if (int jselect = cfg->getvalue<int>("joy_select")) {
		joy_button_select = jselect;
	}
	if (int jsort = cfg->getvalue<int>("joy_sort")) {
		joy_button_sort = jsort;
	}
	
	//Check if mamedb.org downloading should be disabled
	std::string mamedbswitch = cfg->getvalue<std::string>("mamedb.org");
	if (mamedbswitch.compare("") != 0) {
		if (mamedbswitch.compare("true") == 0) {
			MameDB::Enable();
		}
	}

	//Everything loaded successfully
	return 1;
}

int CabUI::Init() {
	//Process the config file
	if (!ProcessConfigFile()) {
		cout<<"ERROR; Invalid Config file"<<endl;
		return -1;
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
	
	//Setup the positioning of the UI elements based on the resolution
	x_image = res_width/50;
	y_image = res_height/50;
	x_list = res_width-5;
	y_list = 5;
	x_gamename = res_width/10;
	y_gamename = res_height-fontsize-5;
	x_category = res_width/10*3;
	y_category = res_height-fontsize-5;
	x_search = res_width/10*7;
	y_search = res_height-fontsize-5;
	display_list_size = res_height/TTF_FontLineSkip(font)-2;
	
	//Ensure that the size of the game list does not exceed the list size
	game_list.Initialize(location.GetGames());
	if (game_list.Size() < display_list_size) {
		//limit display list to the size of the game list
		display_list_size = game_list.Size();
	}
	
	//Empty the searchterm, initialize the currently selected game and categories, set mode to LIST
	searchterm = "";
	selectedgame = 0;
	selectedcategory = 0;
	status = LIST;
	
	//Dynamically size dialog box based on largest string
	std::list<std::string>* pass = new std::list<std::string>(categories.List());
	CabDisplay::DetermineDialogSize(pass, font);
	delete(pass);
	
	//Initialize SDL to support video and joystick
	Uint32 initflags = SDL_INIT_VIDEO|SDL_INIT_JOYSTICK;  /* See documentation for details */
	Uint8  video_bpp = 0;	//Use the current display bitsperpixel
	
	/* Initialize the SDL library */
	if ( SDL_Init(initflags) < 0 ) {
		std::cerr<<"Couldn't initialize SDL: "<<SDL_GetError()<<std::endl;
		exit(1);	//Exit the program with code 1
	}
	
	/* Set 640x480 video mode */
	screen=SDL_SetVideoMode(res_width,res_height, video_bpp, videoflags);
	if (screen == NULL) {
		std::cerr<<"Couldn't set "<<video_bpp<<" video mode: "<<SDL_GetError()<<std::endl;
		SDL_Quit();
		exit(2);	//Exit the program with code 2
	}
	
	//Create the initial display list
	game_display_list = CreateDisplayList(display_list_size);
	
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
	return 0;
}

int CabUI::Cleanup() {
	//Cleanup pointers
	delete cfg;
	//Make sure the game_chooser font is closed
	TTF_CloseFont(font);
	TTF_Quit();
	//If the joystick is opened, close it
	if (SDL_JoystickOpened(0)) {
		SDL_JoystickClose(joy);
	}
	return 0; //No error
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
