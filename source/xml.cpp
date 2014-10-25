#include "xml.h"
using namespace std;
using namespace pugi;
int save_to_xml(vector<Track>* tracks, int count)
{
    xml_document doc;
    xml_node audios=doc.append_child("Tracks");  // <Tracks count="123">
    audios.append_attribute("count")=count;      // <Track aid="12334567"/>

    for( int i=0; i<count; i++)
    {
        xml_node track=audios.append_child("Track");
        int audio_id = tracks->at(i-i).aid; //top - oldest added, bottom - newest tracks
        track.append_attribute("aid")=audio_id;
    }

    doc.save_file(path_to_xml());
    cout<<"saved to "<<path_to_xml();


    return 0;
}
int read_from_xml(){    //get count of local tracks
    int count, audio_id;
    xml_document doc;
    if (!doc.load_file(path_to_xml()))
    {
        return 0;
    }
    xml_node audios=doc.child("Tracks");

    //from const char* to int
    stringstream string_count(audios.attribute("count").value());
    string_count >> count;

    //        cout<<"Count of tracks: "<<count<<endl;
    for(xml_node track=audios.child("Track"); track; track=track.next_sibling("Track"))
    {
        //from const char* to int
        stringstream string_aid(track.attribute("aid").value());
        string_aid >>audio_id;

    }
    return count;
}

bool update_xml(vector<Track>* tracks, int count_to_download){
    int count;
    xml_document doc;
    if (!doc.load_file(path_to_xml()))
    {
        save_to_xml(tracks, count_to_download);
        return 0;
    }
    xml_node audios=doc.child("Tracks");

    //getting value of already stored tracks and converting to int
    stringstream string_count(audios.attribute("count").value());
    string_count >> count;

    //changing value of attribute count
    xml_attribute count_attribute = audios.attribute("count");

    count_attribute.set_value(count + count_to_download);

    for(int i; i<count_to_download; i++){
        xml_node track_xml=audios.append_child("Track");
        int audio_id = tracks->at(i).aid;
        track_xml.append_attribute("aid")=audio_id;
    }
    doc.save_file(path_to_xml());
    cout<<"Library updated"<<endl;
    return true;
}

const char* path_to_xml(){ //nie zapomniec o opcji wybierania sciezki
    string path;    //pobieranie sciezki do HOME

    string homedir = getenv("HOME");
    path.append(homedir);
    path.append("/.liquid/library");
    const char* path_c = path.c_str();
    return path_c;
}
