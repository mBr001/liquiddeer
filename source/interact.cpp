#include <iostream>
#include <string>

#include "interact.h"
#include "config.h"

using namespace std;



bool yesman(){
    string answer;
    vector<string> settings = read_config();
    //check settings if ask question
    if(settings[3] == "0"){
        settings.clear();
        return false;
    }
    cin>>answer;

    if(answer=="y" || answer=="yes"){
        return true;
    }else if(answer=="n" || answer=="no"){
        return false;
    }else{
        cout<<"Please enter yes(y) or no(n)"<<endl;
        yesman();
    }
}

void auth_message(){
    cout<<endl<<"Program needs your permission to use friend list, music library and basic information"<<endl;
}

void warning(){
    cout<<"\t\tWARNING:"<<endl;
    cout<<"Downloading some of tracks may be illegal in your country."<<endl<<endl;
}
