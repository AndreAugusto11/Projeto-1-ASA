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
int findArticulationPoints(Graph *graph);
int countArticulationPoints(Node *node, Node *nodesList, int *visited);
int exploreArticulationPoints(Graph *graph);
int exploreWay(Node *node, Node *nodesList, int *visitedCount);


int SIZE = 25;								/* Size of output array in findScc */
int rootTreeEdges = 0;						/* If the root has at least two tree edges then it is an articulation point */
int auxCount = 0;
int count = 0;
int articulationPointId = 0;

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

	printf("%d\n", findArticulationPoints(graph));

	printf("%d\n", exploreArticulationPoints(graph));

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

int findArticulationPoints(Graph *graph){

	Node *nodesList = graph->nodesList;
	int i = 0, totalArtPoints = 0, aux = 0;

	int *visitedCount = calloc((graph->numberRouters + 1), sizeof(int));			/* Used when exploring the graph to count the number of articulation points */

	for (i = 1; i <= graph->numberRouters; i++){
		if (visitedCount[nodesList[i].id] == 0){
			totalArtPoints = countArticulationPoints(&nodesList[i], nodesList, visitedCount);
			rootTreeEdges = 0;
		}
	}

	free(visitedCount);
	return totalArtPoints;
}

int countArticulationPoints(Node *node, Node *nodesList, int *visitedCount){

	static int numArticulationPoints = 0;
	int i = 0, countAux = 0;

	visitedCount[node->id] = 1;
	Neighbour *iter = node->first;

	while(iter != NULL){
		if (visitedCount[iter->id] == 0	){
			if ((node->discovered == node->low) && (rootTreeEdges != 2)){
				rootTreeEdges++;
			}

			countArticulationPoints(&nodesList[iter->id], nodesList, visitedCount);
		}

		if (nodesList[iter->id].low >= node->discovered){
			if (node->discovered != node->low){
				if (node->articulationPoint == 0){
					node->articulationPoint = 1;
					numArticulationPoints++;
				}
			}

			else {
				if (rootTreeEdges == 2){
					if (node->articulationPoint != 1){
						node->articulationPoint = 1;
						numArticulationPoints++;
					}
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


/* Returns the number of elements in the biggest subgraph that results in removing all articulation points */
int exploreArticulationPoints(Graph *graph){

	Node *nodesList = graph->nodesList;
	
	int *visitedCount = calloc(graph->numberRouters, sizeof(int));
	int i = 0, maxAux = 0, max = 0;

	for (i = 1; i <= graph->numberRouters; i++){
		if ((visitedCount[nodesList[i].id] == 0) && (nodesList[i].articulationPoint == 1)){
			articulationPointId = nodesList[i].id;
			maxAux = exploreWay(&nodesList[i], nodesList, visitedCount);
			if (maxAux > max)
				max = maxAux;
			auxCount = 0;
		}
	}

	free(visitedCount);
	return max;
}


int exploreWay(Node *node, Node *nodesList, int *visitedCount){

	Neighbour *iter = node->first;
	visitedCount[node->id] = 1;

	while(iter != NULL){
		if ((visitedCount[iter->id] != 1) && (nodesList[iter->id].articulationPoint == 0)){
			auxCount++;
			exploreWay(&nodesList[iter->id], nodesList, visitedCount);
		}

		if (iter->id == articulationPointId){
			if (auxCount > count)
				count = auxCount;
		}

		iter = iter->next;
	}

	auxCount = 0;
	return count;
}