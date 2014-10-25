#include "sync.h"
#include <sstream>
#include <algorithm>
#define URL_ADRESS "https://api.vkontakte.ru/method/audio.get?"
#define OFFSET "0"
#define COUNT_OF_TRACKS "0" //0 - get all tracks from user. Doesn't really work. To get this work,
                            //reimplement get_count function
#define API_VERSION "5.21"
#define TIMEOUT 15


int synchro(){                              //getting and downloading only fresh tracks
    vector<Track> tracks_to_download;
    int count_to_download = 0, count_of_local_tracks = 0, count_of_server_tracks = 0;

    //do not get token if already have one in config
    //note: [0] hash, [1] uid, [2] default path to music directory
    vector<string> settings;
    if(check_config()){
        settings = read_config();
    }else{
        auth_message();
        vector<string>auth_things = auth();
        save_config(auth_things[0], auth_things[1]);
        auth_things.clear();
    }

    count_of_local_tracks = read_from_xml();//reading number of local tracks from library

    if(count_of_local_tracks==0){                           //asking user to continue
        cout<<"Library is empty. Downloading tracks and creating library will take a time."<<endl<<"Proceed ? ";   //if not then quit
        if(!yesman()){
            return 1;
        }
    }

    //first we need to know how much to download
    string buffer = curl_httpget(get_audio_url(URL_ADRESS, settings[1], COUNT_OF_TRACKS, settings[0], OFFSET, API_VERSION));
    count_of_server_tracks = get_count(buffer);

    count_to_download = count_of_server_tracks - count_of_local_tracks; //very complicated algorithm

    if(count_to_download > 0){
        cout<<"Tracks to obtain: "<<count_to_download<<endl;
        tracks_to_download = parse_this(curl_httpget(get_audio_url(URL_ADRESS, settings[1], count_to_download, settings[0], OFFSET, API_VERSION)));


        cout.precision(2);
        cout<<"Do You really want to download "; show_tracks_weight(&tracks_to_download, count_to_download);
        if(yesman()){
            for(int i = 0; i<count_to_download+1; i++){
                //checking if element of vector exists
                if(i<tracks_to_download.size()){
                    cout<<i<<". Downloading "<<tracks_to_download[i].artist<<" - " <<tracks_to_download[i].title<<endl;
                    //downloading
                    if(!save_file(tracks_to_download[i].url, tracks_to_download[i].title, tracks_to_download[i].artist, settings[2])){
                        cerr<<"Something wrong with saving"<<endl;
                    }
                }else{
                    cout<<"No more tracks to download"<<endl;
                }
            }
            if(!update_xml(&tracks_to_download, count_to_download)){
                cerr<<"Library NOT updated"<<endl;
            }

        }else{
            cout<<"That's all folks"<<endl;
        }
    }
    else if(count_to_download == 0){
        cout<<"Number of tracks not changed: "<<count_to_download<<endl;
    }else{
        cout<<"Looks like You have deleted few tracks from VK: "<<count_to_download<<endl;
    }

    tracks_to_download.clear();
    return 0;
}

double show_tracks_weight(vector<Track>* tracks_to_download, int count_to_download){
    double total_weight = 0.0;
    for(int i=0; i<count_to_download; i++){
        total_weight += tracks_to_download->at(i).weight;
    }
    total_weight /=(1024*1024);
    if(total_weight>999){
        total_weight /= 1024;
        cout<<total_weight<<"GB ?";
    }else{
        cout<<total_weight<<"MB ?";
    }
    return 0;
}


bool save_file(string url, string title, string artist, string path){   //taken from http://www.cyberforum.ru/post1746541.html
    CURL *curl;
    FILE *fp;
    CURLcode res;

    //cut the title string
    artist = artist.substr(0, 60);

    //because people sometimes use backslashes
    size_t size_of_artist = artist.find_first_of("/");
    size_t size_of_title = title.find_first_of("/");

    if(size_of_artist != string::npos){
        replace( artist.begin(), artist.end(), '/', ' ');
    }
    if(size_of_title != string::npos){
        replace( title.begin(), title.end(), '/', ' ');
    }
    //generating file name
    path.append("/"+artist+"-"+title+".mp3");
    sleep(0.3);

    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(path.c_str(),"wb");


        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func); //progressbar
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);

        if(fp==NULL){
            cout<<"FILE NULL ";
        }
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        if(res!=0){
            cout<<curl_easy_strerror(res)<<endl;
        }else{
            cout<<"Saved"<<endl;
        }
        return true;
    }
    cerr<<"Something wrong with saving"<<endl;
    return false;
}

//http://stackoverflow.com/questions/1637587/c-libcurl-console-progress-bar

int progress_func(void* ptr, double TotalToDownload, double NowDownloaded,
                  double TotalToUpload, double NowUploaded)
{
    // how wide you want the progress meter to be
    int totaldotz=40;
    double fractiondownloaded = 0.0;
    //avoiding NaN in fractiondownloaded
    if(NowDownloaded!=0){
        fractiondownloaded = NowDownloaded / TotalToDownload;
    }

    // part of the progressmeter that's already "full"
    int dotz = round(fractiondownloaded * totaldotz);

    cout.sync_with_stdio(false);


    // create the "meter"
    int ii=0;
    // part  that's full already
    for ( ; ii < dotz;ii++) {
        cout<<".";
    }
    // remaining part (spaces)
    for ( ; ii < totaldotz;ii++) {
        cout<<" ";
    }
    //avoiding 0 at right
    if(fractiondownloaded!=0){
        cout<<fixed<<fractiondownloaded*100<<"%";
    }
    cout<<"\r";
    // and back to line begin - do not forget the fflush to avoid output buffering problems!

    cout.flush();

    return 0;
}
