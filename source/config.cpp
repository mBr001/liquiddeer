#include "config.h"
#include <sys/stat.h>
#include "pugi/pugixml.hpp"
#include <iostream>
#include <sstream>

using namespace std;
using namespace pugi;

void save_config(string access_token, string uid)
{
    string default_path_to_store;
    string homedir = getenv("HOME");

    default_path_to_store.append(homedir);
    default_path_to_store.append("/Music/");
    xml_document doc;
    xml_node ac_to=doc.append_child("access_token");
    ac_to.append_attribute("value")=access_token.c_str();
    xml_node uidx=doc.append_child("uid");
    uidx.append_attribute("value")=uid.c_str();
    //default path to music directory
    xml_node path_to_store_tracks=doc.append_child("path");
    path_to_store_tracks.append_attribute("value")=default_path_to_store.c_str();
    //ask question about downloading ?
    xml_node askx=doc.append_child("ask_question");
    askx.append_attribute("value")=1; //deafault 1 "NO"
    //download tracks from wall
    xml_node download_wall=doc.append_child("download_wall");
    download_wall.append_attribute("value")=1; //deafault 1 "NO"


    struct stat sb;

    homedir.append("/.liquid");

    //check if ./liquid directory exist
    if (stat(homedir.c_str(), &sb) !=( 0 && S_ISDIR(sb.st_mode)))
    {
        mkdir(homedir.c_str(), 0777);
    }
    homedir.clear();

    doc.save_file(path_to_config());
    cout<<"config saved to "<<path_to_config()<<endl<<endl;
}

vector<string> read_config(){
    vector<string> settings;
    xml_document doc;
    string access_token, uid, default_path_to_store, ask_question;
    if (!doc.load_file(path_to_config()))
    {
        cerr<<"Something wrong with path to config"<<endl;
    }
    //reading token
    xml_node ac_to=doc.child("access_token");
    access_token = ac_to.attribute("value").value();
    settings.push_back(access_token);

    //reading uid
    xml_node uidx=doc.child("uid");
    uid = uidx.attribute("value").value();
    settings.push_back(uid);

    //reading path
    xml_node path_to_store_tracks=doc.child("path");
    default_path_to_store = path_to_store_tracks.attribute("value").value();
    settings.push_back(default_path_to_store);

    //reading ask_question state
    xml_node askx=doc.child("path");
    ask_question = askx.attribute("value").value();
    settings.push_back(ask_question);

    return settings;
}

bool check_config(){
    xml_document doc;
    if(doc.load_file(path_to_config())){
        if(doc.child("access_token")){
            return true;
        }
    }
    return false;
}

const char* path_to_config(){
    string path;

    string homedir = getenv("HOME");
    path.append(homedir);
    path.append("/.liquid/conf");
    const char* path_c = path.c_str();
    return path_c;
}

