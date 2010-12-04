/*
 *  mamedb.cpp
 *  CabSDL
 *
 *  Created by ickymachine on 12/2/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#include "mamedb.h"
#include <iostream>
#include <sstream>

using namespace std;

struct MemoryStruct {
	char* memory;
	size_t size;
};

/*
 * Downloads a remote image from mamedb.com and saves it to the rom image path
 *
 * @return void
 */
void MameDB::DownloadImage(const string& file_name) {
	string game_name;
	game_name = file_name.substr(file_name.find_last_of("/")+1);
	
	CURL *curl;
	FILE *fp;
	CURLcode res;
	string url = "http://www.mamedb.org/images/rom/titles/"+game_name+".png";
	string outfilename = file_name+".png";
	//	cout<<"INFO; attempting to download "<<url<<"..."<<endl;
	curl = curl_easy_init();
	if (curl) {
		//		cout<<"INFO; downloading "<<url<<"..."<<endl;
		fp = fopen(outfilename.c_str(), "wb");
		//		cout<<"INFO; trying to open "<<outfilename<<" for file output"<<endl;
		if (fp != NULL) {
			curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, MameDB::WriteFile);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
			//			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			res = curl_easy_perform(curl);
			
			long http_code = 0;
			curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
			
			// If the transfer was bad, delete the crap file
			if ( ! (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK))
			{
				remove(outfilename.c_str());
				cout<<"MameDB::DownloadImage; Couldn't retrieve file for download."<<endl;
			}
			
			curl_easy_cleanup(curl);
			fclose(fp);
		}
		else {
			cout<<"MameDB::DownloadImage; Couldn't open output file"<<endl;
		}
	}
}

size_t MameDB::WriteFile(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}
