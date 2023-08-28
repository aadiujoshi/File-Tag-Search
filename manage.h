#pragma once

#include "linked_list.h"

#define MAX_DIR 50

#define TAG_SPLIT_CHARACTER ':'

#define se(str1, str2) !strcmp(str1, str2)
#define strim(__str)    {                                                   \
                            size_t __l = strlen(__str);                     \
                            if (__l > 0 && __str[__l - 1] == '\n') {        \
                                __str[__l - 1] = '\0';                      \
                            }                                               \
                        }

#define cmdtrim(__str, __buf)   {                                                   \
                                    size_t __i = 0;                                 \
                                    size_t __slen = strlen(__str);                  \
                                    while ((__i < __slen) & (__str[__i] != ' ')) {  \
                                        __buf[__i] = __str[__i];                    \
                                        __i++;                                      \
                                    }                                               \
                                    __buf[__i] = '\0';                              \
                                }                                   

#define tagsplit(__tagsstr, __len, __taglist)                                           \
                                {                                                       \
                                    char __tbuf[128] = { 0 };                           \
                                    size_t __bi = 0;                                    \
                                    for (size_t __i = 0; __i < __len; __i++) {          \
                                        if (tagargs[__i] == TAG_SPLIT_CHARACTER) {      \
                                            __tbuf[__bi] = '\0';                        \
                                            tag* __nt = create_file_tag(__tbuf);        \
                                            list_add(__taglist, __nt);                  \
                                            __bi = 0;                                   \
                                        }                                               \
                                    }                                                   \
                                }


#define println() printf("\n")

#define uint unsigned int

typedef struct _tag{
    char* val;
} tag;

typedef struct _file_info{
    char* file_name;
    linked_list* tags;
} file_info;

typedef struct _folder_info{
    char* folder_name;
    linked_list* files;
    int selected_file;
} folder_info;

typedef struct _search_context{
    folder_info* folders[MAX_DIR];
    uint next_free;
    int selected_folder;
} search_context;

search_context* create_search_context();
folder_info* create_folder_info(char* folder_name);
file_info* create_file_info(char* file_name);
tag* create_file_tag(char* tag_val);

void delete_search_context(search_context* sc);
void delete_folder_info(folder_info* fi);
void delete_file_info(file_info* fi);
void delete_file_tag(tag* t);

void load_info(search_context* ctx);
void save_info(search_context* ctx);

//tagged_files is a linked list of file_info*
void search_tags(search_context* sc, linked_list* tag, linked_list* tagged_files);
