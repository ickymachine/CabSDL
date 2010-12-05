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

void CabUI::EnterSearchTerm(SDL_KeyboardEvent* key) {
	if( key->type == SDL_KEYDOWN ){
		// If the Unicode value is less than 0x80 then it can be converted to ASCCI text using (char)unicode.
		if( key->keysym.unicode < 0x80 && key->keysym.unicode > 0 ){
			//Add the character to the search term
			searchterm.push_back((char)key->keysym.unicode);
		}
	}
}

int CabUI::UpdateDisplayList(SDL_keysym* key) {
	//Determine the proper list size
	int size = display_list_size;
	int small = game_list.Size();
	if (small < display_list_size) size = small;
	
	//Check to see if list needs to be updated
	if (selectedgame == 0 && key->sym == SDLK_LEFT) {
		game_display_list = CreateDisplayList(-size);
		selectedgame = size-1;
	}
	else if (selectedgame == size-1 && key->sym == SDLK_RIGHT) {
		game_display_list = CreateDisplayList(size);
		selectedgame = 0;
	}
	else if (key->sym == SDLK_UP || key->sym == SDLK_DOWN) {
		game_display_list = CreateDisplayList(size);
		selectedgame = 0;
	}
	else if (key->sym == SDLK_RIGHT) {
		selectedgame++;
	}
	else if (key->sym == SDLK_LEFT) {
		selectedgame--;
	}
	else {
		//Must have been a search
		selectedgame = 0;
		game_display_list = CreateDisplayList(size);
	}
	return 0;
}

void CabUI::Update() {
	switch (status) {
		case LIST: {
			CabDisplay::BlankDisplay(screen);
			//Display the game image
			std::string image_path = location.GetImages()+game_list.GetGame();
			SDL_Surface* image = GameImage::GenerateImage(image_path);
			image = GameImage::ScaleImage(image, static_cast<int>(res_width*(0.70)), static_cast<int>(res_height*(0.90)));
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
					case LIST: {
						searchterm = "";
						//Switch out of fullscreen
						screen = SDL_SetVideoMode(res_width,res_height, 0, SDL_SWSURFACE);
						//Execute the command-line program
						system(ConstructExecutableCall());
						//Switch back to fullscreen
						screen = SDL_SetVideoMode(res_width,res_height, 0, videoflags);
					}
						break;
					case SORT:{
						//Sort games by category
						list<string> available_categories;
						available_categories = categories.List();
						list<string>::iterator it = available_categories.begin();
						//Move to the correct category
						int dir = (selectedcategory > 0 ? 1:-1);
						int vel = abs(selectedcategory);
						for (int i = 0; i < vel; i++) {
							if (dir == 1) {
								if (it != available_categories.end()) {
									it++;
								}
								else {
									it = available_categories.begin();
								}
							}
							else {
								if (it != available_categories.begin()) {
									it--;
								}
								else {
									it = available_categories.end();
									it--;
								}
							}
						}
						//Check if Category == ALL
						if (it->compare("ALL") == 0) {
							game_list.Restore();
						}
						else {
							game_list.Filter(categories.GetMatches(*it));
							list<string> check_list = game_list.GetList();
						}
					}
						game_display_list = CreateDisplayList(display_list_size);
						status = LIST;	
						selectedcategory = 0;
						selectedgame = 0;
						searchterm = "";
						break;
					default:
						break;
				}
				Update();				
			}
			else if (event->jbutton.button == joy_button_sort) {
				status = SORT;
				Update();
			}
			break;
		case SDL_JOYAXISMOTION:
			//Detemine 
			if ( (event->jaxis.value < -3200) || (event->jaxis.value > 3200) ) {
				if (event->jaxis.axis == 0) {
					//Left/Right
					if (event->jaxis.value > 0) {
						//Right
						switch (status) {
							case LIST:
								game_list.MovePosition(1);
								searchterm = "";
								UpdateDisplayList(&event->key.keysym);
								break;
							case SORT:
								selectedcategory++;
								break;
							default:
								break;
						}
						Update();
					}
					else {
						//Left
						switch (status) {
							case LIST:
								game_list.MovePosition(-1);
								searchterm = "";
								UpdateDisplayList(&event->key.keysym);
								break;
							case SORT:
								selectedcategory--;
								break;
							default:
								cout<<"DO NOTHING"<<endl;
								break;
						}
						Update();						
					}
				}
				if (event->jaxis.axis == 1) {
					//Up/Down
					if (event->jaxis.value < 0) {
						//UP
						switch (status) {
							case LIST:
								game_list.MovePosition(-display_list_size);
								searchterm = "";
								UpdateDisplayList(&event->key.keysym);
								break;
							case SORT:
								break;
							default:
								break;
						}
						Update();
					}
					else {
						//Down
						switch (status) {
							case LIST:
								game_list.MovePosition(display_list_size);
								searchterm = "";
								UpdateDisplayList(&event->key.keysym);
								break;
							default:
								break;
						}
						Update();						
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
	return 0;
}

int CabUI::HandleKeypress(SDL_Event* event) {
	//Determine which key was pressed
	switch (event->key.keysym.sym) {
		case SDLK_ESCAPE:
			return 1;
			break;
		case SDLK_LEFT:
			switch (status) {
				case LIST:
					game_list.MovePosition(-1);
					searchterm = "";
					UpdateDisplayList(&event->key.keysym);
					break;
				case SORT:
					selectedcategory--;
					break;
				default:
					cout<<"DO NOTHING"<<endl;
					break;
			}
			Update();
			break;
		case SDLK_RIGHT:
			switch (status) {
				case LIST:
					game_list.MovePosition(1);
					searchterm = "";
					UpdateDisplayList(&event->key.keysym);
					break;
				case SORT:
					selectedcategory++;
					break;
				default:
					break;
			}
			Update();
			break;
		case SDLK_UP:
			switch (status) {
				case LIST:
					game_list.MovePosition(-display_list_size);
					searchterm = "";
					UpdateDisplayList(&event->key.keysym);
					break;
				case SORT:
					break;
				default:
					break;
			}
			Update();
			break;
		case SDLK_DOWN:
			switch (status) {
				case LIST:
					game_list.MovePosition(display_list_size);
					searchterm = "";
					UpdateDisplayList(&event->key.keysym);
					break;
				default:
					break;
			}
			Update();
			break;
		case SDLK_SPACE:
			switch (status) {
				case LIST: {
					searchterm = "";
					//Switch out of fullscreen
					screen = SDL_SetVideoMode(res_width,res_height, 0, SDL_SWSURFACE);
					//Execute the command-line program
					system(ConstructExecutableCall());
					//Switch back to fullscreen
					screen = SDL_SetVideoMode(res_width,res_height, 0, videoflags);
				}
					break;
				case SORT:{
					//Sort games by category
					list<string> available_categories;
					available_categories = categories.List();
					list<string>::iterator it = available_categories.begin();
					//Move to the correct category
					int dir = (selectedcategory > 0 ? 1:-1);
					int vel = abs(selectedcategory);
					for (int i = 0; i < vel; i++) {
						if (dir == 1) {
							if (it != available_categories.end()) {
								it++;
							}
							else {
								it = available_categories.begin();
							}
						}
						else {
							if (it != available_categories.begin()) {
								it--;
							}
							else {
								it = available_categories.end();
								it--;
							}
						}
					}
					//Check if Category == ALL
					if (it->compare("ALL") == 0) {
						game_list.Restore();
					}
					else {
						game_list.Filter(categories.GetMatches(*it));
						list<string> check_list = game_list.GetList();
					}
				}
					game_display_list = CreateDisplayList(display_list_size);
					status = LIST;	
					selectedcategory = 0;
					selectedgame = 0;
					searchterm = "";
					break;
				default:
					break;
			}
			Update();
			break;
		case SDLK_BACKSPACE:
			searchterm = "";
			Update();
			break;
		case SDLK_TAB:
			status = SORT;
			Update();
			break;
		default:
			//Assume search initiated
			switch (status) {
				case LIST:
					EnterSearchTerm(&event->key);
					game_list.Search(searchterm);
					UpdateDisplayList(&event->key.keysym);					
					break;
				case SORT:
	//				EnterSearchTerm(&event->key);
					break;
				default:
					break;
			}
			Update();
			break;
	}
	return 0;
}

int CabUI::ProcessConfigFile() {
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
	joy_button_select = cfg->getvalue<int>("joy_select");
	joy_button_sort = cfg->getvalue<int>("joy_sort");
	
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
	
	searchterm = "";
	selectedgame = 0;
	selectedcategory = 0;
	game_display_list = CreateDisplayList(display_list_size);
	status = LIST;
	
	std::list<std::string>* pass = new std::list<std::string>(categories.List());
	CabDisplay::DetermineDialogSize(pass, font);
	delete(pass);
	
	Uint32 initflags = SDL_INIT_VIDEO|SDL_INIT_JOYSTICK;  /* See documentation for details */
	Uint8  video_bpp = 0;
	
	/* Initialize the SDL library */
	if ( SDL_Init(initflags) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",
				SDL_GetError());
		exit(1);
	}
	
	/* Set 640x480 video mode */
	screen=SDL_SetVideoMode(res_width,res_height, video_bpp, videoflags);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set %d video mode: %s\n",
				video_bpp, SDL_GetError());
		SDL_Quit();
		exit(2);
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
	return 0;
}

std::list<std::string> CabUI::CreateDisplayList(int size) {
	std::list<std::string> games_for_list = game_list.GetList(size);
	std::list<std::string> rtn;
	for (std::list<std::string>::iterator i = games_for_list.begin(); i != games_for_list.end(); i++) {
		rtn.push_back(descriptions.Name(*i));
	}
	return rtn;
}
