#ifndef AUTH_H
#define AUTH_H
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <glib.h>
#include <vector>
#include "parse.h"
using namespace std;
vector<string> auth();
string create_app_request_url(string url_request_adress, string app_id, string scope, string display);
string create_login_request_url(string ip_h, string to, string _origin, string pass, string email);
vector<string> get_the_token(string headers);
void echo( bool on = true );
#endif // AUTH_H
