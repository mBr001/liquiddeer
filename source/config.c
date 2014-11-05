#include "config.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <libxml/tree.h>
#include <unistd.h>
void save_config(GString *access_token, GString *uid){
    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;/* node pointers */
    //check if dir exists
    struct stat st = {0};
    GString* homedir = g_string_new(NULL);
    g_string_append(homedir, getenv("HOME"));
    g_string_append(homedir, "/.liquid");
    printf("%s", homedir->str);
    if (stat(homedir->str, &st) == -1) {
        mkdir(homedir->str, 0777);
        g_string_free(homedir, TRUE);
    }
        doc = xmlNewDoc(BAD_CAST "1.0");
        root_node = xmlNewNode(NULL, BAD_CAST "settings");
        xmlDocSetRootElement(doc, root_node);
        //saving access token
        xmlNewChild(root_node, NULL, BAD_CAST "access_token", BAD_CAST access_token->str);
        //saving user id
        xmlNewChild(root_node, NULL, BAD_CAST "uid",BAD_CAST uid->str);
        //retrieving default path
        GString* default_path = g_string_new(NULL);
        g_string_append(default_path, getenv("HOME"));
        g_string_append(default_path, "/Music/");
        //saving default path
        xmlNewChild(root_node, NULL, BAD_CAST "path",BAD_CAST default_path->str);
        //asking all this questions ?
        xmlNewChild(root_node, NULL, BAD_CAST "path",BAD_CAST "1");


        if (doc != NULL) {
            xmlSaveFormatFile (path_to_config(), doc, 0);
            xmlFreeDoc(doc);
            g_string_free(default_path, TRUE);
            printf("Config saved to %s", path_to_config());
        }else{
            printf("holy shit");
            xmlFreeDoc(doc);
            // Free the global variables that may have been allocated by the parser.
            xmlCleanupParser();
        }

}


// GArray * read_config(){
//     GArray *settings;
//     settings = g_array_new (TRUE, TRUE, sizeof (GString *));
//     xmlChar *value;
//     xmlDocPtr doc;
//     xmlNodePtr cur;
//     doc = xmlParseFile(path_to_config());
//     cur = xmlDocGetRootElement(doc);

//     check_config(doc, cur);
//     while (cur != NULL) {

//     if ((!xmlStrcmp(cur->name, (const xmlChar *)"access_token"))) {
//                 value = xmlGetProp(cur, "value");
//                 printf("uri: %s\n", value);
//                 xmlFree(value);
//             }
//             cur = cur->next;
//     }
////     //reading token
////     xml_node ac_to=doc.child("access_token");
////     *access_token->str = *ac_to.attribute("value").value();
////     g_array_append_val(settings, access_token);

////     //reading uid
////     xml_node uidx=doc.child("uid");
////     *uid->str = *uidx.attribute("value").value();
////     g_array_append_val(settings, uid);

////     //reading path
////     xml_node path_to_store_tracks=doc.child("path");
////     *default_path_to_store->str = *path_to_store_tracks.attribute("value").value();
////     g_array_append_val(settings, default_path_to_store);

////     //reading ask_question state
////     xml_node askx=doc.child("path");
////     *ask_question->str = *askx.attribute("value").value();
////     g_array_append_val(settings, ask_question);

//     return settings;
// }

bool check_config(xmlDocPtr doc, xmlNodePtr cur){

    if (doc == NULL ) {
        fprintf(stderr,"Document not parsed successfully. \n");
        return false;
    }
    if (cur == NULL) {
        fprintf(stderr,"empty document\n");
        xmlFreeDoc(doc);
        return false;
    }
    if (xmlStrcmp(cur->name, (const xmlChar *) "access_token")) {
        fprintf(stderr,"document of the wrong type, root node != Tracks");
        xmlFreeDoc(doc);
        return false;
    }
    return true;
}

gchar* path_to_config(){
    GString* config_path = g_string_new(NULL);

    g_string_append(config_path, getenv("HOME"));
    g_string_append(config_path, "/.liquid/conf1");
    return config_path->str;
}
