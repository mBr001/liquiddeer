//taken from http://www.kselax.ru/2013/09/rabota-s-curl/
//and Gumbo examples

#include "auth.h"
#include "gumbo/gumbo.h"
#include "interact.h"
#include <termios.h>

static vector<string> search_for_inputs(GumboNode* node, vector<string> *input_values) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return *input_values;
    }
    GumboAttribute* input_value;
    if (node->v.element.tag == GUMBO_TAG_INPUT &&
            (input_value = gumbo_get_attribute(&node->v.element.attributes, "value"))) {
        input_values->push_back(input_value->value);
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        search_for_inputs(static_cast<GumboNode*>(children->data[i]), input_values);
    }
    return *input_values;
}

//функция экранирования
//string escape(CURL* curl,const string& text)
//{
//    string result;
//    char* esc_text=curl_easy_escape(curl,text.c_str(),text.length());
//    if(!esc_text)
//        cout<<"Can not convert string to URL";

//    result=esc_text;
//    curl_free(esc_text);

//    return result;
//}



vector<string> auth()
{
    /***************************************************************************************************/
    /**/    string app_request_url = "http://api.vk.com/oauth/authorize?";                           /**/
    /**/    string app_id = "4381957";                                                               /**/
    /**/    string scope = "friends,audio,offline"; //permissions: notify,friends,photos,audio,video,/**/
    /**/                                            //docs,notes,pages,wall,groups,messages,ads      /**/
    /**/                                            //to get neverending token use offline or 999999 /**/
    /**/                                                                                             /**/
    /**/    string auth_window = "wap"; //look of auth window: page, popup, touch & wap              /**/
    /***************************************************************************************************/

    vector<string> input_values;
    string content;
    string contents = curl_httpget(create_app_request_url(app_request_url, app_id, scope, auth_window));
    string headers, mail, pass;

    GumboOutput* output = gumbo_parse(contents.c_str());
    search_for_inputs(output->root, &input_values) ;
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    cout<<"Yor email or phone number: ";
    getline(cin, mail);
    cout<<"Your password:";
    echo(false);//disable echo
    getline(cin, pass);
    echo(true);

    CURL* curl=curl_easy_init();

    string url = create_login_request_url(input_values[1], input_values[2], input_values[0], pass, mail);
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()  );
    }


    //saving html to content
    curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA, &content);

    //saving headers
    curl_easy_setopt(curl,CURLOPT_HEADERFUNCTION, NULL);
    curl_easy_setopt(curl,CURLOPT_WRITEHEADER, &headers);

    //            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    //            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    //            curl_easy_setopt(curl, CURLOPT_POST, 1L);

    string path = getenv("HOME");
    path.append("/.liquid/cookie");

    //pretending that LiquidDeer is a FireFox
    curl_easy_setopt(curl,CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:29.0) Gecko/20100101 Firefox/29.0");
    curl_easy_setopt(curl,CURLOPT_COOKIEFILE, path.c_str());
    curl_easy_setopt(curl,CURLOPT_COOKIEJAR, path.c_str());

    //redirecting
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1 );
    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1);
    curl_easy_setopt(curl,CURLOPT_MAXREDIRS,5);

    CURLcode res=curl_easy_perform(curl);

    if(res!=CURLE_OK)
    {
        cout <<curl_easy_strerror(res)<<endl;
        getchar();
    }else{
        size_t position = headers.find("access_token");
        if(position != std::string::npos)
        {
            cout <<"Authentificated! ";
            return get_the_token(headers);
        }
        else
        {
            cerr <<"Check your email and password and try again"<<endl<<endl;
            curl_easy_cleanup(curl);
           return auth();
        }
    }
    curl_easy_cleanup(curl);
}

string create_app_request_url(string app_request_url, string app_id, string scope, string display){
    string url;
    url+=app_request_url+"client_id="+app_id;
    url+="&redirect_uri=http://api.vk.com/blank.html&scope=";
    url+=scope;
    url+="&display=";
    url+=display;
    url+="&response_type=token"; //the gold
    return url;
}

string create_login_request_url(string ip_h, string to, string _origin, string pass, string email){
    string url = "https://login.vk.com/?act=login&soft=1&utf8=1";
    url+="&_origin="+_origin;
    url+="&ip_h="+ip_h;
    url+="&to="+to;
    url+="&email="+email+"&pass="+pass;
    return url;
}

//getting token and uid from headers
vector<string> get_the_token(string headers){
    string access_token_temp;
    string access_token;
    string uid_temp, uid;
    vector<string> auth_things;

    access_token_temp = headers.substr(headers.rfind("access_token")+13); //passing 12+1 characters, "="
    headers.clear();
    access_token = access_token_temp.substr(0, access_token_temp.find_first_of("&")); //getting token
    auth_things.push_back(access_token);

    uid_temp = access_token_temp.substr(access_token_temp.rfind("user_id")+8);
    uid = uid_temp.substr(0, uid_temp.find_first_of("\r")); //getting uid
    auth_things.push_back(uid);

    return auth_things;
}

//showing or not acho
//http://www.cplusplus.com/forum/general/12256/
void echo( bool on)
{
    struct termios settings;
    tcgetattr( STDIN_FILENO, &settings );
    settings.c_lflag = on
            ? (settings.c_lflag |   ECHO )
            : (settings.c_lflag & ~(ECHO));
    tcsetattr( STDIN_FILENO, TCSANOW, &settings );
}
