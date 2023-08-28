#pragma once
#include <stdio.h>

#define SAVED_DATA_FILE_NAME "info.dat"

//commands
#define HELP "?"
#define PRINTDIR "printdir"
#define SELECTDIR "selectdir"
#define ADDDIR "adddir"
#define EXCLUDEFILE "excludefile"
#define SELECTFILE "selectfile"
#define SEARCHDIR "search"
#define ADDFILE "addfile"
#define ADDTAGS "addtags"
#define OPENFILE "open"
#define REMOVETAGS "removetags"
#define EXIT "exit"

#define FILE_PLACEHOLDER "filename.extension"
#define TAG_PLACEHOLDER "tag"
#define DIR_INDEX_PLACEHOLDER "index"
//user messages

#define PRINT_FOLDER_DIRECTORIES(__sc)									\
		{																\
			folder_info* fi = NULL;										\
			printf("Saved folders and their corresponding index: \n");	\
			uint i = 0;													\
			while ((fi = sc->folders[i]) != NULL) {						\
				printf("\t%i | %s\n", i, fi->folder_name);				\
				i++;													\
			}															\
		}			

#define PRINT_FILES_IN_FOLDER(__sc)										\
		{																\
			linked_list* __files = __sc->folders[__sc->selected_folder]->files;\
			printf("Saved files and their corresponding index: \n");	\
			for(size_t __i = 0; __i < __files->length; __i++) {						\
				file_info* __f = *((file_info**)list_get(__files, __i));								\
				printf("\t%i | %s\n", __i, __f->file_name);				\
			}															\
		}

#define PRINT_GENERAL_HELP()								\
		{													\
			printf("Print Directories: %s\n", PRINTDIR);	\
			printf("Select Directory: %s %s\n", SELECTDIR, DIR_INDEX_PLACEHOLDER);	\
			printf("Add Directory: %s\n", ADDDIR);			\
			printf("Exclude File: %s\n", EXCLUDEFILE);		\
			printf("Exit program: %s\n", EXIT);				\
		}

#define PRINT_SELECTED_DIR_HELP()														\
		{																				\
			printf("Select and edit File: %s %s\n", SELECTFILE, FILE_PLACEHOLDER);		\
			printf("Print all files in current folder: %s\n", PRINTDIR);		\
			printf("Open file by index in File Explorer: %s %s\n", OPENFILE, DIR_INDEX_PLACEHOLDER);		\
			printf("Search tagged files (seperate tags by comma): %s %s1,%s2,...\n", SEARCHDIR, TAG_PLACEHOLDER, TAG_PLACEHOLDER);	\
			printf("Save and Exit folder: %s\n", EXIT);											\
		}

#define PRINT_SELECTED_FILE_HELP()														\
		{																				\
			printf("Remove File Tags: %s\n", REMOVETAGS);	\
			printf("Add File Tags: %s\n", ADDTAGS);	\
			printf("Open file in File Explorer: %s\n", OPENFILE);				\
			printf("Save and Exit file: %s\n", EXIT);											\
		}


#define INTRO_MSG "Enter command (? for help): "
#define SELECTED_DIR_MSG "Selected Folder: [%s]\nEnter command (? for help): "
#define SEARCH_DIR_RES_MSG "Found %i files related to tags:\n"