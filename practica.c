#include "pq.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODOS 20
#define MAX_ARISTAS 50
#define INF 9999

char *nodos[MAX_NODOS];
bool nodos_visitados[MAX_NODOS];
int numero_de_nodos = 0;
int adj[MAX_NODOS][MAX_NODOS];

typedef struct
{
  char *nodo_destino[MAX_NODOS];
  char *nodo_previo[MAX_NODOS];
  double distancia[MAX_NODOS];
} DatosTabla;

void verificarUso(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Uso: %s <nombre_archivo>\n", argv[0]);
    exit(1);
  }
}

void esDirigido(char **linea, int *es_dirigido) {
  char *str = *linea;
  while (*str != '\n') {
    ++str;
  }
  *str = '\0';
  if (0 == (strncmp("true", *linea, strlen("true")))) {
    printf("El grafo es dirigido.\n");
    *es_dirigido = 1;
  } else {
    printf("El grafo no es dirigido.\n");
  }
  return;
}

void comprobarSiGrafoEsValido(char *grafo) {
  if (strchr(grafo, '\n') != NULL) {
    printf("El grafo no válido, debe estar en una sola linea.\n");
    exit(1);
  }
}

void eliminarEspacioInicial(char **str) {
  while (isspace((unsigned char)**str)) {
    (*str)++;
  }
}

void eliminarEspacioFinal(char **str) {
  char *end;
  end = *str + strlen(*str) - 1;
  while (end > (*str) && isspace((unsigned char)*end))
    end--;
  end[1] = '\0';
}

char *eliminarEspaciosEnBlanco(char *str) {
  eliminarEspacioInicial(&str);
  if (*str == 0)
    return str;
  eliminarEspacioFinal(&str);
  return str;
}

int obtenerIndexDelNodo(char *nodo) {
  int index = -1;
  for (int i = 0; i < numero_de_nodos; i++) {
    if (0 == (strcmp(nodos[i], nodo))) {
      index = i;
      break;
    }
  }
  return index;
}

void verificarCantidadNodos() {
  if (numero_de_nodos > MAX_NODOS) {
    printf("El grafo tiene demasiados nodos.\n");
    exit(1);
  }
}

void procesarNodoOrigen(char *token_inner, int *source) {
  token_inner = eliminarEspaciosEnBlanco(token_inner);
  token_inner += 1;
  *source = obtenerIndexDelNodo(token_inner);
  if (*source == -1) {
    *source = numero_de_nodos;
    nodos[*source] = malloc(sizeof(char) * (strlen(token_inner) + 1));
    strcpy(nodos[*source], token_inner);
    numero_de_nodos++;
    verificarCantidadNodos();
  }
}

void procesarPeso(char *token_inner, int *weight) {
  token_inner = eliminarEspaciosEnBlanco(token_inner);
  char *str = token_inner;
  while (*str != ')') {
    ++str;
  }
  *str = '\0';
  *weight = atoi(token_inner);
}

void procesarNodoDestino(char *token_inner, int *destination) {
  token_inner = eliminarEspaciosEnBlanco(token_inner);
  *destination = obtenerIndexDelNodo(token_inner);

  if (*destination == -1) {
    *destination = numero_de_nodos;
    nodos[*destination] = malloc(sizeof(char) * (strlen(token_inner) + 1));
    strcpy(nodos[*destination], token_inner);
    numero_de_nodos++;
    verificarCantidadNodos();
  }
}

void agregarValoresMatriz(int source, int destination, int weight, int es_dirigido) {
  adj[source][destination] = weight;
  if (es_dirigido == 0) {
    adj[destination][source] = weight;
  }
}

void procesarToken(char *token, int es_dirigido) {
  int token_index = 0;
  int source = -1;
  int destination = -1;
  int weight = -1;
  char *end_token;
  char *token_inner = strtok_r(token, ",", &end_token);
  while (token_inner != NULL && token_index < 3) {
    switch (token_index) {
    case 0:
      procesarNodoOrigen(token_inner, &source);
      break;
    case 1:
      procesarNodoDestino(token_inner, &destination);
      break;
    case 2:
      procesarPeso(token_inner, &weight);
      break;
    default:
      printf("Error de sintaxis. No se pueden tener 4 valores\n");
      exit(1);
    }
    token_index++;
    token_inner = strtok_r(NULL, ",", &end_token);
  }
  agregarValoresMatriz(source, destination, weight, es_dirigido);
}

void crearMatriz(char *line, int es_dirigido) {
  char *end_str;
  char *token = strtok_r(line, ";", &end_str);
  int numero_de_tokens = 0;
  while (token != NULL) {
    procesarToken(token, es_dirigido);
    token = strtok_r(NULL, ";", &end_str);
    numero_de_tokens++;
    if (numero_de_tokens > MAX_ARISTAS) {
      printf("El grafo tiene demasiadas aristas.\n");
      exit(1);
    }
  }
}

void print_adj() {
  for (int i = 0; i < numero_de_nodos; i++) {
    for (int j = 0; j < numero_de_nodos; j++) {
      printf("%d ", adj[i][j]);
    }
    printf("\n");
  }
}

void procesarArchivo(FILE *archivo) {
  size_t len = 0;
  ssize_t read;
  char *linea = NULL;
  int read_line = 0;
  int es_dirigido = 0;

  while ((read = getline(&linea, &len, archivo)) != -1) {
    if (read_line == 0) {
      esDirigido(&linea, &es_dirigido);
    } else {
      crearMatriz(linea, es_dirigido);
    }
    read_line++;
  }
  print_adj();
  free(linea);
}

void *abrirArchivoYCrearMatriz(char *nombreArchivo) {
  FILE *archivo = fopen(nombreArchivo, "r");
  if (archivo == NULL) {
    printf("No se pudo abrir el archivo.\n");
    exit(1);
  }
  procesarArchivo(archivo);
  fclose(archivo);
}

void mostrarNombreNodos() {
  printf("Nodos: ");
  for (int i = 0; i < numero_de_nodos; i++) {
    printf("%s ", nodos[i]);
  }
  printf("\n");
}

int solicitarNodoInicial() {
  char nodo_inicial[255];
  printf("Ingrese el nodo inicial: ");
  scanf("%s", nodo_inicial);
  int index_nodo_inicial = obtenerIndexDelNodo(nodo_inicial);
  if (index_nodo_inicial == -1) {
    printf("El nodo inicial no existe.\n");
    return solicitarNodoInicial();
  }
  printf("Nodo inicial: %s\n", nodo_inicial);
  return index_nodo_inicial;
}

void inicializarDatosTabla(int inicio, DatosTabla *tabla) {
  for (int i = 0; i < numero_de_nodos; i++) {
    tabla->nodo_destino[i] = nodos[i];
    tabla->nodo_previo[i] = NULL;
    tabla->distancia[i] = INF;
    nodos_visitados[i] = false;
  }
  tabla->distancia[inicio] = 0;
  tabla->nodo_previo[inicio] = "";
  nodos_visitados[inicio] = true;
}

int faltanNodosPorVisitar() {
  for (int i = 0; i < numero_de_nodos; i++) {
    if (!nodos_visitados[i]) {
      return 1;
    }
  }
  return 0;
}

int obtenerNodoNoVisitado() {
  for (int i = 0; i < numero_de_nodos; i++) {
    if (!nodos_visitados[i]) {
      return i;
    }
  }
  return -1;
}

void actualizarDatosTabla(int nodo_actual, DatosTabla *datos_tabla, int arista) {
  int distancia_a_nodo = adj[nodo_actual][arista];
  int distancia_siguiente = datos_tabla->distancia[nodo_actual] + distancia_a_nodo;
  if (distancia_a_nodo > 0 &&
      distancia_siguiente < datos_tabla->distancia[arista]) {
    datos_tabla->distancia[arista] = distancia_siguiente;
    datos_tabla->nodo_previo[arista] = nodos[nodo_actual];
  }
}

void actualizarMinDistanciaYNodo(DatosTabla *datos_tabla, double *min_distancia, char **nodo_siguiente, int *index_nodo_siguiente, int arista) {
  if (datos_tabla->distancia[arista] < *min_distancia) {
    *min_distancia = datos_tabla->distancia[arista];
    *nodo_siguiente = nodos[arista];
    *index_nodo_siguiente = arista;
  }
}

void imprimirTabla(DatosTabla *tabla) {
  printf("Nodo\tDistancia\tNodo anterior\n");
  for (int i = 0; i < numero_de_nodos; i++) {
    printf("%s\t%f\t%s\n", tabla->nodo_destino[i], tabla->distancia[i], tabla->nodo_previo[i]);
  }
}

void dijkstra(int inicio) {
  DatosTabla datos_tabla;
  inicializarDatosTabla(inicio, &datos_tabla);
  int nodo_actual = inicio;
  while (faltanNodosPorVisitar()) {
    char *nodo_siguiente = NULL;
    int index_nodo_siguiente = -1;
    double min_distancia = INF;
    for (int arista = 0; arista < numero_de_nodos; arista++) {
      if (!nodos_visitados[arista]) {
        actualizarDatosTabla(nodo_actual, &datos_tabla, arista);
        actualizarMinDistanciaYNodo(&datos_tabla, &min_distancia, &nodo_siguiente, &index_nodo_siguiente, arista);
      }
    }
    if (nodo_siguiente == NULL) {
      break;
    }
    nodos_visitados[index_nodo_siguiente] = true;
    nodo_actual = index_nodo_siguiente;
  }
  imprimirTabla(&datos_tabla);
}

void reiniciarNodosVisitados() {
  for (int i = 0; i < numero_de_nodos; i++) {
    nodos_visitados[i] = false;
  }
}

void dijkstraConHeap(int inicio) {
  reiniciarNodosVisitados();
  DatosTabla datos_tabla_heap;
  inicializarDatosTabla(inicio, &datos_tabla_heap);

  PQ *cola_prioridades = pq_create(numero_de_nodos, 1);
  pq_insert(cola_prioridades, inicio, 0);

  while (pq_size(cola_prioridades) > 0) {
    int nodo_actual;
    double distancia_actual;
    pq_delete_top(cola_prioridades, &nodo_actual, &distancia_actual);
    nodos_visitados[nodo_actual] = true;
    for (int arista = 0; arista < numero_de_nodos; arista++) {
      if (!nodos_visitados[arista] && adj[nodo_actual][arista] > 0) {
        double nueva_distancia = distancia_actual + adj[nodo_actual][arista];
        if (nueva_distancia < datos_tabla_heap.distancia[arista]) {
          datos_tabla_heap.distancia[arista] = nueva_distancia;
          datos_tabla_heap.nodo_previo[arista] = nodos[nodo_actual];

          pq_insert(cola_prioridades, arista, nueva_distancia);
        }
      }
    }
  }
  imprimirTabla(&datos_tabla_heap);
  pq_free(cola_prioridades);
}

int main(int argc, char *argv[]) {
  verificarUso(argc, argv);
  abrirArchivoYCrearMatriz(argv[1]);
  mostrarNombreNodos();
  int nodo_inicial = solicitarNodoInicial();
  printf("Dijkstra sin heap\n");
  dijkstra(nodo_inicial);
  printf("Dijkstra con min-heap\n");
  dijkstraConHeap(nodo_inicial);
  return 0;
}
