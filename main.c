#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "list.c"

void readInput(Graph *graph);
void readInput(Graph *graph);
int* findSCC(Graph *graph, int* outputArray);
int TarjanAdapt(Node *parent, Node *node, Node *nodesList);
int min(int n1, int n2);


int SIZE = 25;								/* Size of output array in findScc */
int count = 0;
int articulationPointId = 0;
int sccId = 0;								/* Variable to help finding the subgraphsId's */
static int numArticulationPoints = 0;

int main(){

	int i = 1, count = 0;

	Graph *graph = (Graph *)malloc(sizeof(struct graph));

	/* Read the first and the second value from the stdin */
	scanf("%d", &graph->numberRouters);
	scanf("%d", &graph->numberConnections);

	/* Alloc a list representing the graph */
	Node *nodesList = allocList(graph);
	graph->nodesList = nodesList;

	/* Read the input values */
	readInput(graph);

	/* outputArray to store in the first position the number of SCCs and then the subgraphs' id (The highest id in each subgraph) */
	int *outputArray = calloc(graph->numberRouters+1, sizeof(int));

	outputArray = findSCC(graph, outputArray);

	printf("%d\n", outputArray[0]);

	while (i <= graph->numberRouters){
		if (outputArray[i] == 1){
			if (count == 0){
				printf("%d", i);
				count++;
			}
			else
				printf(" %d", i);
		}
		i++;
	}

	printf("\n");

	printf("%d\n", numArticulationPoints);

	/*printf("printing: \n");
	printList(graph);*/

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
	
	int i = 0;
	int numberSCCs = 0, aux = 0;

	Node *nodesList = graph->nodesList;
	Node *parent = NULL;

	for (i = 1; i <= graph->numberRouters; i++){
		if (nodesList[i].discovered == -1){
			aux = TarjanAdapt(parent, &nodesList[i], nodesList);
			outputArray[aux] = 1;
			numberSCCs++;
			sccId = 0;
		}
	}

	outputArray[0] = numberSCCs;

	return outputArray;
}


int TarjanAdapt(Node *parent, Node *node, Node *nodesList){
	
	static int time = 1;
	int rootTreeEdges = 0;

	node->discovered = time++;
	node->low = node->discovered;
	Neighbour *iter = node->first;

	if (sccId < node->id)
		sccId = node->id;

	while(iter != NULL){
		if (nodesList[iter->id].discovered == -1){
			rootTreeEdges++;
			TarjanAdapt(node, &nodesList[iter->id], nodesList);
			
			node->low = min(nodesList[iter->id].low, node->low);

			if ((nodesList[iter->id].low >= node->discovered) && (parent != NULL) && (node->articulationPoint == 0)) {
				node->articulationPoint = 1;
				numArticulationPoints++;
			}

			else if ((rootTreeEdges > 1) && (parent == NULL) && (node->articulationPoint == 0)) {
				node->articulationPoint = 1;
				numArticulationPoints++;
			}
		}

		else if (parent != NULL) {
			if (iter->id != parent->id)
				if (node->low > nodesList[iter->id].discovered)
					node->low = nodesList[iter->id].discovered;
		}

		iter = iter->next;
	}

	if (node->discovered == nodesList[node->id].low)
		return sccId;

	return 0;
}

int min(int n1, int n2){
	if (n1 < n2)
		return n1;

	return n2;
}