#include <stdio.h>
#include <glib.h>
#include "interact.h"
#include "config.h"

bool yesman(){
    GArray *settings;
    gchar *ask_question;
    gchar answer[1];
    settings = g_array_new(TRUE, TRUE, sizeof(gchar *));
    settings = read_config();

    ask_question = g_array_index(settings, gchar *, 3);

    //check settings if ask question
    if(*ask_question == 48){                // 48 is "0"
        g_array_free(settings, TRUE);
        return false;
    }
    scanf("%s", answer);
    if(answer[0]=='y'){
        return true;
    }else if(answer[0]=='n'){
        return false;
    }else{
        printf("Please enter yes(y) or no(n)\n");
        yesman();
    }
}

void auth_message(){
    printf("\nProgram needs your permission to use friend list, music library and basic information\n");
}

void warning(){
    printf("\t\tWARNING:\n");
    printf("Downloading some of tracks may be illegal in your country.\n\n");
}
