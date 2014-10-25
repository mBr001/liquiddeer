#ifndef PARSE_H
#define PARSE_H
#include <string>
#include <iostream>
#include <sstream>
#include <curl/curl.h>
#include <jansson.h>
#include <vector>
#include <typeinfo>
#include "track.h"
#include "xml.h"

int writer(char *data, size_t size, size_t nmemb, string *buffer);
string curl_httpget(const string &url);
vector<Track> parse_this(string buffer);
template<class T> string get_audio_url(string url_adress, string uid, T count, string access_token, string offset, string ver){
       if (typeid(count) == typeid(int)){ //powtórzenia kodu :(
            stringstream strs;
            strs << count;
            string count = strs.str();
            return url_adress+"uid="+uid+"&"+"offset="+offset+"&"+"count="+count+"&"+"v="+ver+"&"+"access_token="+access_token;

        }else{
           stringstream strs;
           strs << count;
           string count = strs.str(); //мясорубка для стрингов
           string url = url_adress+"uid="+uid+"&"+"offset="+offset+"&"+"count="+count+"&"+"v="+ver+"&"+"access_token="+access_token;
       return url;
       }

    }

bool fileExists(const string& filename);
int get_count(string buffer);
double check_weight(string url);
#endif // PARSE_H
