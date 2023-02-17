// Copyright (C) Dragusin Daniela-Alexandra (311CA / 2021-2022)
#include "book.h"

/*Functie ce adauga o noua definitie in cartea specificata. Daca aceasta carte
  nu exista in biblioteca, se afiseaza un mesaj specific.
  Daca exista cartea, se adauga in hashtable-ul sau noua pereche
  cheie-valoare. Se redimensioneaza hashtable-ul daca este necesar.*/
void add_def(hashtable_t *library)
{
	char book_name[MAX_BOOK_NAME], key[MAX_STRING], value[MAX_STRING];
	get_book_name(book_name);
	scanf("%s", key);
	scanf("%s", value);

	if (!ht_has_key(library, book_name)) {
		printf("The book is not in the library.\n");
	} else {
		/*Calculez indexul cu ajutorul functiei de hash si parcurg lista
		  de elemente cu indexul dorit.*/
		unsigned int ind = (library->hash_function)(book_name) % library->hmax;
		list_t *list = library->buckets[ind];
		node_t *node = list->head;
		while (node) {
			/*Caut cartea cu numele dorit.*/
			if (!library->compare_function(book_name, (*(info *)node->data).key)) {
				book_info_t *book_info = (*(info *)node->data).value;
				hashtable_t *book_hasht = book_info->book;

				/*Adaug noua definitie.*/
				ht_put(book_hasht, key, strlen(key) + 1, value, strlen(value) + 1);

				/*Redimensionez hashtable-ul daca este necesar.*/
				verify_load_factor(book_hasht, 0, 0);
			}
			node = node->next;
		}
	}
}

/*Functie ce afiseaza valoarea definitiei corespunzatoare din cartea
  specificata. In cazul in care nu exista cartea respectiva in biblioteca,
  se va afisa un mesaj specific.*/
void get_def(hashtable_t *library)
{
	char book_name[MAX_BOOK_NAME], key[MAX_STRING];
	get_book_name(book_name);
	scanf("%s", key);

	if (!ht_has_key(library, book_name)) {
		printf("The book is not in the library.\n");
	} else {
		/*Calculez indexul cu ajutorul functiei de hash si parcurg lista
		  de elemente cu indexul dorit.*/
		unsigned int ind = (library->hash_function)(book_name) % library->hmax;
		list_t *list = library->buckets[ind];
		node_t *node = list->head;
		while (node) {
			/*Caut definitia pentru cheia dorita si o printez.
			  In cazul in care nu exista definitia, se va afisa un mesaj.*/
			if (!library->compare_function(book_name, (*(info *)node->data).key)) {
				book_info_t *book_info = (*(info *)node->data).value;
				hashtable_t *book_hasht = book_info->book;
				char *value = ht_get(book_hasht, key);
				if (!value)
					printf("The definition is not in the book.\n");
				else
					printf("%s\n", value);
			}
			node = node->next;
		}
	}
}

/*Functie ce sterge o definitie specificata dintr-o anumita carte.
  In cazul in care nu exista cartea in biblioteca sau nu exista definitia
  in carte, se vor afisa mesaje corespunzatoare.
  In caz contrar, va fi stearsa perechea cheie-valoare din cartea specificata*/
void rmv_def(hashtable_t *library)
{
	char book_name[MAX_BOOK_NAME], key[MAX_STRING];
	get_book_name(book_name);
	scanf("%s", key);

	if (!ht_has_key(library, book_name)) {
		printf("The book is not in the library.\n");
	} else {
		/*Calculez indexul cu ajutorul functiei de hash si parcurg lista
		  de elemente cu indexul dorit.*/
		unsigned int ind = (library->hash_function)(book_name) % library->hmax;
		list_t *list = library->buckets[ind];
		node_t *node = list->head;
		while (node) {
			info *data = node->data;
			/*Caut cheia dorita si sterg perechea cheie-valoare din hashtable.*/
			if (!library->compare_function(book_name, (*(info *)node->data).key)) {
				book_info_t *book_info = data->value;
				hashtable_t *book_hasht = book_info->book;

				if (!ht_has_key(book_hasht, key)) {
					printf("The definition is not in the book.\n");
				} else {
					ht_remove_entry(book_hasht, key, 0);
				}
			}
			node = node->next;
		}
	}
}
