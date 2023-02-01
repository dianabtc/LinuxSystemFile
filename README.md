# LinuxSystemFile
Proiectul prezinta implementarea unui Linux-like CLI pentru a manipula
un sistem de fisiere.

In fisierul header.h, am inceput prin a realiza structurile necesare:

->TDirectory, structura pentru director

->TFile, structura pentru fisier

In system_file.c am realizat doua functii ce realizeaza alocarea de memorie pentru
aceste doua structuri. Aloc memorie pentru o structura de tip TFile si setez
campul "name" si "parent" al acesteia, iar pentru TDirectory procedez similar.
In main am realizat citirea unor comenzi de la tastatura cu ajutorul functiei
"getline". Inainte, am initializat directorul "root" ce are parintele NULL si
am construit un vector de structuri TDirectory* unde voi retine directoarele
curente in care ma aflu, pe care il voi folosi ulterior in apelul comenzii "find".
Pe prima pozitie din acest vector va fi root-ul. 
Am implementat functii pentru comenzi astfel:

-> TOUCH

In cadrul acestei comenzi m-am folosit de functia "directory_exists", ce verifica
daca exista un director cu numele fisierului pe care vrem sa-l cream in ierarhia
de subdirectoare (compara numele primit ca parametru si daca nu sunt egale cauta
in arborele stang, respectiv drept) si retine rezultatul cautarii in variabila
"res". Daca exista (res este 1), afisam mesajul respectiv, daca nu, apelam functia
"insert_file" pentru arborele de fisiere al directorului curent, fisierul respectiv
avand ca parinte directorul curent in care ne aflam. In functia de insert_file
verific de asemenea daca deja exista un fisier cu numele respectiv; in caz
afirmativ, se afiseaza un mesaj si inserarea nu se mai realizeaza.
Functia este dupa modelul celei din laborator: Inserare_ABC.

-> MKDIR

In cadrul acestei comenzi m-am folosit de functia "file_exists", ce verifica in
mod similar cu cea pt directoare existenta unui fisier cu numele directorului
pe care vrem sa-l cream in arborele de fisiere. Daca exista un astfel de fisier,
se afiseaza un mesaj. Daca nu, apelam functia "insert_directory" ce este tot
dupa modelul de Inserare_ABC in arborele de subdirectoare al directorului curent,
avand ca parinte tot directorul curent. In aceasta functie verific daca exista
deja un director cu numele respectiv; in caz afirmativ, se afiseaza un mesaj
si inserarea nu se mai realizeaza.

-> LS

In aceasta comanda ma folosesc de functiile "print_directories" si "print_files"
ce afiseaza arborii de subdirectoare si fisiere in ordine SRD.

-> RM

In aceasta comanda, verific initial daca exista fisierul cu numele respectiv
folosindu-ma de functia "file_exists". Daca nu exista, afisez mesajul. Daca exista,
apelez functia "remove_file" pe arborele de fisiere al directorului curent.
Aceasta este functia de stergere a unui nod dintr-un arbore binar de cautare, ce
functioneaza astfel: daca numele este mai mic, apelam functia pe subarborele
stang; in caz contrar, apelam pe subarborele drept. Apoi verificam daca nodul
are doar un copil sau doi. Daca are doi, obtinem valoarea minima din subarborele
drept, copiem numele si stergem nodul.
(https://www.geeksforgeeks.org/binary-search-tree-set-2-delete/)

-> RMDIR

Aceasta comanda functioneaza la fel ca RM, numai ca verific daca exista directorul
cu functia "directory_exists", dupa care apelez functia "remove_directory" pe
arborele de subdirectoare al directorului curent.

-> CD

In cadrul acestei comenzi, am tratat separat cazul cand numele directorului este
"..", adica atunci cand va trebui sa ne deplasam pe parintele directorului curent.
In celalalt caz, mai intai verific daca directorul curent este "root". Verific
existenta directorului in subdirectoarele din root, in caz ca nu exista afisez
un mesaj. In caz ca exista setez directorul curent cu directorul gasit cu ajutorul
functiei "search_directory", ce este asemanatoare cu "directory_exists" numai ca
returneaza directorul cu nume respectiv. Daca directorul curent nu este root,
caut in subdirectoarele parintelui directorului curent si in subdirectoarele
directorului curent. Daca nu il gasesc, afisez mesajul, iar apoi in functie de
unde am gasit directorul respectiv, apelez functia "search_directory".
De fiecare data cand ma mut in alt director, retin directorul curent in vectorul
de TDirectory*.

-> PWD

In cadrul acestei comenzi m-am folosit de functia f_pwd ce este apelata pe
directorul curent. In cadrul acestei functii, numar initial numarul de parinti
incepand din directorul in care ma aflu. Apoi, imi aloc un vector unde voi 
retine numele directoarelor parinti. Copiez in acest vector numele parintilor,
avansand de fiecare data la urmatorul parinte din arbore si la final pun si
root-ul. Apoi, parcurg acest vector in ordine inversa si afisez, obtinand astfel
calea directorului curent.

-> FIND

In aceasta comanda ma folosesc de vectorul de TDirectory* pe care l-am creat
pe parcurs. Astfel, pentru "find -f" apelez functia "file_exists" pe fiecare
element din vector (adica pentru fiecare director care a fost "curent" la un
moment dat) pe arborele de fisiere. Daca am gasit fisierul respectiv, afisez
mesajul si calea acestuia cu functia f_pwd. Am procedat asemanator pentru find
-d, numai ca am apelat functia "directory_exists" pe arborele de subdirectoare
al fiecarui element din vector. In caz ca am gasit directorul respectiv, am
afisat mesajul, calea acestuia si numele directorului.
