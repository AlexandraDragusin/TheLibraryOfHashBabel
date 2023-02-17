// Copyright (C) 2022 Dragusin Daniela-Alexandra (311CA / 2021-2022)
#ifndef BOOK_H_
#define BOOK_H_

#include "utils.h"
#include "functions.h"

/*Functie ce adauga o noua definitie in cartea specificata. Daca aceasta carte
  nu exista in biblioteca, se afiseaza un mesaj specific.
  Daca exista cartea, se adauga in hashtable-ul sau noua pereche
  cheie-valoare. Se redimensioneaza hashtable-ul daca este necesar.*/
void add_def(hashtable_t *library);

/*Functie ce afiseaza valoarea definitiei corespunzatoare din cartea
  specificata. In cazul in care nu exista cartea respectiva in biblioteca,
  se va afisa un mesaj specific.*/
void get_def(hashtable_t *library);

/*Functie ce sterge o definitie specificata dintr-o anumita carte.
  In cazul in care nu exista cartea in biblioteca sau nu exista definitia
  in carte, se vor afisa mesaje corespunzatoare.
  In caz contrar, va fi stearsa perechea cheie-valoare din cartea specificata*/
void rmv_def(hashtable_t *library);

#endif  // BOOK_H_
