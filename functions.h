// Copyright (C) 2022 Dragusin Daniela-Alexandra (311CA / 2021-2022)
#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "utils.h"

/*Functie ce citeste numele unei carti. Numele este citit inclusiv cu " ".
  Numarul 34 reprezinta codul ASCII al ", cu ajutorul caruia a fost realizata
  conditia de oprire a citirii.*/
void get_book_name(char* book_name);

/*Functie ce verifica gradul de umplere al unui hashtable. Daca factorul
  depaseste valoarea 1, se realizeaza operatia de redimensionare.
  Am considerat cazuri speciale pentru biblioteca, carte si utilizator.*/
void verify_load_factor(hashtable_t* ht, int library, int user);

/*Functie ce realizeaza un clasament al cartilor in functie de rating.
  Este folosit un vector in care sunt adaugate informatiile tuturor cartilor
  din biblioteca. Este sortat vectorul descrescator dupa rating. In caz de
  egalitate, vectorul este sortat dupa numarul de imprumuturi finalizate sau
  in ultim caz, lexicografic dupa numele cartii.
  Sunt printate informatiile despre fiecare carte din vectorul sortat.*/
void top_books(hashtable_t *library);

/*Functie ce realizeaza un clasament al utilizatorilor in functie de punctajul
  fiecaruia. Este folosit un vector in care sunt introduse informatiile
  tuturor utilizatorilor. Vectorul este sortat descrescator dupa punctaj,
  iar in caz de egalitate este sortat lexicografic dupa nume.
  Sunt printate informatiile utilizatorilor din vectorul sortat.*/
void top_users(hashtable_t *community);

#endif  // FUNCTIONS_H_
