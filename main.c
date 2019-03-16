#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "list.c"
#include "stack.c"

void readInput(Graph *graph);
void readInput(Graph *graph);
int* findSCC(Graph *graph, int* outputArray);
int TarjanAdapt(Node *node, Node *nodesList);
int min(int n1, int n2);

/* Size of output array in findScc */
int SIZE = 25;

int main(){

	int i = 1;

	Graph *graph = (Graph *)malloc(sizeof(struct graph));

	/* Read the first and the second value from the stdin */
	scanf("%d", &graph->numberRouters);
	scanf("%d", &graph->numberConnections);

	/* Alloc a list representing the graph */
	Node *nodesList = allocList(graph->numberRouters);
	graph->nodesList = nodesList;

	/* Read the input values */
	readInput(graph);

	/* outputArray to store in the first position the number of SCCs and the subgraphs' id (The highest id in each subgraph) */
	int *outputArray = calloc(SIZE, sizeof(int));

	outputArray = findSCC(graph, outputArray);

	printf("%d\n", outputArray[0]);

	while (outputArray[i] != 0)
		printf("%d ", outputArray[i++]);

	printf("\n");

	free(outputArray);

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


int* findSCC(Graph *graph, int *outputArray){
	
	int i = 0, j = 1;
	int numberSCCs = 0;

	Node *nodesList = graph->nodesList;

	for (i = 1; i <= graph->numberRouters; i++){
		if (nodesList[i].discovered == -1){
			outputArray[j++] = TarjanAdapt(&nodesList[i], nodesList);
			numberSCCs++;
			
			if (j == SIZE - 1){
				SIZE *= 2;
				outputArray = realloc(outputArray, SIZE);
			}
		}
	}

	outputArray[0] = numberSCCs;

	return outputArray;
}


int TarjanAdapt(Node *node, Node *nodesList){
	
	static int time = 1;
	static int sccId = 0;

	node->discovered = time++;
	node->low = node->discovered;
	Neighbour *iter = node->first;

	if (sccId < node->id)
		sccId = node->id;

	while(iter != NULL){
		if (nodesList[iter->id].discovered == -1)
			TarjanAdapt(&nodesList[iter->id], nodesList);

			node->low = min(nodesList[iter->id].low, node->low);
		
		iter = iter->next;
	}

	if (node->id == nodesList[node->id].low)
		return sccId;

	return 0;
}


int min(int n1, int n2){
	if (n1 < n2)
		return n1;

	return n2;
}