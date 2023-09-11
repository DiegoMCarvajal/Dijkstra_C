int (*dijkstra_with_heap(char *initialNode, int (*adj)[20]))[3] {
  int(*table)[3] = malloc(number_of_nodes * sizeof(*table)); // tabla de distancias
  PQ *pq = pq_create(number_of_nodes, 1);
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
  double priority;
  pq_get_priority(pq, index, &priority);
  while (pq_size(pq) > 0) {
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
          pq_insert(pq, i, table[index][1] + adj[index][i]);
          table[i][1] = table[index][1] + adj[index][i];
          // actualizamos el nodo previo del vecino
          table[i][2] = index;
        }
      }
    }
    pq_delete_top(pq, &index, &priority);
  }

  return table;
}