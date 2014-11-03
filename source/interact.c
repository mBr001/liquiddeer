#include <stdio.h>
#include <glib.h>
#include "interact.h"
#include "config.h"

//bool yesman(){
//    GArray *settings;
//    GString *setting, answer;
//    settings = g_array_new(TRUE, TRUE, sizeof(GString *));
//    settings = read_config();
//    //check settings if ask question
//    setting = g_array_index(settings, GString *, 3);
//    if(setting->str == "0"){
//        g_array_free(settings, TRUE);
//        return false;
//    }
//    *answer.str = getchar();

//    if(answer.str=="y" || answer.str=="yes"){
//        return true;
//    }else if(answer.str=="n" || answer.str=="no"){
//        return false;
//    }else{
//        printf("Please enter yes(y) or no(n)\n");
//        yesman();
//    }
//}

void auth_message(){
    printf("\nProgram needs your permission to use friend list, music library and basic information\n");
}

void warning(){
    printf("\t\tWARNING:\n");
    printf("Downloading some of tracks may be illegal in your country.\n\n");
}
