// Copyright (C) 2022 Dragusin Daniela-Alexandra (311CA / 2021-2022)
#ifndef USER_H_
#define USER_H_

#include "utils.h"
#include "functions.h"

/*Functie ce adauga un nou utilizator in sistem. Verific daca exista deja
  sau daca a fost banat. In caz contrar, este creat un nou utilizator si
  este adaugat in hashtable. Redimensionez hashtable-ul daca este nevoie.*/
void add_user(hashtable_t *community);

/*Functie ce atribuie unui utilizator o anumita carte. Verific daca
  utilizatorul este banat sau a imprumutat deja o carte. In caz contrar,
  ii atribui cartea dorita si marchez cartea ca fiind indisponibila.*/
void borrow(hashtable_t *community, hashtable_t *library);

/*Functie prin care un utilizator returneaza o carte. Acesta specifica cate
  zile au trecut de la imprumut si ofera o nota cartii. Cartea este marcata
  ca fiind disponibila si ii sunt actualizate caracteristicile.*/
void return_book(hashtable_t *community, hashtable_t *library);

/*Functie prin care utilizatorul pierde o carte. Cartea este stearsa definitiv
  din biblioteca, iar utilizatorului ii sunt scazute 50 de puncte.*/
void lost(hashtable_t *community, hashtable_t *library);

/*Functie ce elibereaza memoria ocupata de hashtable-ul cu utilizatori.*/
void exit_community(hashtable_t *community);

#endif  // USER_H_
