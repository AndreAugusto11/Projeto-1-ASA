#include <stdio.h>

typedef enum {true, false} bool;

typedef struct neighbour{
	int id;
	struct neighbour *next;
} Neighbour;

typedef struct node {
	int id;
	int low;
	int discovered;
	bool onStack;
	struct neighbour *first;
} Node;

typedef struct graph {
	int numberRouters;
	int numberConnections;
	Node *nodesList;
} Graph;