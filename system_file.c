#include "header.h"

/* functie care aloca o structura de tip TFile si seteaza campul name si
parintele director, iar pe restul cu NULL */
TFile* Alloc_file(char* name, TDirectory* parent) {
    TFile* file = (TFile*)malloc(sizeof(TFile));
    if (!file) {
        printf("Alloc failed!\n");
        return NULL;
    }
    file->name = malloc(strlen(name) + 1); //aloca spatiu pentru name
    if (!file->name) {
        free(file);
        return NULL;
    }
    strcpy(file->name, name); //copiaza name
    file->parent = (TDirectory*)malloc(sizeof(TDirectory));
    if (!file->parent) {
        free(file->name);
        free(file);
        return NULL;
    }
    file->parent = parent; //seteaza parintele
    file->left = NULL;
    file->right = NULL;
    return file;
}

/* functie care aloca o structura de tip TDirectory si seteaza campul name si
parinte, iar restul campurilor cu NULL */
TDirectory* Alloc_directory(char* name, TDirectory* parent) {
    TDirectory* directory = (TDirectory*)malloc(sizeof(TDirectory));
    if (!directory) {
        printf("Alloc failed!\n");
        return NULL;
    }
    directory->name = malloc(strlen(name) + 1); //aloca spatiu pentru name
    if (!directory->name) {
        free(directory);
        return NULL;
    }
    strcpy(directory->name, name); //copiaza name
    directory->parent = (TDirectory*)malloc(sizeof(TDirectory));
    if (!directory->parent) {
        free(directory->name);
        free(directory);
        return NULL;
    }
    directory->parent = parent;
    directory->directories = NULL;
    directory->files = NULL;
    directory->left = NULL;
    directory->right = NULL;
    return directory;
}

/* functie de inserare intr-un arbore binar de cautare pentru fisiere - lab */
int insert_file(TFile** files, char* name, TDirectory* parent) {
    TFile *aux, *p, *n;
    /* in cazul in care arborele este NULL */
    if (*files == NULL) {
        *files = Alloc_file(name, parent);
        if (!(*files)) {
            return 0;
        }
        return 1;
    }
    n = *files;
    while (n) {
        p = n; //retinem parintele nodului
        /* daca exista deja un fisier cu numele respectiv */
        if (strcmp(name, n->name) == 0) {
            printf("File %s already exists!\n", name); 
            return 0;
        }
        /* altfel, avansam in functie de ordinea lexicografica */
        if (strcmp(name, n->name) < 0) {
            n = n->left;
        } else {
            n = n->right;
        }
    }
    aux = Alloc_file(name, parent); //alocam noul fisier
    if (!aux) {
        return 0;
    }
    /* daca numele noului fisier este mai mic, il pun in arborele stang */
    /* daca numele noului fisier este mai mare, il pun in arborele drept */
    if (strcmp(name, p->name) < 0) {
        p->left = aux;
    } else {
        p->right = aux;
    }
    return 1;
}

/* functie de inserare intr-un arbore binar de cautare pentru directoare - lab */
int insert_directory(TDirectory** directories, char* name, TDirectory* parent) {
    TDirectory *aux, *p, *n;
    /* in cazul in care arborele este NULL */
    if (*directories == NULL) {
        *directories = Alloc_directory(name, parent); //alocam noul director radacina
        if (!(*directories)) {
            return 0;
        }
        return 1;
    }
    n = *directories;
    while (n) {
        p = n; //retinem parintele nodului
        /* daca exista deja un director cu numele respectiv */
        if (strcmp(name, n->name) == 0) {
            printf("Directory %s already exists!\n", name);
            return 0;
        }
        /* daca nu, facem avasnul corespunzator */
        if (strcmp(name, n->name) < 0) {
            n = n->left;
        } else {
            n = n->right;
        }
    }
    aux = Alloc_directory(name, parent); //aloc noul director
    if (!aux) {
        return 0;
    }
    /* daca numele noului director este mai mic, il punem in arborele stang */
    /* daca numele noului director este mai mare, il punem in arborele drept */
    if (strcmp(name, p->name) < 0) {
        p->left = aux;
    } else {
        p->right = aux;
    }
    return 1;
}

/* functie care verifica existenta unui fisier in arborele de fisiere - curs */
// 1 daca exista, 0 altfel
int file_exists(TFile* files, char* name) {
    if (!files) {
        return 0;
    }
    /* daca fisierul din arbore are acelasi nume cu fisierul cautat */
    if (strcmp(files->name, name) == 0) {
        return 1;
    }
    /* cautam in arborele stang */
    if (strcmp(name, files->name) < 0) {
        return file_exists(files->left, name);
    } else {
        /* cautam in arborele drept */
        return file_exists(files->right, name);
    }
}

/* functie care verifica existenta unui director in arborele de directoare */
// 1 daca exista, 0 altfel
int directory_exists(TDirectory* directories, char* name) {
    if (!directories) {
        return 0;
    }
    if (strcmp(directories->name, name) == 0) {
        return 1;
    }
    if (strcmp(name, directories->name) < 0) {
        return directory_exists(directories->left, name);
    } else {
        return directory_exists(directories->right, name);
    }
}

/* functie care cauta un director cu un anumit nume si il returneaza */
TDirectory* search_directory(TDirectory* directories, char* name) {
    if (!directories) {
        return NULL;
    }
    if (strcmp(directories->name, name) == 0) {
        return directories;
    }
    if (strcmp(name, directories->name) < 0) {
        return search_directory(directories->left, name);
    } else {
        return search_directory(directories->right, name);
    }
}

/* functie de stergere fisier cu un anumit nume din ierarhia de fisiere */
TFile* remove_file(TFile* files, char* name) {
    if (!files) {
        return files;
    }
    /* daca numele este mai mic, cautam in subarborele stang */
    /* daca numele este mai mare, cautam in subarborele drept */
    if (strcmp(name, files->name) < 0) {
        files->left = remove_file(files->left, name);
    } else if (strcmp(name, files->name) > 0) {
        files->right = remove_file(files->right, name);
    } else {
        /* daca nodul are doar un copil */
        if (files->left == NULL) {
            TFile* aux = files->right;
            free(files);
            return aux;
        } else if (files->right == NULL) {
            TFile* aux = files->left;
            free(files);
            return aux;
        }
        /* daca nodul are doi copii */
        /* obtinem valoarea minima din subarborele drept */
        /* copiem numele si stergem nodul */
        TFile* aux = files->right;
        while (aux && aux->left != NULL) {
            aux = aux->left;
        }
        strcpy(files->name, aux->name);
        files->right = remove_file(files->right, aux->name);
    }
    return files;
}

/* functie de stergere director din ierarhia de directoare */
TDirectory* remove_dir(TDirectory* directories, char* name) {
    if (!directories) {
        return directories;
    }
    /* daca numele este mai mic, cautam in subarborele stang */
    /* daca numele este mai mare, cautam in subarborele drept */
    if (strcmp(name, directories->name) < 0) {
        directories->left = remove_dir(directories->left, name);
    } else if (strcmp(name, directories->name) > 0) {
        directories->right = remove_dir(directories->right, name);
    } else {
        /* daca nodul are doar un copil */
        if (directories->left == NULL) {
            TDirectory* aux = directories->right;
            free(directories);
            return aux;
        } else if (directories->right == NULL) {
            TDirectory* aux = directories->left;
            free(directories);
            return aux;
        }
        /* daca nodul are doi copii */
        /* obtinem valoarea minima din subarborele drept */
        /* copiem numele si stergem nodul */
        TDirectory* aux = directories->right;
        while (aux && aux->left != NULL) {
            aux = aux->left;
        }
        strcpy(directories->name, aux->name);
        directories->right = remove_dir(directories->right, aux->name);
    }
    return directories;
}

/* functie care afiseaza arborele de fisiere in ordine lexicografica (SRD) */
void print_files(TFile* files) {
    if (!files) {
        return;
    }
    print_files(files->left);
    printf("%s ", files->name);
    print_files(files->right);
}

/* functie care afiseaza arborele de directoare in ordine lexicografica (SRD) */
void print_directories(TDirectory* directories) {
    if (!directories) {
        return;
    }
    print_directories(directories->left);
    printf("%s ", directories->name);
    print_directories(directories->right);
}

/* functie pentru comanda pwd, afiseaza calea directoarelor (parintii) */
void f_pwd(TDirectory* directory) {
    TDirectory* aux = directory;
    int size = 0;
    while (aux != NULL) {
        /* numaram parintii */
        size++;
        aux = aux->parent;
    }
    /* alocam un vector pentru numele parintilor */
    char** array = (char**)malloc(size * sizeof(char*));
    if (!array) {
        return;
    }
    int i = 0;
    for (i = 0; i < size; i++) {
        /* alocam memorie pentru fiecare nume */
        array[i] = (char*)malloc(50);
        if (!array[i]) {
            free(array);
            return;
        }
    }
    i = 0;
    while (directory->parent != NULL) {
        strcpy(array[i], directory->name); //punem numele parintelui in vector
        directory = directory->parent; //avansam la urmatorul parinte
        i++;
    }
    strcpy(array[i], directory->name); //punem root-ul
    /* parcurgem vectorul si afisam invers */
    int j;
    for (j = i; j >= 0; j--) {
        printf("/%s", array[j]);
    }
    free(array);
}

int main () {
    char* line = NULL;
    size_t len = 0;
    /* alocam directorul root */
    TDirectory* root = Alloc_directory("root", NULL);
    /* directorul curent este initial root */
    TDirectory* curr_directory = root;
    /* alocam vector de directoare ca sa salvam ulterior directoarele curente 
    (curr_directories) pentru comanda find */
    TDirectory** array = (TDirectory**)malloc(20 * sizeof(TDirectory*));
    if (!array) {
        return 0;
    }
    int i;
    for (i = 0; i < 20; i++) {
        /* alocam memorie pentru fiecare director */
        array[i] = (TDirectory*)malloc(sizeof(TDirectory));
        if (!array[i]) {
            free(array);
            return 0;
        }
    }
    i = 0;
    /* punem root-ul pe prima pozitie din vector */
    array[i] = root;
    i++;
    while (getline(&line, &len, stdin) != -1) {
        char* token = strtok(line, " \n");
        if (!strcmp(token, "touch")) {
            token = strtok(NULL, " \n"); // numele fisierului
            int res = directory_exists(curr_directory->directories, token);
            /* daca exista un director cu numele fisierului pe care vrem sa-l
            cream afisam mesajul */
            /* daca nu exista, inseram in campul files al directorului curent dupa
            ce am verificat ca nu exista deja un fisier cu numele respectiv
            in functie */
            if (res == 1) {
                printf("Directory %s already exists!\n", token);
            } else {
                insert_file(&(curr_directory)->files, token, curr_directory);
            }
        } else if (!strcmp(token, "mkdir")) {
            token = strtok(NULL, " \n");
            int res = file_exists(curr_directory->files, token);
            /* daca exista un fisier cu numele directorului pe care vrem sa-l
            cream */
            /* daca nu exista, inseram in campul directories al directorului
            curent dupa ce am verificat ca nu exista deja un director cu numele
            respectiv in functie */
            if (res == 1) {
                printf("File %s already exists!\n", token);
            } else {
                insert_directory(&(curr_directory)->directories, token, 
                curr_directory);
            }
        } else if (!strcmp(token, "ls")) {
            /* afisam subdirectoarele si fisierele directorului curent */
            print_directories(curr_directory->directories);
            print_files(curr_directory->files);
            printf("\n");
        } else if (!strcmp(token, "rm")) {
            token = strtok(NULL, " \n");
            /* verificam daca exista fisierul respectiv */
            /* daca exista apelam functia de stergere fisier */
            int res = file_exists(curr_directory->files, token);
            if (res == 0) {
                printf("File %s doesn't exist!\n", token);
            } else {
                curr_directory->files = remove_file(curr_directory->files, token);
            }
        } else if (!strcmp(token, "rmdir")) {
            token = strtok(NULL, " \n");
            /* verificam daca exista directorul respectiv */
            /* daca exista apelam functie de stergere director */
            int res = directory_exists(curr_directory->directories, token);
            if (res == 0) {
                printf("Directory %s doesn't exist!\n", token);
            } else {
                curr_directory->directories = remove_dir(curr_directory->directories,
                token);
            }
        } else if (!strcmp(token, "cd")) {
            token = strtok(NULL, " \n");
            if (!strcmp(token, "..")) {
                if (curr_directory != root) {
                    /* setam directorul curent cu parintele acestuia */
                    curr_directory = curr_directory->parent;
                    /* punem directorul curent in vector */
                    array[i] = curr_directory;
                    i++;
                }
            } else {
                if (curr_directory == root) {
                    /* cautam in directoarele din root */
                    int res = directory_exists(root->directories, token);
                    if (res == 0) {
                        printf("Directory not found!\n");
                    } else {
                        /* setam directorul curent */
                        curr_directory = search_directory(root->directories, token);
                        /* il punem in vectorul de directoare */
                        array[i] = curr_directory;
                        i++;
                    }
                } else {
                    /* daca directorul este diferit de root, cautam mai intai
                    in directoarele parintelui */
                    int ok1 = 1, ok2 = 1;
                    int res1 = directory_exists(curr_directory->parent->directories, token);
                    if (res1 == 0) { //nu am gasit directorul
                        ok1 = 0;
                    }
                    /* cautam in subdirectoarele directorului */
                    int res2 = directory_exists(curr_directory->directories, token);
                    if (res2 == 0) { //nu am gasit directorul
                        ok2 = 0;
                    }
                    if (ok1 == 0 && ok2 == 0) {
                        printf("Directory not found!\n");
                    } else {
                        /* cautam in directoarele din parinte */
                        if (ok1 == 1) {
                            curr_directory = search_directory(curr_directory->parent->directories, token);
                        } 
                        /* cautam in subdirectoarele directorului */
                        if (ok2 == 1) {
                            curr_directory = search_directory(curr_directory->directories, token);
                        }
                        /* punem directorul curent in vector */
                        array[i] = curr_directory;
                        i++;
                    }
                }
            }
        } else if (!strcmp(token, "pwd")) {
            TDirectory* aux = curr_directory;
            f_pwd(aux); //apelam functia pe directorul curent
            printf("\n");
        } else if (!strcmp(token, "find")) {
            token = strtok(NULL, " ");
            if (!strcmp(token, "-f")) {
                token = strtok(NULL, " \n"); //numele fisierului
                int ok = 0, j = 0;
                /* caut in arborele de fisiere al fiecarui element din vectorul
                de curr_directories */
                for (j = 0; j < i; j++) {
                    int res = file_exists(array[j]->files, token);
                    if (res == 1) {
                        ok = 1;
                        printf("File %s found!\n", token);
                        f_pwd(array[j]); //afisam calea fisierului
                        printf("\n");
                        break;
                    }         
                }
                /* daca nu am gasit fisierul, ok ramane 0 */
                if (ok == 0) {
                    printf("File %s not found!\n", token);
                }
            } else if (!strcmp(token, "-d")) {
                token = strtok(NULL, " \n"); //numele directorului
                int ok = 0, j = 0;
                /* caut in arborele de directoare al fiecarui element din
                vectorul de curr_directories */
                for (j = 0; j < i; j++) {
                    int res = directory_exists(array[j]->directories, token);
                    if (res == 1) {
                        ok = 1;
                        printf("Directory %s found!\n", token);
                        /* afisam calea directorului */
                        f_pwd(array[j]);
                        /* afisam si numele directorului */
                        printf("/%s\n", token);
                        break;
                    }
                }
                /* daca nu am gasit directorul, ok ramane 0 */
                if (ok == 0) {
                    printf("Directory %s not found!\n", token);
                }
            }
        } else if (!strcmp(token, "quit")) {
            break;
        }
    }
    return 0;
}