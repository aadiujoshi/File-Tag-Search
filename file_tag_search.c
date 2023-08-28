#include "file_tag_search.h"
#include "manage.h"
#include "stdio.h"
#include <windows.h>
#include "stdbool.h"
#include "string.h"
#include "linked_list.h"
#include "stdlib.h"

int main() {
    search_context* sc = create_search_context();
    char message[256];
    strcpy(message, INTRO_MSG);
    load_info(sc);
    
    PRINT_FOLDER_DIRECTORIES(sc);

    //tagged files 
    linked_list* tagged = create_list(sizeof(file_info*));

    size_t _next_free = 0;
    while (sc->folders[_next_free++] != NULL);
    sc->next_free = _next_free;

    while(true){
        println();
        println();

        char arg[256] = { 0 };
        char cmd[16] = { 0 };

        printf(message);
        fgets(arg, sizeof(arg), stdin);
        strim(arg);
        cmdtrim(arg, cmd);
        println();
        println();

        //COMMANDS
        if (sc->selected_folder >= 0) {
            folder_info* sfolder = sc->folders[sc->selected_folder];

            if (sfolder->selected_file >= 0) {
                file_info* sfile = *((file_info**)list_get(sfolder->files, sfolder->selected_file));

                if (se(cmd, HELP)) {
                    PRINT_SELECTED_FILE_HELP();
                    continue;
                }
                if (se(cmd, REMOVETAGS)) {

                    continue;
                }
                if (se(cmd, ADDTAGS)) {
                    char* tagargs = arg + strlen(ADDTAGS) + 1;
                    tagsplit(tagargs, strlen(tagargs), sfile->tags);
                    continue;
                }
                if (se(cmd, OPENFILE)) {
                    ShellExecuteA(NULL, "open", sfile->file_name, NULL, NULL, SW_SHOWNORMAL);
                    continue;
                }
                if (se(cmd, EXIT)) {
                    save_info(sc);
                    sfolder->selected_file = -1;
                    continue;
                }
                continue;
            }

            if (se(cmd, HELP)) {
                PRINT_SELECTED_DIR_HELP();
                continue;
            }

            if (se(cmd, SEARCHDIR)) {
                char* tagargs = arg + strlen(SEARCHDIR) + 1;
                size_t rtlen = strlen(tagargs);
                linked_list* searchtags = create_list(sizeof(tag*));

                tagsplit(tagargs, rtlen, searchtags);

                search_tags(sc, searchtags, tagged);

                printf(SEARCH_DIR_RES_MSG, tagged->length);
                for (size_t i = 0; i < tagged->length; i++) {
                    file_info* tf = *((file_info**)list_get(tagged, i));
                    printf("\t%i | %s\n", i, tf->file_name);
                }

                strcpy(message, INTRO_MSG);
                continue;
            }

            if (se(cmd, PRINTDIR)) {
                PRINT_FILES_IN_FOLDER(sc);
                continue;
            }

            if (se(cmd, OPENFILE)) {
                int index = (*(arg + strlen(OPENFILE) + 1)) - '0';

                if ((index < 0) | (index >= sfolder->files->length)) {
                    printf("\tInvalid file index: %i\n", index);
                    continue;
                }

                file_info* sfile = *((file_info**)list_get(sfolder->files, index));
                printf("Opening File: [%s]:\n", sfile->file_name);
                ShellExecuteA(NULL, "open", sfile->file_name, NULL, NULL, SW_SHOWNORMAL);
                continue;
            }

            if (se(cmd, SELECTFILE)) {
                int index = (*(arg + strlen(SELECTFILE) + 1)) - '0';


                list_get(sfolder->files, index
            }
        }

        if (se(cmd, SELECTDIR)) {
            int f_index = (*(arg + strlen(SELECTDIR) + 1)) - '0';

            if ((f_index >= 0) & (f_index < MAX_DIR)) {
                sc->selected_folder = f_index;

                if (sc->folders[sc->selected_folder] != NULL) {
                    snprintf(message, sizeof(message), SELECTED_DIR_MSG, sc->folders[sc->selected_folder]->folder_name);
                }
            }
            continue;
        }
        if(se(cmd, PRINTDIR)){
            PRINT_FOLDER_DIRECTORIES(sc);
            continue;
        }
        if(se(cmd, ADDDIR)){
            sc->folders[sc->next_free++] = create_folder_info(arg + strlen(ADDDIR) + 1);
            continue;
        }
        if(se(cmd, HELP)) {
            PRINT_GENERAL_HELP();
            continue;
        }
        if(se(cmd, EXIT)){
            break;
        }
    }
    
    //save
    printf("Saving...\n");
    save_info(sc);

    //delete_search_context(sc);

    return 0;
}

