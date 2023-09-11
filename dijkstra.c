#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 20

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

int number_of_nodes = 0;
int graph[MAX_NODES][MAX_NODES];
char *nodes[MAX_NODES];
int is_directed = 0;

char *trimwhitespace(char *str) {
  char *end;
  // Trim leading space
  while (isspace((unsigned char)*str))
    str++;
  if (*str == 0) // All spaces?
    return str;
  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;
  // Write new null terminator character
  end[1] = '\0';
  return str;
}

// Returns -1 if node is not present
int get_index_of_node(char *node_to_check) {
  int value = -1;
  for (int i = 0; i < number_of_nodes; i++) {
    if (0 == (strcmp(nodes[i], node_to_check))) {
      value = i;
      break;
    }
  }
  // printf("Index: %d\n", value);
  return value;
}

void print_graph() {
  for (int i = 0; i < number_of_nodes; i++) {
    for (int j = 0; j < number_of_nodes; j++) {
      printf("%d ", graph[i][j]);
    }
    printf("\n");
  }
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
      if (strcmp(line, "true") == 0) {
        is_directed = 1;
        printf("Es dirigido\n");
      } else {
        printf("No es dirigido\n");
      }
      ++line_number;
      continue;
    }
    char *token = strtok_r(line, ";", &line); // Puntero al token
    // Leemos los tokens de la línea
    while (token != NULL) {
      int i = 0; // auxiliar
      int index = -1;
      int destination = -1;
      int weight = -1;
      char *end_token;
      char *token_inner = strtok_r(token, ",", &end_token);
      while (token_inner != NULL && i < 3) {
        if (i == 0) {
          token_inner = trimwhitespace(token_inner);
          token_inner += 1;
          index = get_index_of_node(token_inner);
          if (index == -1) {
            index = number_of_nodes;
            nodes[index] = malloc(sizeof(char) * (strlen(token_inner) + 1));
            strcpy(nodes[index], token_inner);
            ++number_of_nodes;
          }
        } else if (i == 1) {
          token_inner = trimwhitespace(token_inner);
          destination = get_index_of_node(token_inner);
          if (destination == -1) {
            destination = number_of_nodes;
            nodes[destination] = malloc(sizeof(char) * (strlen(token_inner) + 1));
            strcpy(nodes[destination], token_inner);
            ++number_of_nodes;
          }
        } else if (i == 2) {
          token_inner = trimwhitespace(token_inner);
          char *s = token_inner;
          // This while is to erase the the character ) and replace it by \n in order of no affect the atoi cast!
          while (*s != ')') {
            ++s;
          }
          *s = '\0';
          weight = atoi(token_inner);
        } else {
          printf("Parsing error. Not possible four values for edege\n");
        }
        i++;
        token_inner = strtok_r(NULL, ",", &end_token);
      }
      graph[index][destination] = weight;
      if (is_directed == 0) {
        graph[destination][index] = weight;
      }
      token = strtok_r(NULL, ";", &line);
    }
  }

  print_graph();

  // Cerramos el fichero
  fclose(fp);
}