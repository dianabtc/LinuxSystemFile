#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct directory {
    char* name;
    struct directory* parent; //pointer catre directorul parinte
    struct file* files; //radacina arborelui de fisiere
    struct directory* directories; //radacina arborelui de subdirectoare
    struct directory *left, *right;
} TDirectory;

typedef struct file {
    char* name;
    struct directory* parent;
    struct file *left, *right;
} TFile;