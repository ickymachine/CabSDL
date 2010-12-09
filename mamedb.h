// $Id$
/**
 *  @file mamedb.h
 *  Allows downloading of missing resources from mamedb.org
 *
 *  @author Matthew Capodarco
 */
// $Log$

/// Provide functions to download missing resources (image,category,description) from mamedb.org.

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
	/**
	 Attempts to download the image based on the .png file specified in file_name and save it to that location
	 
	 @param[in]	file_name	Path to missing file
	 */
	static void DownloadImage(const string& file_name);
	/**
	 Sets the boolean switch to true, allowing downloading from mamedb.org
	 */
	static void Enable();
	/**
	 Check if mamedb functionality is activated.
	 */
	static bool IsEnabled();
private:
	/**
	 Curl callback function for writing data to a file
	 */
	static size_t WriteFile(void* ptr, size_t size, size_t nmemb, FILE *stream);
	/**
	 Boolean value that must be set to true before using any mamedb downloading functionality.
	 */
	static bool enabled;
};

#endif
