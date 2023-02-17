// Copyright (C) Dragusin Daniela-Alexandra (311CA / 2021-2022)
#include "library.h"

/*Functie ce adauga o noua carte in biblioteca. Este alocata memorie pentru o
  noua carte. Sunt initializate caracteristicile cartii. Sunt citite si
  introduse definitiile in carte. Cartea este adaugata in biblioteca.
  Este verificata conditia de redimensionare.*/
void add_book(hashtable_t* library)
{
	char book_name[MAX_BOOK_NAME], key[MAX_STRING], value[MAX_STRING];
	int nr;

	/*Este alocata memorie pentru o noua carte si sunt initializate
	  caracteristicile cartii.*/
	book_info_t *new = malloc(sizeof(book_info_t));
	new->availability = 1;
	new->sum_grades = 0;
	new->nr_borrows = 0;
	new->purchase = 0;

	/*Este creat un nou hashtable in care vor fi stocate datele efective ale
	  cartii: definitiile.*/
	new->book = ht_create(HMAX, hash_function, compare_function);

	/*Este citit numarul de perechi cheie-valoare ce urmeaza a fi introduse
	  in hashtable.*/
	get_book_name(book_name);
	scanf("%d", &nr);

	/*Daca exista deja cartea, definitiile sale vor fi stearse, urmand
	  a fi actualizate.*/
	if (ht_has_key(library, book_name))
		ht_remove_entry(library, book_name, 1);

	/*Sunt citite si adugate in hashtable perechile cheie-valoare.*/
	for (int i = 0; i < nr; i++) {
		scanf("%s", key);
		scanf("%s", value);

		ht_put(new->book, key, strlen(key) + 1, value, strlen(value) + 1);
		verify_load_factor(new->book, 0, 0);
	}

	/*Noua carte este adaugata in biblioteca.*/
	ht_put(library, book_name, strlen(book_name) + 1, new, sizeof(book_info_t));
	free(new);
}

/*Functie ce afiseaza detaliile unei anumite carti. Daca aceasta carte nu
  exista in biblioteca, se va afisa un mesaj specific.*/
void get_book(hashtable_t* library)
{
	char book_name[MAX_BOOK_NAME], buff[MAX_STRING_SIZE];
	get_book_name(book_name);

	/*Daca exista cartea cu numele dorit in biblioteca, se vor afisa detaliile
	  sale: numele, ratingul si numarul de imprumuturi finalizate.
	  In caz contrar, se afiseaza un mesaj sugestiv.*/
	if (ht_has_key(library, book_name)) {
		book_info_t *book_info = ht_get(library, book_name);
		double rating = 0;
		if (book_info->purchase)
			rating = (double)book_info->sum_grades / (double)book_info->purchase;

		strcpy(buff, book_name  + 1);
		strcpy(book_name, buff);
		book_name[strlen(book_name) - 1] = '\0';

		printf("Name:%s ", book_name);
		printf("Rating:%.3f ", rating);
		printf("Purchases:%d \n", book_info->purchase);
	} else {
		printf("The book is not in the library.\n");
	}
}

/*Functie ce sterge cartea cu numele dat din biblioteca. Daca aceasta carte nu
 exista se va afisa un mesaj specific.*/
void remove_book(hashtable_t *library)
{
	char book_name[MAX_BOOK_NAME];
	get_book_name(book_name);

	if (ht_has_key(library, book_name)) {
		ht_remove_entry(library, book_name, 1);
	} else {
		printf("The book is not in the library.\n");
	}
}

/*Functie ce elibereaza memoria ocupata de intreaga biblioteca.
  Se elibereaza memoria ocupata de fiecare carte, apoi cea ocupata de liste,
  iar in final, se elibereaza biblioteca.*/
void exit_library(hashtable_t *library)
{
	for (int i = 0; i < library->hmax; i++) {
				list_t *list = library->buckets[i];
				if (list->size) {
					node_t *node = list->head;
					while (node) {
						free((*(info*)node->data).key);

						book_info_t *book_info = (*(info*)node->data).value;
						hashtable_t *book_hasht = book_info->book;
						ht_free(book_hasht);
						free(book_info);

						node = node->next;
					}
				}
				ll_free(&list);
	}
	free(library->buckets);
	free(library);
}
