// Copyright (C) 2022 Dragusin Daniela-Alexandra (311CA / 2021-2022)
#ifndef LIBRARY_H_
#define LIBRARY_H_

#include "utils.h"
#include "functions.h"

/*Functie ce adauga o noua carte in biblioteca. Este alocata memorie pentru o
  noua carte. Sunt initializate caracteristicile cartii. Sunt citite si
  introduse definitiile in carte. Cartea este adaugata in biblioteca.
  Este verificata conditia de redimensionare.*/
void add_book(hashtable_t* library);

/*Functie ce afiseaza detaliile unei anumite carti. Daca aceasta carte nu
  exista in biblioteca, se va afisa un mesaj specific.*/
void get_book(hashtable_t* library);

/*Functie ce sterge cartea cu numele dat din biblioteca. Daca aceasta carte nu
 exista se va afisa un mesaj specific.*/
void remove_book(hashtable_t *library);

/*Functie ce elibereaza memoria ocupata de intreaga biblioteca.
  Se elibereaza memoria ocupata de fiecare carte, apoi cea ocupata de liste,
  iar in final, se elibereaza biblioteca.*/
void exit_library(hashtable_t *library);

#endif  // LIBRARY_H_
