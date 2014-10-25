#ifndef XML_H
#define XML_H
#include "parse.h"
#include "pugi/pugixml.hpp"
#include <iostream>
#include <sstream>

int save_to_xml(vector<Track> *tracks, int count);
int read_from_xml();
bool update_xml(vector<Track> *track, int downloaded_tracks);
const char *path_to_xml();


#endif // XML_H
