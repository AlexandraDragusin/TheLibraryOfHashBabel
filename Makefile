#Copyright (C) Dragusin Daniela-Alexandra (311CA / 2021-2022) */

build:
	gcc -Wall -Wextra -std=c99 -o main main.c utils.c functions.c library.c book.c user.c -lm

clean:
	-rm -f main
	-rm -f *~

.PHONY: pack clean