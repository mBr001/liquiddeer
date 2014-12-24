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

    printf("\n%s", homedir->str);

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
    xmlNewChild(root_node, NULL, BAD_CAST "ask_question",BAD_CAST "1");


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

GArray *parse_settings(xmlDocPtr doc, xmlNodePtr cur) {
    GArray *parsed_things;
    parsed_things = g_array_new (TRUE, TRUE, sizeof (xmlChar*));
    xmlChar *access_token=NULL, *uid=NULL, *path=NULL, *ask=NULL, *bee=NULL;
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"access_token"))) {
            access_token = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            g_array_append_val(parsed_things, access_token);

//            xmlFree(access_token);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"uid"))) {
            uid = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            g_array_append_val(parsed_things, uid);

//            printf("uid: %s\n", uid);
//            xmlFree(uid);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"path"))) {
            path = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            g_array_append_val(parsed_things, path);

//            printf("path: %s\n", path);
//            xmlFree(path);
        }
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"ask"))) {
            ask = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            g_array_append_val(parsed_things, ask);

//            printf("ask: %s\n", ask);
//            xmlFree(ask);
        }
    cur = cur->next;
    }
    return parsed_things;
}

GArray * read_config(){
    GArray *settings;
    settings = g_array_new (TRUE, TRUE, sizeof (gchar *));
    xmlDocPtr doc = NULL;
    xmlNodePtr root = NULL;

    doc = xmlParseFile(path_to_config());
    root = xmlDocGetRootElement(doc);

    check_config(doc, root);

    while (root != NULL) {
        if ((!xmlStrcmp(root->name, (const xmlChar *)"settings"))){
            settings = parse_settings(doc, root);
        }

        root = root->next;
    }
    return settings;
}

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
    if (xmlStrcmp(cur->name, (const xmlChar *) "settings")) {
        fprintf(stderr,"document of the wrong type, root node != settings");
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
