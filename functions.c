// Copyright (C) Dragusin Daniela-Alexandra (311CA / 2021-2022)
#include "functions.h"

/*Functie ce citeste numele unei carti. Numele este citit inclusiv cu " ".
  Numarul 34 reprezinta codul ASCII al ", cu ajutorul caruia am realizat
  conditia de oprire a citirii.*/
void get_book_name(char* book_name)
{
	char buff[MAX_STRING_SIZE];
	scanf("%s", buff);
	strcpy(book_name, buff);

	while (buff[strlen(buff) - 1] - 34) {
		scanf("%s", buff);
		strcat(book_name, " ");
		strcat(book_name, buff);
	}
}

/*Functie ce verifica gradul de umplere al unui hashtable. Daca factorul
  depaseste valoarea 1, se realizeaza operatia de redimensionare.
  Am considerat cazuri speciale pentru biblioteca, carte si utilizator.*/
void verify_load_factor(hashtable_t* ht, int library, int user)
{
	int factor = ht->size / ht->hmax;
	if (factor <= 1)
		return;

	if (factor > 1) {
		/*Este creat un nou hashtable, de dimensiune maxima dubla fata de
		  cel initial.*/
		hashtable_t *new_ht = ht_create(2 * ht->hmax, hash_function,
							compare_function);

		/*Sunt introduse datele din cel vechi in cel nou, pentru fiecare caz.
		  Datele sunt sterse din hashtable-ul initial.*/
		for (int i = 0; i < ht->hmax; i++) {
			list_t *list = ht->buckets[i];
			if (list->size) {
				node_t *node = list->head;
				while (node) {
					info *data = node->data;
					if (!library) {
						if (user) {
							user_t *user = data->value;
							ht_put(new_ht, (char*)data->key, strlen(data->key) + 1,
								user, sizeof(user_t));
						} else {
							ht_put(new_ht, (char*)data->key, strlen(data->key) + 1,
								(char*)data->value, strlen(data->value) + 1);
						}
						node = node->next;
						ht_remove_entry(ht, data->key, 0);
					} else {
						book_info_t *book = data->value;
						ht_put(new_ht, (char*)data->key, strlen(data->key) + 1,
								book, sizeof(book_info_t));
						node = node->next;
						ht_remove_entry(ht, data->key, 1);
					}
				}
			}
			ll_free(&ht->buckets[i]);
		}

		free(ht->buckets);

		/*Hashtable-ul initial preia datele si caracteristicile celui nou.*/
		ht->size = new_ht->size;
		ht->hmax = new_ht->hmax;
		ht->buckets = new_ht->buckets;

		free(new_ht);
	}
}

/*Functie ce realizeaza un clasament al cartilor in functie de rating.
  Este folosit un vector in care sunt adaugate informatiile tuturor cartilor
  din biblioteca. Este sortat vectorul descrescator dupa rating. In caz de
  egalitate, vectorul este sortat dupa numarul de review-uri sau in ultim caz,
  lexicografic dupa numele cartii.
  Sunt printate informatiile despre fiecare carte din vectorul sortat.*/
void top_books(hashtable_t *library)
{
	/*Este alocata memorie pentru un vector in care adaug informatii despre
	  toate cartile din biblioteca.*/
	info *array = malloc(library->size * sizeof(info));

	int nr = 0;
	for (int i = 0; i < library->hmax; i++) {
		list_t *list = library->buckets[i];
		if (list->size) {
			node_t *node = list->head;
			while (node) {
				array[nr++] = *(info*)node->data;
				node = node->next;
			}
		}
	}

	/*Este calculat rating-ul cartilor prin impartirea sumei tututor notelor
	  la numarul de imprumuturi. Vectorul este sortat descrescator.*/
	for (int i = 0; i < nr; i++) {
		for (int j = i + 1; j < nr; j++) {
			book_info_t *book1 = array[i].value;
			book_info_t *book2 = array[j].value;

			double rating1 = 0;
			double rating2 = 0;

			if (book1->sum_grades != 0 && book1->purchase != 0)
				rating1 = (double)book1->sum_grades / (double)book1->purchase;

			if (book2->sum_grades != 0 && book2->purchase != 0)
				rating2 = (double)book2->sum_grades / (double)book2->purchase;

			if (rating1 < rating2) {
				info aux = array[i];
				array[i] = array[j];
				array[j] = aux;
			}
			if (rating1 == rating2) {
				if (book1->purchase < book2->purchase) {
					info aux = array[i];
					array[i] = array[j];
					array[j] = aux;
				}
				if (book1->purchase == book2->purchase) {
					if (strcmp(array[j].key, array[i].key) < 0) {
						info aux = array[i];
						array[i] = array[j];
						array[j] = aux;
					}
				}
			}
		}
	}

	/*Este printat clasamentul.*/
	printf("Books ranking:\n");

	for (int i = 0; i < nr; i++) {
		int purchases = ((book_info_t*)array[i].value)->purchase;
		double grades = ((book_info_t*)array[i].value)->sum_grades;
		double nr_borrows = ((book_info_t*)array[i].value)->nr_borrows;
		char book_name[MAX_BOOK_NAME], buff[MAX_BOOK_NAME];
		double rating = 0.0;

		if (nr_borrows && purchases)
			rating = grades / purchases;

		strcpy(book_name, (char*)array[i].key);
		strcpy(buff, book_name  + 1);
		strcpy(book_name, buff);
		book_name[strlen(book_name) - 1] = '\0';

		printf("%d. Name:%s Rating:%.3f Purchases:%d\n", i + 1, book_name,
			rating, purchases);
	}

	/*Este eliberata memoria ocupata de vector.*/
	free(array);
}

/*Functie ce realizeaza un clasament al utilizatorilor in functie de punctajul
  fiecaruia. Este folosit un vector in care sunt introduse informatiile
  tuturor utilizatorilor. Vectorul este sortat descrescator dupa punctaj,
  iar in caz de egalitate este sortat lexicografic dupa nume.
  Sunt printate informatiile utilizatorilor din vectorul sortat.*/
void top_users(hashtable_t *community)
{
	/*Este alocata memorie pentru un vector in care sunt adaugate informatiile
	  tuturor utilizatorilor.*/
	info *array = malloc(community->size * sizeof(info));

	int nr = 0;
	for (int i = 0; i < community->hmax; i++) {
		list_t *list = community->buckets[i];
		if (list->size) {
			node_t *node = list->head;
			while (node) {
				info *info = node->data;
				user_t *user = info->value;

				if (!user->banned)
					array[nr++] = *info;

				node = node->next;
			}
		}
	}
	/*Este sortat vectorul descrescator dupa punctaj. Daca utilizatorii au
	  acelasi numar de puncte, vor fi sortati lexicografic dupa nume.*/
	for (int i = 0; i < nr - 1; i++) {
		for (int j = i + 1; j < nr; j++) {
			user_t *user1 = array[i].value;
			user_t *user2 = array[j].value;

			if (user1->points < user2->points) {
				info aux = array[i];
				array[i] = array[j];
				array[j] = aux;
			}
			if (user1->points == user2->points) {
				if (strcmp(array[i].key, array[j].key) > 0) {
					info aux = array[i];
					array[i] = array[j];
					array[j] = aux;
				}
			}
		}
	}

	/*Este printat clasamantul.*/
	printf("Users ranking:\n");

	for (int i = 0; i < nr; i++) {
		info info = array[i];
		user_t *user = info.value;
		printf("%d. Name:%s Points:%d\n", i + 1, (char*)(info.key), user->points);
	}

	/*Este eliberata memoria ocupata de vector.*/
	free(array);
}
