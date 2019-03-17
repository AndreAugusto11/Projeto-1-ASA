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
int countArticulationPoints(Graph *graph);
int iterateOverGraph(Node *node, Node *nodesList, int *visited);


/* Size of output array in findScc */
int SIZE = 25;
int rootTreeEdges = 0;

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

	printf("%d\n", countArticulationPoints(graph));

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

	if (node->discovered == nodesList[node->id].low)
		return sccId;


	return 0;
}

int countArticulationPoints(Graph *graph){

	Node *nodesList = graph->nodesList;
	int i = 0, totalArtPoints = 0, aux = 0;

	int *visited = calloc((graph->numberRouters + 1), sizeof(int));

	for (i = 1; i <= graph->numberRouters; i++){
		if (visited[nodesList[i].id] == 0){
			totalArtPoints = iterateOverGraph(&nodesList[i], nodesList, visited);
			rootTreeEdges = 0;
		}
	}

	free(visited);
	return totalArtPoints;
}

int iterateOverGraph(Node *node, Node *nodesList, int *visited){

	static int numArticulationPoints = 0;			/* If the root has at least two tree edges then it is an articulation point */
	int i = 0;

	visited[node->id] = 1;
	Neighbour *iter = node->first;

	while(iter != NULL){
		if (visited[iter->id] == 0){
			if ((node->discovered == node->low) && (rootTreeEdges != 2)){
				rootTreeEdges++;
			}

			iterateOverGraph(&nodesList[iter->id], nodesList, visited);
		}

		if (nodesList[iter->id].low >= node->discovered){
			if (node->discovered != node->low){
				if (node->articulationPoint == 0){
				/*	printf("Parent: %d\n", node->id);
					printf("Child: %d\n", nodesList[iter->id].id);*/
					node->articulationPoint = 1;
					numArticulationPoints++;
				}
			}

			else {
				if (rootTreeEdges == 2){
					rootTreeEdges++;			/* Guarantees numArticulationPoints is only increased */
					numArticulationPoints++;
				}
			}
		}

		iter = iter->next;
	}

	return numArticulationPoints;
}

int min(int n1, int n2){
	if (n1 < n2)
		return n1;

	return n2;
}