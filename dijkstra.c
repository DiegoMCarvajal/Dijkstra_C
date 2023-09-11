#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 20
#define INT_MAX 2147483647

typedef struct pq_struct PQ;

typedef struct node node;

struct node {
  int id;
  int priority;
};

struct pq_struct {
  int capacity;
  int size;
  int status;
  int *posArray;
  node *pqArray;
};

PQ *pq_create(int capacity, int min_heap) {
  int i;
  PQ *init = malloc(sizeof(PQ));
  init->capacity = capacity;
  init->size = 0;
  init->status = min_heap;
  init->pqArray = malloc(sizeof(node) * (capacity + 1));
  init->posArray = malloc(sizeof(int) * capacity);
  for (i = 0; i < capacity; i++)
    init->posArray[i] = -1;
  return init;
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

int pq_insert(PQ *pq, int id, int priority) {
  if (((pq->size + 1) > pq->capacity) || id > pq->capacity || id < 0 || pq->posArray[id] != -1) // If the PQ is full or the id is out of range or the id is already in the PQ
    return 0;
  pq->size++;                                // Increment the size of the PQ by 1
  pq->pqArray[pq->size].id = id;             // Set the id of the node in the last position of the PQ
  pq->pqArray[pq->size].priority = priority; // Set the priority of the node in the last position of the PQ
  pq->posArray[id] = pq->size;               // Set the position of the node in the posArray
  return 1;                                  // Return 1 if the node has been inserted
}

PQ *percolate_up(PQ *pq, int root) {
  int parent = root / 2;
  if (parent < 1)
    return pq;

  int idRoot = pq->pqArray[root].id;
  int idParent = pq->pqArray[parent].id;

  if (pq->status != 0) { // if it is a min heap

    if (pq->pqArray[root].priority <= pq->pqArray[parent].priority) {
      node temp;
      temp = pq->pqArray[parent];

      pq->pqArray[parent] = pq->pqArray[root]; // swap in heap array
      pq->pqArray[root] = temp;

      pq->posArray[idRoot] = parent; // swap in pos array
      pq->posArray[idParent] = root;

      return percolate_up(pq, parent);
    } else
      return pq;
  } else { // else it is a max heap

    if (pq->pqArray[root].priority >= pq->pqArray[parent].priority) {
      node temp;
      temp = pq->pqArray[parent];

      pq->pqArray[parent] = pq->pqArray[root]; // swap in heap array
      pq->pqArray[root] = temp;

      pq->posArray[idRoot] = parent; // swap in pos array
      pq->posArray[idParent] = root;

      return percolate_up(pq, parent);
    } else
      return pq;
  }
}

PQ *percolate_down(PQ *pq, int root) {
  if ((root * 2) > pq->size) // if the root has no children
    return pq;

  int chosen;
  int idRoot = pq->pqArray[root].id;
  int idChosen;

  if (pq->status != 0) { // if it is a min heap
    if (pq->pqArray[root * 2].priority < pq->pqArray[(root * 2) + 1].priority)
      chosen = root * 2;
    else
      chosen = (root * 2) + 1;

    idChosen = pq->pqArray[chosen].id;

    if (pq->pqArray[root].priority >= pq->pqArray[chosen].priority) {
      node temp;
      temp = pq->pqArray[chosen];

      pq->pqArray[chosen] = pq->pqArray[root]; // swap in heap array
      pq->pqArray[root] = temp;

      pq->posArray[idChosen] = root; // swap in pos array
      pq->posArray[idRoot] = chosen;

      return percolate_down(pq, chosen);
    } else
      return pq;
  } else { // else it is a max heap

    if (pq->pqArray[root * 2].priority > pq->pqArray[(root * 2) + 1].priority)
      chosen = root * 2;
    else
      chosen = (root * 2) + 1;

    idChosen = pq->pqArray[chosen].id;

    if (pq->pqArray[root].priority <= pq->pqArray[chosen].priority) {
      node temp;
      temp = pq->pqArray[chosen];

      pq->pqArray[chosen] = pq->pqArray[root]; // swap
      pq->pqArray[root] = temp;

      pq->posArray[idChosen] = root; // swap in pos array
      pq->posArray[idRoot] = chosen;

      return percolate_down(pq, chosen);
    } else
      return pq;
  }
}

int pq_change_priority(PQ *pq, int id, int new_priority) {
  if (id > pq->capacity || id < 0 || pq->posArray[id] == -1)
    return 0;

  int posID = pq->posArray[id];
  int old_priority = pq->pqArray[posID].priority;

  if (new_priority == old_priority)
    return 0;

  pq->pqArray[posID].priority = new_priority; // update priority

  if (pq->status != 0) { // it is a min heap

    if (new_priority > old_priority)
      pq = percolate_down(pq, posID);
    else
      pq = percolate_up(pq, posID);
  }

  else { // it is a max heap

    if (new_priority > old_priority)
      pq = percolate_up(pq, posID);
    else
      pq = percolate_down(pq, posID);
  }

  return 1;
}

int pq_remove_by_id(PQ *pq, int id) {
  if (((pq->size) < 1) || id > pq->capacity || id < 0 || pq->posArray[id] == -1)
    return 0;

  int posID = pq->posArray[id];

  int idLastElem = pq->pqArray[pq->size].id;

  pq->pqArray[posID] = pq->pqArray[pq->size]; // swap in the node array

  pq->posArray[idLastElem] = posID; // update the pos array

  pq->posArray[id] = -1; // reset the id in the pos array

  pq->size--; // reduce the size since you're removing an element

  pq = percolate_down(pq, posID);

  return 1;
}

int pq_get_priority(PQ *pq, int id, int *priority) {
  if (id > pq->capacity || id < 0 || pq->posArray[id] == -1)
    return 0;

  int pos = pq->posArray[id];
  *priority = pq->pqArray[pos].priority;

  return 1;
}

int pq_delete_top(PQ *pq, int *id, int *priority) {
  if (pq->size < 1)
    return 0;

  *id = pq->pqArray[1].id;

  *priority = pq->pqArray[1].priority;

  int lastElemId = pq->pqArray[pq->size].id;

  pq->posArray[*id] = -1; // reset the top elem

  pq->pqArray[1] = pq->pqArray[pq->size]; // swap in the node array

  pq->posArray[lastElemId] = 1;

  pq->size--; // decrement the size

  pq = percolate_down(pq, 1);

  return 1;
}

int pq_capacity(PQ *pq) {
  return pq->capacity;
}

int pq_size(PQ *pq) {
  return pq->size;
}

int pq_contains(PQ *pq, int id) {
  if (pq->posArray[id] == -1)
    return 0;
  else
    return 1;
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

void print_table(int (*table)[3]) {
  for (int i = 0; i < number_of_nodes; i++) {
    for (int j = 0; j < 3; j++) {
      if (j == 1) {
        printf("%d ", table[i][j]);
      } else {
        printf("%s ", nodes[table[i][j]]);
      }
    }
    printf("\n");
  }
}

int (*dijkstra(char *initialNode, int (*adj)[20]))[3] {
  int(*table)[3] = malloc(number_of_nodes * sizeof(*table)); // tabla de distancias
  int visited[number_of_nodes];                              // nodos visitados
  int unvisited_neighbours[number_of_nodes];                 // nodos no visitados vecinos de un nodo visitado

  // buscamos el nodo inicial
  int index = get_index_of_node(initialNode);
  if (index == -1) {
    printf("El nodo inicial no existe\n");
    return table;
  }

  // inicializamos la tabla
  for (int i = 0; i < number_of_nodes; i++) {
    table[i][0] = i;       // nodo
    table[i][1] = INT_MAX; // distancia
    table[i][2] = -1;      // nodo previo
    visited[i] = 0;
  }

  // inicializamos los nodos no visitados
  for (int i = 0; i < number_of_nodes; i++) {
    unvisited_neighbours[i] = 0;
  }
  // mientras haya nodos no visitados
  int unvisited_nodes = 1; // el nodo inicial
  table[index][1] = 0;     // la distancia del nodo inicial es 0
  while (unvisited_nodes > 0) {
    visited[index] = 1;
    unvisited_neighbours[index] = 0;
    --unvisited_nodes;
    // para cada vecino del nodo
    for (int i = 0; i < number_of_nodes; i++) {
      // si el vecino no ha sido visitado
      if (adj[index][i] != 0 && visited[i] == 0) {
        // si la distancia del nodo actual más la distancia al vecino es menor a la distancia del vecino
        if (table[index][1] + adj[index][i] < table[i][1]) {
          // actualizamos la distancia del vecino
          table[i][1] = table[index][1] + adj[index][i];
          // actualizamos el nodo previo del vecino
          table[i][2] = index;
        }
        // agregamos el vecino a los nodos no visitados
        unvisited_neighbours[i] = 1;
        ++unvisited_nodes;
      }
    }
    // buscamos el nodo no visitado con menor distancia
    int min = INT_MAX;
    int min_index = -1;
    for (int i = 0; i < number_of_nodes; i++) {
      if (unvisited_neighbours[i] == 1 && table[index][1] < min) {
        min = table[index][i];
        min_index = i;
      }
    }
    // si no hay nodos no visitados con distancia menor a infinito
    if (min_index == -1) {
      break;
    }
    index = min_index;
  }

  return table;
}

// dijkstra con heap

int (*dijkstra_with_heap(char *initialNode, int (*adj)[20]))[3] {
  int(*table)[3] = malloc(number_of_nodes * sizeof(*table)); // tabla de distancias
  PQ *pq = pq_create(MAX_NODES, 1);
  int visited[number_of_nodes]; // nodos visitados

  // buscamos el nodo inicial
  int index = get_index_of_node(initialNode);
  if (index == -1) {
    printf("El nodo inicial no existe\n");
    return table;
  }

  // inicializamos la tabla
  for (int i = 0; i < number_of_nodes; i++) {
    table[i][0] = i;       // nodo
    table[i][1] = INT_MAX; // distancia
    table[i][2] = -1;      // nodo previo
    visited[i] = 0;
  }

  // agregamos al nodo inicial a la cola de prioridad
  pq_insert(pq, index, 0);
  table[index][1] = 0;
  int priority;
  while (pq_size(pq) > 0) {
    pq_delete_top(pq, &index, &priority);
    visited[index] = 1;
    // para cada vecino del nodo
    for (int i = 0; i < number_of_nodes; i++) {
      // si el vecino no ha sido visitado
      if (adj[index][i] != 0 && visited[i] == 0) {
        if (pq_contains(pq, i) == 1) {
          if (table[index][1] + adj[index][i] < table[i][1]) {
            // actualizamos la distancia del vecino
            pq_change_priority(pq, i, table[index][1] + adj[index][i]);
            table[i][1] = table[index][1] + adj[index][i];
            // actualizamos el nodo previo del vecino
            table[i][2] = index;
          }
        } else {
          priority = table[index][1] + adj[index][i];
          pq_insert(pq, i, priority);
          table[i][1] = priority;
          // actualizamos el nodo previo del vecino
          table[i][2] = index;
        }
      }
    }
  }
  pq_destroy(pq);
  return table;
}

int main(int argc, char *argv[]) {
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
  printf("\n");
  printf("Djikstra sin heap\n");
  int(*table)[3] = dijkstra("A", graph);
  print_table(table);
  free(table);
  printf("\n");
  printf("Djikstra con heap\n");
  int(*table2)[3] = dijkstra_with_heap("A", graph);
  print_table(table2);
  free(table2);

  return 0;
}