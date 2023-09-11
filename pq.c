#include "pq.h"
#include <stdio.h>
#include <stdlib.h>

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

void pq_free(PQ *pq) {
  free(pq->posArray);
  free(pq->pqArray);
  free(pq);
}

int pq_insert(PQ *pq, int id, double priority) {
  if ((((pq->size) + 1) > pq->capacity) || id > pq->capacity || id < 0 || pq->posArray[id] != -1)
    return 0;

  pq->size++;

  pq->pqArray[pq->size].id = id;
  pq->pqArray[pq->size].priority = priority;

  pq->posArray[id] = pq->size;

  pq = percolate_up(pq, pq->size);

  return 1;
}

PQ *percolate_up(PQ *pq, int root) {
  if (root / 2 < 1)
    return pq;

  int idRoot = pq->pqArray[root].id;
  int idParent = pq->pqArray[root / 2].id;

  if (pq->status != 0) { // if it is a min heap

    if (pq->pqArray[root].priority <= pq->pqArray[root / 2].priority) {
      node temp;
      temp = pq->pqArray[root / 2];

      pq->pqArray[root / 2] = pq->pqArray[root]; // swap in heap array
      pq->pqArray[root] = temp;

      pq->posArray[idRoot] = root / 2; // swap in pos array
      pq->posArray[idParent] = root;

      return percolate_up(pq, root / 2);
    } else
      return pq;
  } else { // else it is a max heap

    if (pq->pqArray[root].priority >= pq->pqArray[root / 2].priority) {
      node temp;
      temp = pq->pqArray[root / 2];

      pq->pqArray[root / 2] = pq->pqArray[root]; // swap in heap array
      pq->pqArray[root] = temp;

      pq->posArray[idRoot] = root / 2; // swap in pos array
      pq->posArray[idParent] = root;

      return percolate_up(pq, root / 2);
    } else
      return pq;
  }
}

PQ *percolate_down(PQ *pq, int root) {
  if ((root * 2) > pq->size)
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

int pq_change_priority(PQ *pq, int id, double new_priority) {
  if (id > pq->capacity || id < 0 || pq->posArray[id] == -1)
    return 0;

  int posID = pq->posArray[id];
  double old_priority = pq->pqArray[posID].priority;

  if (new_priority == old_priority)
    return 0;

  pq->pqArray[posID].priority = new_priority; // change the priority

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
  if ((((pq->size)) < 1) || id > pq->capacity || id < 0 || pq->posArray[id] == -1)
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

int pq_get_priority(PQ *pq, int id, double *priority) {
  if (id > pq->capacity || id < 0 || pq->posArray[id] == -1)
    return 0;

  int pos = pq->posArray[id];
  *priority = pq->pqArray[pos].priority;

  return 1;
}

int pq_delete_top(PQ *pq, int *id, double *priority) {
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
