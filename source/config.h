#ifndef CONFIG_H
#define CONFIG_H
#include "auth.h"

void save_config(string access_token, string uid);
const char * path_to_config();
vector<string> read_config();
bool check_config();
string get_selfpath();
#endif // CONFIG_H
