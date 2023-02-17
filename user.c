// Copyright (C) Dragusin Daniela-Alexandra (311CA / 2021-2022)
#include "user.h"

/*Functie ce adauga un nou utilizator in sistem. Verific daca exista deja
  sau daca a fost banat. In caz contrar, este creat un nou utilizator si
  este adaugat in hashtable. Redimensionez hashtable-ul daca este nevoie.*/
void add_user(hashtable_t *community)
{
	char user_name[MAX_USER_NAME];
	scanf("%s", user_name);
	user_t *user;

	/*Verific daca utilizatorul exista deja sau daca a fost banat. In caz
	  afirmativ, afisez mesaje corespunzatoare si ies din functie.*/
	if (ht_has_key(community, user_name)) {
		user = ht_get(community, user_name);
		if (user) {
			printf("User is already registered.\n");
			return;
		} else if (user->banned) {
			printf("You are banned from this library.\n");
		}
		return;
	}

	/*Aloc memorie pentru un nou utilizator si initializez caracteristicile
	  acestuia.*/
	user = malloc(sizeof(user_t));
	user->points = 100;
	user->days_borrowed = 0;
	strcpy(user->book_borrowed, "");
	user->banned = 0;

	/*Adaug noul utilizator in hashtable.*/
	ht_put(community, user_name, strlen(user_name) + 1, user, sizeof(user_t));

	/*Redimensionez hashtable-ul daca este necesar.*/
	verify_load_factor(community, 0, 1);
	free(user);
}

/*Functie ce atribuie unui utilizator o anumita carte. Verific daca
  utilizatorul este banat sau a imprumutat deja o carte. In caz contrar,
  ii atribui cartea dorita si marchez cartea ca fiind indisponibila.*/
void borrow(hashtable_t *community, hashtable_t *library)
{
	char user_name[MAX_USER_NAME], book_name[MAX_BOOK_NAME];
	int days;

	scanf("%s", user_name);
	get_book_name(book_name);
	scanf("%d", &days);

	/*Verific daca utilizatorul este banat sau daca a imprumutat deja o carte.
	  In caz afirmativ, afisez mesaje corespunzatoare si ies din functie.*/
	if (ht_has_key(community, user_name)) {
		user_t *user = ht_get(community, user_name);

		if (user->banned) {
			printf("You are banned from this library.\n");
			return;
		}

		if (strcmp(user->book_borrowed, "")) {
			printf("You have already borrowed a book.\n");
			return;
		}

		if (ht_has_key(library, book_name)) {
			book_info_t *book = ht_get(library, book_name);

			/*Verific daca este disponibila cartea dorita. In caz afirmativ,
			  utilizatorului ii este atribuita cartea, iar aceasta este marcata
			  ca fiind indisponibila.*/
			if (book->availability) {
				if (strcmp(user->book_borrowed, "")) {
					printf("You have already borrowed a book.\n");
					return;
				} else {
					strcpy(user->book_borrowed, book_name);
					user->days_borrowed = days;
					book->availability = 0;
					book->nr_borrows++;
				}
			} else {
				printf("The book is borrowed.\n");
				return;
			}
		} else {
			printf("The book is not in the library.\n");
			return;
		}
	} else {
		printf("You are not registered yet.\n");
		return;
	}
}

/*Functie prin care un utilizator returneaza o carte. Acesta specifica cate
  zile au trecut de la imprumut si ofera o nota cartii. Cartea este marcata
  ca fiind disponibila si ii sunt actualizate caracteristicile.*/
void return_book(hashtable_t *community, hashtable_t *library)
{
	char user_name[MAX_USER_NAME], book_name[MAX_BOOK_NAME];
	int days, rating;

	scanf("%s", user_name);
	get_book_name(book_name);
	scanf("%d", &days);
	scanf("%d", &rating);

	/*Caut utilizatorul cu numele precizat.*/
	user_t *user = ht_get(community, user_name);

	if (!user->banned) {
		if (!strcmp(user->book_borrowed, book_name)) {
			/*Caut cartea ce a fost returnata, o marchez ca fiind disponibila
			  si ii actualizez detaliile.*/
			book_info_t *book = ht_get(library, book_name);
			book->availability = 1;
			book->sum_grades += rating;
			book->purchase++;
			strcpy(user->book_borrowed, "");

			/*In functie de numarul de zile dupa care a fost returnata cartea,
			  utilizatorului i se pot aduna sau scadea puncte.
			  Daca punctajul devine negativ, utilizatorul este banat.*/

			if (days > user->days_borrowed)
				user->points -= 2 * (days - user->days_borrowed);

			if (days < user->days_borrowed)
				user->points += user->days_borrowed - days;

			if (user->points < 0) {
				user->days_borrowed = 0;
				user->banned = 1;
				printf("The user %s has been banned from this library.\n", user_name);
				return;
			}
		} else {
			printf("You didn't borrow this book.\n");
			return;
		}
	} else {
		printf("You are banned from this library.\n");
	}
}

/*Functie prin care utilizatorul pierde o carte. Cartea este stearsa definitiv
  din biblioteca, iar utilizatorului ii sunt scazute 50 de puncte.*/
void lost(hashtable_t *community, hashtable_t *library)
{
	char user_name[MAX_USER_NAME], book_name[MAX_BOOK_NAME];

	scanf("%s", user_name);
	get_book_name(book_name);

	if (ht_has_key(community, user_name)) {
		user_t *user = ht_get(community, user_name);

		if (user->banned) {
			printf("You are banned from this library.\n");
			return;
		}

		/*Cartea este stearsa din biblioteca.*/
		ht_remove_entry(library, book_name, 1);

		/*Utilizatorului ii sunt sczaute 50 de puncte.*/
		user->points -= 50;
		strcpy(user->book_borrowed, "");
		user->days_borrowed = 0;

		/*Daca punctajul utilizatorului devine negativ, acesta este banat.*/
		if (user->points < 0) {
			user->banned = 1;
			printf("The user %s has been banned from this library.\n", user_name);
		}
	} else {
		printf("You are not registered yet.\n");
	}
}

/*Functie ce elibereaza memoria ocupata de hashtable-ul cu utilizatori.*/
void exit_community(hashtable_t *community)
{
	ht_free(community);
}
