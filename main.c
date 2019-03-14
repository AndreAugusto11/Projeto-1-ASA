#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "list.c"
#include "stack.c"


int numberRouters;
int numberConnections;


void readInput(Graph *graph);
int findSCC(Graph *graph);


int main(){

	Graph *graph = (Graph *)malloc(sizeof(struct graph));

	/* Read the first and the second value from the stdin */
	scanf("%d", &graph->numberRouters);
	scanf("%d", &graph->numberConnections);

	/* Alloc a list representing the graph */
	Node *nodesList = allocList(graph->numberRouters);
	graph->nodesList = nodesList;

	/* Read the input values */
	readInput(graph);

	int numberSCCs = findSCC(graph);

	printf("\nPrinting:\n");
	printList(graph);

	freeList(graph);
	free(graph);
	return 0;
}


void readInput(Graph *graph){

	int i = 0;
	int node1, node2;

	while(i++ < graph->numberConnections){
		scanf("%d %d", &node1, &node2);		
		addNode(node1, node2, graph->nodesList);
	}
}


int findSCC(Graph *graph){
	int visited = 0;
	int numberSCCs = 0;

	Stack *s = allocStack(graph->numberRouters);




	freeStack(s);
	return 0;
}