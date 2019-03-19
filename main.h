#include <stdio.h>

typedef struct neighbour{
	int id;
	struct neighbour *next;
} Neighbour;

typedef struct linkedList{
	Neighbour *list;
	int size;
	Neighbour *tail;
} LinkedList;

typedef struct node {
	int id;
	int low;
	int discovered;
	int articulationPoint;
	int parentId;
	struct neighbour *first;
} Node;

typedef struct graph {
	int numberRouters;
	int numberConnections;
	Node *nodesList;
} Graph;

