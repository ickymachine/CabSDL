/*
 *  mamedb.h
 *  CabSDL
 *
 *  Created by ickymachine on 12/2/10.
 *  Copyright 2010. All rights reserved.
 *
 */

#ifndef MAMEDB_H
#define MAMEDB_H

#define CURL_STATICLIB
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#include <string>

using namespace std;

class MameDB {
public:
	static void DownloadImage(string file_name);
private:
	static size_t WriteFile(void* ptr, size_t size, size_t nmemb, FILE *stream);
};

#endif
