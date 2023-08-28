#include "manage.h"
#include "file_tag_search.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include <string.h>

void load_info(search_context *ctx) {
    FILE* info_f = fopen(SAVED_DATA_FILE_NAME, "r");

    char buffer[2048];

    folder_info* cfolder = NULL;
    file_info* cfile = NULL;

    int ifolder = 0;
    
    while (fgets(buffer, sizeof(buffer), info_f) != NULL) {
        strim(buffer);

        if(buffer[0] == 'd'){
            cfolder = create_folder_info(buffer + 2);
            ctx->folders[ifolder++] = cfolder;
        }

        if(buffer[0] == 'f'){
            cfile = create_file_info(buffer + 2);
            list_add(cfolder->files, &cfile);
        }

        if(buffer[0] == 't'){
            tag* t = create_file_tag(buffer + 2);
            list_add(cfile->tags, &t);

            //size_t len = strlen(buffer);
            //char tbuf[128];
            //int ti = 0;
            //for (size_t i = 1; i < len; i++) {
            //    if(buffer[i] == TAG_SPLIT_CHARACTER){
            //        tbuf[ti] = '\0';
            //        ti = 0;
            //        tag* t = create_file_tag(tbuf);
            //        list_add(cfile->tags, &t);
            //        continue;
            //    }
            //    tbuf[ti++] = buffer[i];
            //}
        }
    }

    fclose(info_f);
}

void save_info(search_context* ctx){
    FILE* info_f = fopen(SAVED_DATA_FILE_NAME, "w");
    
    int ifolder = 0;

    folder_info* cfolder = NULL;
    file_info* cfile = NULL;

    while ((cfolder = ctx->folders[ifolder++]) != NULL) {

        fprintf(info_f, "d %s\n", cfolder->folder_name);
        //fprintf(info_f, "--------------------------------------------\n");

        linked_list* fl = cfolder->files;
        for (size_t i = 0; i < fl->length; i++) {
            file_info* file = *((file_info**)list_get(fl, i));

            fprintf(info_f, "f %s\n", file->file_name);

            linked_list* tl = file->tags;
            for (size_t j = 0; j < tl->length; j++) {
                tag* t = *((tag**)list_get(tl, j));
                fprintf(info_f, "t %s\n", t->val);
            }
        }
    }

    fclose(info_f);
}

search_context* create_search_context() {
    search_context* ctx = malloc(sizeof(search_context));
    ctx->selected_folder = -1;
    ctx->next_free = 0;
    memset(ctx->folders, NULL, sizeof(ctx->folders));
    return ctx;
}

folder_info* create_folder_info(char* folder_name) {
    folder_info* fi = malloc(sizeof(folder_info));
    fi->files = create_list(sizeof(file_info*));
    fi->selected_file = -1;
    fi->folder_name = malloc(strlen(folder_name));
    strcpy(fi->folder_name, folder_name);
    return fi;
}

file_info* create_file_info(char* file_name) {
    file_info* fi = malloc(sizeof(file_info));
    fi->tags = create_list(sizeof(tag*));
    fi->file_name = malloc(strlen(file_name));
    strcpy(fi->file_name, file_name);
    return fi;
}

tag* create_file_tag(char* tag_val){
    tag* t = malloc(sizeof(tag));
    t->val = malloc(strlen(tag_val));
    strcpy(t->val, tag_val);
    return t;
}

void delete_search_context(search_context* sc) {
    for (size_t i = 0; i < MAX_DIR; i++) {
        if (sc->folders[i] == NULL)
            break;

        delete_folder_info(sc->folders[i]);
    }
    free(sc);
}

void delete_folder_info(folder_info* fi) {
    linked_list* fl = fi->files;

    for (size_t i = 0; i < fl->length; i++) {
        file_info* file = *((file_info**)list_get(fl, i));
        delete_file_info(file);
    }

    free(fi->folder_name);
    free(fi);
}

void delete_file_info(file_info* fi) {
    linked_list* ft = fi->tags;

    for (size_t i = 0; i < ft->length; i++) {
        tag* t = *((tag**)list_get(ft, i));
        delete_file_tag(t);
    }

    free(fi->file_name);
    free(fi);
}

void delete_file_tag(tag* t) {
    free(t->val);
    free(t);
}

//str1 is real tag, str2 is search tag
//accounts for mispelling
bool _tagscore(char* orig, char* str2) {

    char map1[26] = { 0 };
    char map2[26] = { 0 };

    size_t len1 = strlen(orig);
    size_t len2 = strlen(str2);

    for (size_t i = 0; i < len1; i++) {
        map1[orig[i]]++;
    }

    for (size_t i = 0; i < len2; i++) {
        map2[str2[i]]++;
    }

    int score = 0;

    for (size_t i = 0; i < min(len1, len2); i++) {
        if (orig[i] == str2[i]) {
            score+=2;
        }
        else {
            if (map1[str2[i]]) {
                score++;
            }
        }
    }

    return (score >= (len1 / 2));
}

void search_tags(search_context* sc, linked_list* search_tags, linked_list* tagged_files) {
    if (sc->selected_folder < 0)
        return;

    folder_info* dir = sc->folders[sc->selected_folder];
    
    linked_list* files = dir->files;

    for (size_t i = 0; i < files->length; i++) {
        file_info* file = *((file_info**)list_get(files, i));

        linked_list* filetags = file->tags;

        for (size_t j = 0; j < filetags->length; j++) {
            tag* ftag = *((tag**)list_get(filetags, j));

            for (size_t k = 0; k < search_tags->length; k++) {
                tag* stag = *((tag**)list_get(search_tags, k));

                bool r = _tagscore(ftag->val, stag->val);

                if (r)
                    list_add(tagged_files, &file);
            }
        }
    }
}