// Copyright (C) 2022 Dragusin Daniela-Alexandra (311CA / 2021-2022)
#ifndef UTILS_H_
#define UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING_SIZE	256
#define MAX_BOOK_NAME 40
#define MAX_USER_NAME 20
#define MAX_STRING 20
#define HMAX 10

/*Structurile specifice listei si hashtable-ului sunt preluate din
  rezolvarea laboratoului 4 - Hashmap.*/

typedef struct node_t
{
	void* data;
	struct node_t* next;
} node_t;

typedef struct list_t
{
	node_t* head;
	int data_size;
	int size;
} list_t;

typedef struct info
{
	void *key;
	void *value;
} info;

/*Structura generica ce defineste un hashtable.*/
typedef struct hashtable_t
{
	list_t **buckets;
	int size;
	int hmax;
	unsigned int (*hash_function)(void*);
	unsigned int (*compare_function)(void*, void*);
} hashtable_t;

/*Structura ce defineste o carte. Contine caracteristicile cartii:
  disponibilitatea, suma tuturor notelor primite, numarul de imprumuturi:
  realizate si finalizate si o structura de tip hashtable ce include
  totalitatea definitiilor (perechi chei-valoare).*/
typedef struct book_info_t
{
	int availability;
	int sum_grades;
	int nr_borrows;
	int purchase;
	hashtable_t *book;
} book_info_t;

/*Structura ce defineste un utilizator. Contine detalii despre acel utilizator:
  punctajul, ce carte a imprumutat si pentru cate zile, dar si daca a fost sau
  nu banat.*/
typedef struct user_t
{
	int points;
	int days_borrowed;
	char book_borrowed[MAX_BOOK_NAME];
	int banned;
} user_t;

/*Functie ce creeaza o lista inlantuita.*/
list_t* ll_create(int data_size);

/*Functie ce adauga un nod intr-o lista pe pozitia n.*/
void ll_add_nth_node(list_t* list, int n, const void* new_data);

/*Functie ce sterge nodul de pe pozitia n din lista.*/
node_t* ll_remove_nth_node(list_t* list, int n);

/*Functie ce intoarce dimensiunea listei.*/
int ll_get_size(list_t* list);

/*Functie ce elibereaza memoria ocupata de o lista.*/
void ll_free(list_t** pp_list);

/*Functie ce compara doua siruri de caractere.*/
unsigned int compare_function(void *a, void *b);

/*Functie ce calculeaza hash-ul unul sir de caractere.
  Credits: http://www.cse.yorku.ca/~oz/hash.html .*/
unsigned int hash_function(void *a);

/*Functie ce creeaza un nou hashtable. Este alocata memorie pentru noul
  hashtable si ii sunt initializate caracteristicile.*/
hashtable_t* ht_create(int hmax, unsigned int (*hash_function)(void*),
		unsigned int (*compare_function)(void*, void*));

/*Functie ce verifica daca exista in hashtable intrarea cu cheia respectiva.
  Functia intoarce 1 in caz afirmativ, 0 in caz contrar.*/
int ht_has_key(hashtable_t *ht, void *key);

/*Functie ce afiseaza valoarea corespunzatoare unei anumite chei
  din hashmap.*/
void* ht_get(hashtable_t *ht, void *key);

/*Functie ce adauga o noua intrare in hashtable. Este alocata memorie
  pentru noua cheie si noua valoare si sunt inserate in hashtable.
  Este incrementata dimensiunea hashtable-ului.*/
void ht_put(hashtable_t *ht, void *key, int key_size,
	void *value, int value_size);

/*Functie ce elibereaza memoria ocupata de un hashtable.*/
void ht_free(hashtable_t *ht);

/*Functie ce sterge intrarea cu cheia respectiva din hashtable.
  Este considerat cazul in care elementul contine un alt hashtable
  (cazul bibliotecii).*/
void ht_remove_entry(hashtable_t *ht, void *key, int hasht);

#endif  // UTILS_H_
