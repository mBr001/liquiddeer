#include "parse.h"
#include <cmath>
#include <sys/stat.h>

#define TIMEOUT 1

using namespace std;

//parsing and adding to vector
vector<Track> parse_this(string buffer){
    json_t *root, *items;
    json_error_t error;
    vector<Track> tracks;
    int count_of_loaded_tracks;

    root = json_loads(buffer.c_str(), 0, &error);

    if(!root)
    {
        cerr<<"error: on line "<<error.line<<" "<<error.text<<endl;
    }
    json_t *response = json_object_get(root, "response");

    items = json_object_get(response, "items");

    count_of_loaded_tracks = json_array_size(items); // count of loaded only tracks
    cout.flush();

    for(int i = 0; i < count_of_loaded_tracks; i++)
    {
        json_t *data;
        json_int_t audio_id, owner_id, duration, genre;
        const char *artist, *title, *url;

        data = json_array_get(items, i);
        if(!json_is_object(data))
        {
            cerr<<"error: music data"<<(int)(i + 1)<<" is not an object"<<endl;
            json_decref(items);
        }
        if (json_unpack(data, "{s:I,s:I,s:s,s:s,s:I,s:s,s:I}", //thanks to JKL
                        "id", &audio_id,
                        "owner_id", &owner_id,
                        "artist", &artist,
                        "title", &title,
                        "duration", &duration,
                        "url", &url,
                        "genre", &genre)) {
            cout<<"parsed: "<<i;
            printf("\r");
            cout.flush();
        }


        Track track(audio_id, title, artist, url, check_weight(url)); //check_weight returns size of single track
        tracks.push_back(track);

        //checker
        //        cout<<audio_id<<" "<<artist<<endl<<url<<endl;

    }
    return tracks;
}

//getting count of tracks of whole library
int get_count(string buffer){
    json_t *root, *count;
    json_error_t error;
    int count_of_tracks;
    root = json_loads(buffer.c_str(), 0, &error);

    if(!root)
    {
        cerr<<"error: on line "<<error.line<<" "<<error.text<<endl;
    }
    json_t *response = json_object_get(root, "response");

    count = json_object_get(response, "count"); //count of tracks of whole library, parsed from json
    if(!json_is_integer(count))
    {
        cerr<<"Something wrong with count"<<endl;
        return -1;
    }else{
        count_of_tracks = json_integer_value(count);
    }
    return count_of_tracks;
}

//getting size of track
//from curlib example ftpgetinfo.c
double check_weight(string url){
    CURL *curl;
    CURLcode res;
    double file_weight = 0.0;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, TIMEOUT);

        res = curl_easy_perform(curl);
        if(CURLE_OK == res) {
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &file_weight);
            if((CURLE_OK == res) && (file_weight>0.0)){
                //                printf("filesize: %0.0f bytes\n", file_weight);
                return file_weight;
            }
            else {
                /* we failed */
                cerr<<curl_easy_strerror(res)<<endl;
            }
            curl_easy_cleanup(curl);
            return -1;
        }else{
            cerr<<curl_easy_strerror(res)<<endl;
        }
    }
    return 0;
}

//getting request from url
string curl_httpget(const string &url)
{
    string buffer;
    CURL *curl;
    CURLcode result;
    string path = getenv("HOME");
    path.append("/.liquid/cookie");

    curl = curl_easy_init();

    //removing previous cookie file if exist
    if(fileExists(path)){
        remove(path.c_str());
    }


    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()  );
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
        curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1 );
        curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);
        //        //max 5 перенаправлений
        curl_easy_setopt(curl,CURLOPT_MAXREDIRS,5);
        curl_easy_setopt(curl,CURLOPT_COOKIEFILE, path.c_str());
        curl_easy_setopt(curl,CURLOPT_COOKIEJAR, path.c_str());//cookies!

        result = curl_easy_perform(curl);//http get performed
        curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, NULL);
        curl_easy_setopt(curl,CURLOPT_WRITEHEADER,&cout);


        //error codes: http://curl.haxx.se/libcurl/c/libcurl-errors.html
        if (result == CURLE_OK)

            return buffer;
        //curl_easy_strerror was added in libcurl 7.12.0
        cerr << "error: " << result << " " << curl_easy_strerror(result) <<endl;
        return "";
    }
    cerr << "error: could not initalize curl" << endl;
    curl_easy_cleanup(curl);//must cleanup

    return "";
}

bool fileExists(const string& filename)
{
    struct stat buf;
    if (stat(filename.c_str(), &buf) != -1)
    {
        return true;
    }
    return false;
}

int writer(char *data, size_t size, size_t nmemb, string *buffer)
{
    int result = 0;
    if (buffer != NULL)
    {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}
