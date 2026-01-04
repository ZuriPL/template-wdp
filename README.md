# Projekt

Przy każdym uruchomieniu terminal'a należy uruchomić komendę `$ source config`, która wczytuje zmienne środowiskowe z pliku `config`. Cała konfiguracja odbywa się w tym pliku.

Domyślnie:
- kod znajduje się w folderze `/src`
- make tworzy plik wykonywalny `a.out` w głównym folderze

## 1. makefile

`make` | `make all` - puszcza wszystko

`make test` - puszcza testy <br>
`make valgrind` - puszcza testy z valgrind'em

## 2. tester.sh (alias: test)

Użycie: <br>
`$ test <paczka> <limit> <valgrind[true|false]>`

np. `$ test small 100 false` odpali 100 testów z paczki `small` bez valgrind'a



