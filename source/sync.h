#ifndef SYNC_H
#define SYNC_H
#include <stdio.h>
#include <fstream>
#include <string>
#include <curl/curl.h>

#include "parse.h"
#include "interact.h"
#include "config.h"
#include <cmath>
#include "xml.h"
#include "track.h"
using namespace std;
int synchro();
bool save_file(string url, string title, string artist, string path);
int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded);
//static size_t Writer(char *data, size_t size, size_t nmemb, std::ofstream* pBuf);
double show_tracks_weight(vector<Track> *tracks_to_download, int count_to_download);
#endif // SYNC_H
