/*
 *  gameimage.cpp
 *  CabGL
 *
 *  Created by Matthew Capodarco on 11/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#define CURL_STATICLIB
#include "gameimage.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "SDL_image.h"
#include "SDL.h"
#include <string>
#include <math.h>
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#include <fstream>

const int scale_x_res = 320;
const int scale_y_res = 240;

using namespace std;
/*
GameImage::GameImage() {
	
}

GameImage::~GameImage() {
	
}
*/
 
SDL_Surface* GameImage::GenerateImage(string game_name) {
	//Create an SDL_Surface* to return
	SDL_Surface* image = NULL;
	//Locate the proper image
	string filename = game_name+".png";

	// See if the image exists
	ifstream game_image(filename.c_str());
	if ( ! game_image.good()) {
		GameImage::DownloadImage(game_name);
	}
	game_image.close();

	//Load the desired image
	image = IMG_Load((char*)filename.c_str());
	//check if the image loading failed
	if (image == NULL) {
		//Load and display default image instead
		cout<<"ERROR; GameImage::GenerateImage; Couldn't load image at "<<filename<<endl;
		image = IMG_Load("default.png");
		if (image == NULL) {
			//couldn't load default for some reason
			cout<<"ERROR; GameImage::Generate; Couldn't load default.png"<<endl;
		}
	}
	return image;
}

SDL_Surface* GameImage::ScaleImage(SDL_Surface* image, int width, int height) {
	//If the image surface is large add pixel skipping, default is none
	int skipx = 1;
	int skipy = 1;
	if (image->w > scale_x_res) skipx = image->w/scale_x_res;
	if (image->h > scale_y_res) skipy = image->h/scale_y_res;
	
	if (!width || !height || image->w == 0 || image->h == 0) {
		cout<<"ERROR; GameImage::ScaleImage; Size must be greater than 0"<<endl;
		return NULL;
	}
	//Create the scaled image using the format of the original
	SDL_Surface* scaled = SDL_CreateRGBSurface(image->flags, width, height, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, image->format->Amask);
	double stretchx = (static_cast<double>(width)/static_cast<double>(image->w));
	double stretchy = (static_cast<double>(height)/static_cast<double>(image->h));
	SDL_Rect dest;
	SDL_Color pix;
	
	//Scale down both dimensions
	if (stretchx < 1 && stretchy < 1) {
		int targetx = 0;
		int targety = 0;
		dest.w = skipx;
		dest.h = skipy;
		//Determine scaling factors
		double xinc = static_cast<double>(image->w)/static_cast<double>(width);
		double yinc = static_cast<double>(image->h)/static_cast<double>(height);
		//Loop through the scaled image dimensions
		for (int y = 0; y < height; y+=skipy) {
			dest.y = y;
			for (int x = 0; x < width; x+=skipx) {
				dest.x = x;
				targetx = static_cast<double>(x)*xinc;
				if (targetx >= image->w) {
					targetx = image->w-1;
				}
				pix = GameImage::GetPixel(image,targetx,targety);
				SDL_FillRect(scaled, &dest, SDL_MapRGB(image->format, pix.r, pix.g, pix.b));
			}
			targetx = 0;
			targety = static_cast<double>(y)*yinc;
			//check to see if out of bounds
			if (targety >= image->h) {
				targety = image->h-1;
			}
		}
	}
	//Scale down x, scale up y
	else if (stretchx < 1) {
		int targetx = 0;
		double xinc = static_cast<double>(image->w)/static_cast<double>(width);
		dest.w = skipx;
		for (int y = 0; y < image->h; y+=skipy) {
			dest.y = (static_cast<double>(y)*stretchy);
			dest.h = (static_cast<double>(y+1)*stretchy)*skipx;
			for (int x = 0; x < width; x+=skipx) {
				dest.x = x;
				pix = GameImage::GetPixel(image,targetx,y);
				SDL_FillRect(scaled, &dest, SDL_MapRGB(image->format, pix.r, pix.g, pix.b));
				targetx = static_cast<double>(x)*xinc;
				if (targetx >= image->w) {
					targetx = image->w-1;	
				}
			}
			targetx = 0;		
		}
	}
	//Scaule up x, scale down y
	else if (stretchy < 1) {
		int targety = 0;
		double yinc = static_cast<double>(image->h)/static_cast<double>(height);
		dest.h = skipy;
		for (int y = 0; y < height; y+=skipy) {
			dest.y = y;
			for (int x = 0; x < image->w; x+=skipx) {
				dest.x = (static_cast<double>(x)*stretchx);
				dest.w = (static_cast<double>(x+1)*stretchx)*skipx;
				pix = GameImage::GetPixel(image,x,targety);
				SDL_FillRect(scaled, &dest, SDL_MapRGB(image->format, pix.r, pix.g, pix.b));
			}
			targety = static_cast<double>(y)*yinc;
			//check to see if out of bounds
			if (targety >= image->h) {
				targety = image->h-1;
			}
		}
	}
	//Scale up both dimensions
	else {
		//Loop across the original image, drawing the scaled pixels
		for (int y = 0; y < image->h; y+=skipy) {
			dest.y = (static_cast<double>(y)*stretchy);
			dest.h = (static_cast<double>(y+1)*stretchy)*skipy;
			for (int x = 0; x < image->w; x+=skipx) {
				dest.x = (static_cast<double>(x)*stretchx);
				dest.w = (static_cast<double>(x+1)*stretchx)*skipx;
				pix = GameImage::GetPixel(image,x,y);
				SDL_FillRect(scaled, &dest, SDL_MapRGB(image->format, pix.r, pix.g, pix.b));
				//cout<<dest.x<<" "<<dest.y<<endl;
			}
		}
	}
	//set the image to the scaled result
	return scaled;
}

SDL_Color GameImage::GetPixel(SDL_Surface* image, int x, int y) {
	//Lock the image before inspecting the pixels pointer
	SDL_LockSurface(image);
	SDL_Color color;
	Uint32 col = 0;
	char* position = (char*)image->pixels;
	//Move to the proper x,y coordinate
	position += (image->pitch * y);
	position += (image->format->BytesPerPixel * x);
	//Get the pixel data
	//copy pixel data
	memcpy (&col, position, image->format->BytesPerPixel);
	//Convert color
	SDL_GetRGB(col, image->format, &color.r, &color.g, &color.b);
	//Unlock the image
	SDL_UnlockSurface(image);
	return color;
}

/*
 * Downloads a remote image from mamedb.com and saves it to the rom image path
 *
 * @return void
 */
void GameImage::DownloadImage(string file_name) {
	string game_name;
	game_name = file_name.substr(file_name.find_last_of("/")+1);

	CURL *curl;
	FILE *fp;
	CURLcode res;
	string url = "http://www.mamedb.com/titles/"+game_name+".png";
	string outfilename = file_name+".png";
	cout<<"INFO; attempting to download "<<url<<"..."<<endl;
	curl = curl_easy_init();
	if (curl) {
		cout<<"INFO; downloading "<<url<<"..."<<endl;
		fp = fopen(outfilename.c_str(), "wb");
		cout<<"INFO; trying to open "<<outfilename<<" for file output"<<endl;
		if (fp != NULL) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, GameImage::WriteData);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			res = curl_easy_perform(curl);

			long http_code = 0;
			curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

			// If the transfer was bad, delete the crap file
			if ( ! (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK))
			{
				remove(outfilename.c_str());
			}

			curl_easy_cleanup(curl);
			fclose(fp);
		}
		else {
			cout<<"GameImage::DownloadImage; Couldn't open output file"<<endl;
		}
	}
}

size_t GameImage::WriteData(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}
