#include <stdio.h>
//#include "sync.h"
//#include "config.h"
#include "interact.h"
#define LD_VER "0.9.5"
#define CODENAME "Bambi"
#include <libxml2/libxml/parser.h>
#include "config.h"
int main()
{
        printf("***************************************\n");
        printf("\tLiquidDeer %s %s\n", LD_VER, CODENAME);
        printf("***************************************\n");
       warning();
        // yesman();

//check_config(path_to_config());
       printf("%s", path_to_config());
//       read_config();
       GString* access, *uid;
       access = g_string_new(NULL);
       uid = g_string_new(NULL);
       g_string_append(access, "asgfafkaf");
       g_string_append(uid, "1235345");
    save_config(access, uid);



//        synchro();

}

