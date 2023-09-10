#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pq_struct PQ;

typedef struct node node;

struct node {
  int id;
  double priority;
};

struct pq_struct {
  int capacity;
  int size;
  int status;
  int *posArray;
  node *pqArray;
};

PQ *pq_create(int capacity) {
  PQ *pq = (PQ *)malloc(sizeof(PQ));
  pq->capacity = capacity;
  pq->size = 0;
  pq->status = 0;
  pq->posArray = (int *)malloc(capacity * sizeof(int));
  pq->pqArray = (node *)malloc(capacity * sizeof(node));
  return pq;
}

// argc es el número de argumentos que se le pasan al programa incluyendo el nombre del programa
// argv es un array de strings que contiene los argumentos que se le pasan al programa
int main(int argc, char *argv[]) {
  // printf("argc: %d\n", argc);
  // int logitud = sizeof(*argv);
  // printf("logitud: %d\n", logitud);
  FILE *fp;          // Puntero al fichero
  char *line = NULL; // Puntero a la línea
  size_t len = 0;    // Longitud de la línea
  ssize_t read;      // Número de caracteres leídos

  // Comprobamos que se ha pasado un argumento
  if (argc != 2) {
    printf("Uso: %s <fichero>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Abrimos el fichero
  fp = fopen(argv[1], "r"); // Abrimos el fichero en modo lectura
  if (fp == NULL) {
    perror("Error al abrir el fichero");
    exit(EXIT_FAILURE);
  }

  int line_number = 0; // Número de línea

  // Leemos el fichero línea a línea
  while ((read = getline(&line, &len, fp)) != -1 && line_number < 20) {
    // line apunta al primer caracter de la línea
    // len es el número de caracteres de la línea
    // read es el número de caracteres leídos
    if (line_number == 0) {
      char *str = NULL;
      str = line;
      while (*str != '\0') {
        ++str;
        if (*str == '\r' || *str == '\n') {
          *str = '\0';
        }
      }
      int esDirigido = strcmp(line, "true");
      if (esDirigido == 0) {
        printf("Es dirigido\n");
      } else {
        printf("No es dirigido\n");
      }
      ++line_number;
      continue;
    }
    char *token; // Puntero al token
    // char *s = NULL; // Puntero al final de la línea
    int i = 0; // Número de token

    // Leemos los tokens de la línea
    while ((token = strtok_r(line, ";", &line))) {
    }
  }

  // Cerramos el fichero
  fclose(fp);
}