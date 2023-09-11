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
  PQ *pq = (PQ *)malloc(sizeof(PQ)); // Allocate memory for the PQ struct
  pq->capacity = capacity;
  pq->size = 0;
  pq->status = 0;
  pq->posArray = (int *)malloc(capacity * sizeof(int));  // Allocate memory for the posArray (array of positions)
  pq->pqArray = (node *)malloc(capacity * sizeof(node)); // Allocate memory for the pqArray (array of nodes)
  return pq;
}

void pq_destroy(PQ *pq) {
  free(pq->posArray);
  free(pq->pqArray);
  free(pq);
}

/* int pq_is_empty(PQ *pq) {
  if (pq->size == 0) {
    return 1;
  } else {
    return 0;
  }
}

int pq_is_full(PQ *pq) {
  if (pq->size == pq->capacity) {
    return 1;
  } else {
    return 0;
  }
} */

int pq_insert(PQ *pq, int id, double priority) {
  if (pq->size == pq->capacity) { // If the PQ is full
    return 0;                     // Return 0
  }
  pq->size++;                                // Increment the size of the PQ by 1
  pq->pqArray[pq->size].id = id;             // Set the id of the node in the last position of the PQ
  pq->pqArray[pq->size].priority = priority; // Set the priority of the node in the last position of the PQ
  pq->posArray[id] = pq->size;               // Set the position of the node in the posArray
  return 1;                                  // Return 1 if the node has been inserted
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
      int i = 0;                                            // auxiliar
      int index = -1;                                       // index of the node
      int destination = -1;                                 // index of the destination node
      int weight = -1;                                      // weight of the edge
      char *end_token;                                      // end of the token
      char *token_inner = strtok_r(token, ",", &end_token); // inner token
      while (token_inner != NULL && i < 3) {
        if (i == 0) {                                                        // if it is the first token
          token_inner = trimwhitespace(token_inner);                         // trim the white spaces
          token_inner += 1;                                                  // remove the first character (
          index = get_index_of_node(token_inner);                            // get the index of the node
          if (index == -1) {                                                 // if the node is not present in the array
            index = number_of_nodes;                                         // set the index to the number of nodes
            nodes[index] = malloc(sizeof(char) * (strlen(token_inner) + 1)); // allocate memory for the node
            strcpy(nodes[index], token_inner);                               // copy the node to the array
            ++number_of_nodes;                                               // increment the number of nodes
          }
        } else if (i == 1) {                                                       // if it is the second token
          token_inner = trimwhitespace(token_inner);                               // trim the white spaces
          destination = get_index_of_node(token_inner);                            // get the index of the node
          if (destination == -1) {                                                 // if the node is not present in the array
            destination = number_of_nodes;                                         // set the index to the number of nodes
            nodes[destination] = malloc(sizeof(char) * (strlen(token_inner) + 1)); // allocate memory for the node
            strcpy(nodes[destination], token_inner);                               // copy the node to the array
            ++number_of_nodes;                                                     // increment the number of nodes
          }
        } else if (i == 2) {                         // if it is the third token
          token_inner = trimwhitespace(token_inner); // trim the white spaces
          char *s = token_inner;                     // auxiliar
          // This while is to erase the the character ) and replace it by \n in order of no affect the atoi cast!
          while (*s != ')') {
            ++s;
          }
          *s = '\0';                  // replace the ) by \0
          weight = atoi(token_inner); // convert the string to int
        } else {
          printf("Parsing error. Not possible four values for edege\n");
        }
        i++;                                           // increment the auxiliar
        token_inner = strtok_r(NULL, ",", &end_token); // get the next token
      }
      graph[index][destination] = weight;   // set the weight of the edge
      if (is_directed == 0) {               // if the graph is not directed
        graph[destination][index] = weight; // set the weight of the edge in the other direction
      }
      token = strtok_r(NULL, ";", &line); // get the next token
    }
  }
  // Cerramos el fichero
  fclose(fp);
  print_graph();
}