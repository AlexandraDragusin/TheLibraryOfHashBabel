// Copyright (C) Dragusin Daniela-Alexandra (311CA / 2021-2022)
#include "utils.h"

/*Functiile referitoare la liste inlantuite sunt preluate din scheletul
  laboratoului 4 - Hashmap.*/

/*Functie ce creeaza o lista inlantuita.*/
list_t* ll_create(int data_size)
{
	list_t* ll;

	ll = malloc(sizeof(*ll));

	ll->head = NULL;
	ll->data_size = data_size;
	ll->size = 0;

	return ll;
}

/*Functie ce adauga un nod intr-o lista pe pozitia n.*/
void ll_add_nth_node(list_t* list, int n, const void* new_data)
{
	node_t *prev, *curr;
	node_t* new_node;

	if (!list)
		return;

	if (n > list->size)
		n = list->size;

	curr = list->head;
	prev = NULL;

	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	new_node = malloc(sizeof(*new_node));
	new_node->data = malloc(list->data_size);
	memcpy(new_node->data, new_data, list->data_size);

	new_node->next = curr;
	if (!prev)
		list->head = new_node;
	else
		prev->next = new_node;

	list->size++;
}

/*Functie ce sterge nodul de pe pozitia n din lista.*/
node_t* ll_remove_nth_node(list_t* list, int n)
{
	node_t *prev, *curr;

	if (!list || !list->head)
		return NULL;

	if (n > list->size - 1)
		n = list->size - 1;

	curr = list->head;
	prev = NULL;

	while (n > 0) {
		prev = curr;
		curr = curr->next;
		--n;
	}

	if (!prev)
		list->head = curr->next;
	else
		prev->next = curr->next;

	list->size--;

	return curr;
}

/*Functie ce intoarce dimensiunea listei.*/
int ll_get_size(list_t* list)
{
	if (!list)
	return -1;

	return list->size;
}

/*Functie ce elibereaza memoria ocupata de o lista.*/
void ll_free(list_t** pp_list)
{
	node_t* currNode;

	if (!pp_list || !*pp_list)
		return;

	while (ll_get_size(*pp_list) > 0) {
		currNode = ll_remove_nth_node(*pp_list, 0);
		free(currNode->data);
		currNode->data = NULL;
		free(currNode);
		currNode = NULL;
	}

	free(*pp_list);
	*pp_list = NULL;
}

/*Functie ce compara doua siruri de caractere.*/
unsigned int compare_function(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

/*Functie ce calculeaza hash-ul unul sir de caractere.
  Credits: http://www.cse.yorku.ca/~oz/hash.html .*/
unsigned int hash_function(void *a)
{
	/*
	 * Credits: http://www.cse.yorku.ca/~oz/hash.html
	 */
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned int hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

	return hash;
}

/*Functie ce creeaza un nou hashtable. Este alocata memorie pentru noul
  hashtable si ii sunt initializate caracteristicile.*/
hashtable_t* ht_create(int hmax, unsigned int (*hash_function)(void*),
		unsigned int (*compare_function)(void*, void*))
{
	hashtable_t *ht = (hashtable_t*)malloc(sizeof(hashtable_t));
	ht->buckets = (list_t**)malloc(hmax * sizeof(list_t*));

	for(int i = 0; i < hmax; i++)
		ht->buckets[i] = ll_create(sizeof(info));

	ht->compare_function = compare_function;
	ht->hash_function = hash_function;
	ht->size = 0;
	ht->hmax = hmax;

	return ht;
}

/*Functie ce verifica daca exista in hashtable intrarea cu cheia respectiva.
  Functia intoarce 1 in caz afirmativ, 0 in caz contrar.*/
int ht_has_key(hashtable_t *ht, void *key)
{
	int index = ht->hash_function(key) % ht->hmax;

	list_t *bucket = ht->buckets[index];
	if (bucket->size) {
		node_t *node = bucket->head;

		while(node) {
			if(!ht->compare_function(key, (*(info *)node->data).key))
				return 1;
			node = node->next;
		}
	}

	return 0;
}

/*Functie ce afiseaza valoarea corespunzatoare unei anumite chei
  din hashmap.*/
void* ht_get(hashtable_t *ht, void *key)
{
	unsigned int index = (ht->hash_function)(key) % ht->hmax;
	list_t *bucket = (ht->buckets)[index];

	node_t *node = bucket->head;

	while (node) {
		if (!ht->compare_function(key, (*(info *)node->data).key))
			return (*(info *)node->data).value;
		node = node->next;
	}

	return NULL;
}

/*Functie ce adauga o noua intrare in hashtable. Este alocata memorie
  pentru noua cheie si noua valoare si sunt inserate in hashtable.
  Este incrementata dimensiunea hashtable-ului.*/
void ht_put(hashtable_t *ht, void *key, int key_size,
	void *value, int value_size)
{
	/*Este calculat indexul listei folosind functia de hash.*/
	unsigned int index = ht->hash_function(key) % ht->hmax;
	list_t *bucket = ht->buckets[index];

	if (ht_has_key(ht, key))
		ht_remove_entry(ht, key, 0);

	/*Este alocata memorie pentru o noua structura de tip info.*/
	info* data = (info*)malloc(sizeof(info));

	/*Este alocata memorie pentru noua cheie si este copiata in ea cheia
	  ce se doreste a fi inserata.*/
	data->key = malloc(key_size);
	memcpy(data->key, key, key_size);

	/*Este alocata memorie pentru noua valoare si este copiata in ea valoarea
	  ce se doreste a fi inserata.*/
	data->value = malloc(value_size);
	memcpy(data->value, value, value_size);

	/*Este adaugat un nou nod in lista.*/
	ll_add_nth_node(bucket, 0, data);

	/*Este incrementata dimensiunea hashtable-ului.*/
	ht->size++;
	free(data);
}

/*Functie ce elibereaza memoria ocupata de un hashtable.*/
void ht_free(hashtable_t *ht)
{
	node_t *garbage;

	/*Este parcurs vectorul de bucket-uri.*/
	for(int i = 0; i < ht->hmax; i++) {
		list_t *bucket = ht->buckets[i];

		/*Este parcursa fiecare lista (bucket) si este eliberata memoria
		  ocupata de fiecare nod.*/
		if(bucket->size) {
			garbage = bucket->head;

			while (garbage) {
				/*Sunt eliberate cheia si valoarea continute de nodul
				  respectiv.*/
				free((*(info*)garbage->data).value);
				free((*(info*)garbage->data).key);
				garbage = garbage->next;
			}
		}
		ll_free(&bucket);
	}

	free(ht->buckets);
	free(ht);
}

/*Functie ce sterge intrarea cu cheia respectiva din hashtable.
  Este considerat cazul in care elementul contine un alt hashtable
  (cazul bibliotecii).*/
void ht_remove_entry(hashtable_t *ht, void *key, int hasht)
{
	/*Este calculat indexul utilizand functia de hash.*/
	unsigned int index = (ht->hash_function)(key) % ht->hmax;

	/*Este aleasa lista de la indexul respectiv.*/
	list_t *bucket = (ht->buckets)[index];
	node_t *node = bucket->head;

	int pos = 0;

	/*Pentru fiecare nod (element) din lista, sunt sterse cheia si valoarea.
	  Daca valoarea elementului este un alt hashtable, se apeleaza functia
	  ce sterge un hashtable intreg.*/

	while(node) {
		if(!ht->compare_function(key, (*(info *)node->data).key)) {
			free((*(info*)node->data).key);
			if (hasht) {
				book_info_t *book_info = (*(info*)node->data).value;
				ht_free(book_info->book);
				free(book_info);
			} else {
				free((*(info*)node->data).value);
			}

			node_t *garbage = ll_remove_nth_node(ht->buckets[index], pos);
			free(garbage->data);
			free(garbage);

			ht->size--;
			return;
		}

		node = node->next;
		pos++;
	}
}
