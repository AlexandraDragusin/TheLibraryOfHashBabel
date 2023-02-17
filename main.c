// Copyright (C) 2022 Dragusin Daniela-Alexandra (311CA / 2021-2022)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_SIZE	256
#define MAX_BOOK_NAME 40
#define MAX_USER_NAME 20
#define MAX_STRING 20
#define HMAX 10

#include "utils.h"
#include "library.h"
#include "book.h"
#include "user.h"
#include "functions.h"

int main()
{
	hashtable_t *library = ht_create(HMAX, hash_function,
							compare_function);
	hashtable_t *community = ht_create(HMAX, hash_function, compare_function);

	while (1) {
		char command[20];
		scanf("%s", command);

		if (strcmp(command, "ADD_BOOK") == 0) {
			add_book(library);
		} else if (strcmp(command, "GET_BOOK") == 0) {
			get_book(library);
		} else if (strcmp(command, "RMV_BOOK") == 0) {
			remove_book(library);
		} else if (strcmp(command, "ADD_DEF") == 0) {
			add_def(library);
		} else if (strcmp(command, "GET_DEF") == 0) {
			get_def(library);
		} else if (strcmp(command, "RMV_DEF") == 0) {
			rmv_def(library);
		} else if (strcmp(command, "ADD_USER") == 0) {
			add_user(community);
		} else if (strcmp(command, "BORROW") == 0) {
			borrow(community, library);
		} else if (strcmp(command, "RETURN") == 0) {
			return_book(community, library);
		} else if (strcmp(command, "LOST") == 0) {
			lost(community, library);
		} else if (strcmp(command, "EXIT") == 0) {
			top_books(library);
			top_users(community);
			exit_library(library);
			exit_community(community);
			return 0;
		}
	}

	return 0;
}
