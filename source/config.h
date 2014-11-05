#ifndef CONFIG_H
#define CONFIG_H
//#include "auth.h"
#include <glib.h>
#include <stdbool.h>
#include <libxml2/libxml/parser.h>

void save_config(GString *access_token, GString *uid);
gchar* path_to_config();
GArray *read_config();
bool check_config(xmlDocPtr doc, xmlNodePtr cur);
//string get_selfpath();
#endif // CONFIG_H
