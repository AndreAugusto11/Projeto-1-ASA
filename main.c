#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "list.c"
#include "stack.c"

void readInput(Graph *graph);
int findSCC(Graph *graph);
void readInput(Graph *graph);
int findSCC(Graph *graph);
void TarjanAdapt(Node *node, Node *nodesList, Stack *s);
int min(int n1, int n2);


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
	printf("Numero: %d\n", numberSCCs);

	printf("Printing:\n");
	printList(graph);



	freeList(graph);
	free(graph);

	return 0;
}


void readInput(Graph *graph){

	int i = 0;
	int node1, node2;

	/* Reading the connections from the input */
	while(i++ < graph->numberConnections){
		scanf("%d %d", &node1, &node2);
		addNode(node1, node2, graph->nodesList);
		addNode(node2, node1, graph->nodesList);
	}
}


int findSCC(Graph *graph){
	
	int i = 0;
	int numberSCCs = 0;

	Node *nodesList = graph->nodesList;

	Stack *s = allocStack(graph->numberRouters);

	for (i = 1; i <= graph->numberRouters; i++){
		if (nodesList[i].discovered == -1){
			TarjanAdapt(&nodesList[i], nodesList, s);
				numberSCCs++;
		}
	}

	freeStack(s);
	return numberSCCs;
}


void TarjanAdapt(Node *node, Node *nodesList, Stack *s){
	
	static int time = 1;

	node->discovered = time++;
	node->low = node->discovered;
	node->onStack = true;
	Neighbour *iter = node->first;

	push(node->id, s);

	while(iter != NULL){
		if (nodesList[iter->id].discovered == -1)
			TarjanAdapt(&nodesList[iter->id], nodesList, s);

		else if (nodesList[iter->id].onStack == true)
			node->low = min(nodesList[iter->id].low, node->low);
		
		iter = iter->next;
	}

	if (node->discovered == node->low){
		Node *nodeAux = &nodesList[pop(s)];
		
		while(nodeAux->id != node->id){
			nodeAux->onStack = false;
			nodeAux = &nodesList[pop(s)];
		}
	}
}


int min(int n1, int n2){
	if (n1 < n2)
		return n1;

	return n2;
}