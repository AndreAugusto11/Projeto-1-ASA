#include <stdio.h>
#include <stdlib.h>

/* AUTHOR: ANDRE ALVES AUGUSTO | NUM:90704 | LEIC-T | PROJETO ASA */

/****************************************************************
*																*
*						Defining structures						*
*																*
****************************************************************/

typedef struct neighbour{
	int id;
	struct neighbour *next;
} Neighbour;

typedef struct node {
	int id;
	int low;
	int discovered;
	int articulationPoint;
	struct neighbour *first;
} Node;

typedef struct graph {
	int numberRouters;
	int numberConnections;
	int numArticulationPoints;
	Node *nodesList;
} Graph;


/****************************************************************
*																*
*						Global variables						*
*																*
****************************************************************/


int sccId = 0;								/* Variable to help finding the subgraphsId's */
int maxSubgraphSize = 0;					/* Variable to help finding the size of the biggest subgraph resulting in removing all the articulation points */


/****************************************************************
*																*
*						Auxiliary Operations					*
*																*
****************************************************************/

Node* allocList(Graph *graph){

	int i;
	int size = graph->numberRouters;

	/* Ignoring the list's first position */
	Node *nodesList = calloc((size + 1), sizeof(struct node));

	/* Initializing the nodes' list */
	for (i = 1; i <= size; i++){
		nodesList[i].id = i;
		nodesList[i].discovered = -1;
		nodesList[i].low = -1;
		nodesList[i].articulationPoint = 0;
		nodesList[i].first = NULL;
	}

	return nodesList;
}


void addNode(int node1, int node2, Node *nodesList){
	/* Add a pair of nodes to the list */
	Neighbour *newElement = (Neighbour*)malloc(sizeof(struct neighbour));
	newElement->id = node2;
	newElement->next = nodesList[node1].first;
	nodesList[node1].first = newElement;
}


void freeList(Graph *graph){
	/* Iterates through the nodesList in the Graph structure to free all allocated memory */

	int i;
	Neighbour *iter1, *iter2;
	Node *nodesList = graph->nodesList;

	for (i = 0; i <= graph->numberRouters; i++){
		iter1 = nodesList[i].first;
		
		if (iter1 == NULL)
			continue;

		while (iter1->next != NULL){
			iter2 = iter1->next;
			free(iter1);
			iter1 = iter2;
		}

		free(iter1);
	}

	free(nodesList);
}

/****************************************************************
*																*
*						Main program							*
*																*
****************************************************************/


int min(int n1, int n2){
	/* Returns the minimum of two values */
	if (n1 < n2)
		return n1;

	return n2;
}


int searchGraph(Graph *graph, Node *parent, Node *node, Node *nodesList){
	/* Adaptation of the Tarjan's algorithm to find strongly connected components, its Id's and the number of articulation Points*/
	static int time = 1;
	int rootTreeEdges = 0;

	/* Initializing discover time, low time, and an iterator */
	node->discovered = time++;
	node->low = node->discovered;
	Neighbour *iter = node->first;

	if (sccId < node->id)
		sccId = node->id;

	while(iter != NULL){
		/* Iterates through all neighbours that haven't been visited yet */
		if (nodesList[iter->id].discovered == -1){
			rootTreeEdges++;
			searchGraph(graph, node, &nodesList[iter->id], nodesList);
			
			/* The low value of a vertex is updated if there's an adjacent vertex with a lowest low value */
			node->low = min(nodesList[iter->id].low, node->low);

			if ((nodesList[iter->id].low >= node->discovered) && (parent != NULL) && (node->articulationPoint == 0)) {
				/* Any given vertex that's not the root is an Articulation Point if it's discover time is less
				or equal to the low value of its adjacent vertices */
				node->articulationPoint = 1;
				graph->numArticulationPoints++;
			}

			else if ((rootTreeEdges > 1) && (parent == NULL) && (node->articulationPoint == 0)) {
				/* If the root of the DFS is an Articulation Point then it has 2 or more edges */
				node->articulationPoint = 1;
				graph->numArticulationPoints++;
			}
		}

		/* If the adjacent of a vertex has already been discovered then its low value is updated if necessary */
		else if (parent != NULL) {
			if (iter->id != parent->id)
				node->low = min(nodesList[iter->id].discovered, node->low);
		}

		iter = iter->next;
	}

	return sccId;
}


int* findSCC(Graph *graph, int *outputArray){
	/* Auxiliary function to implement an adaptation of the Tarjan's algorithm to find strongly connected components */ 
	int i = 0;
	int numberSCCs = 0, aux = 0;

	Node *nodesList = graph->nodesList;
	Node *parent = NULL;

	for (i = 1; i <= graph->numberRouters; i++){
		/* For each vertex that hasn't been visited */
		if (nodesList[i].discovered == -1){
			aux = searchGraph(graph, parent, &nodesList[i], nodesList);
			/* Storing the SCC's id */
			outputArray[aux] = 1;
			/* Increasing the count of the number of SCCs */
			numberSCCs++;
			sccId = 0;
		}
	}

	outputArray[0] = numberSCCs;

	return outputArray;
}


int exploreGraph(Node *node, Node *nodesList, int *visited){
	/* Exploring the graph the furthest possible to find the size of the biggest subgraph */
	Neighbour *iter = node->first;

	while (iter != NULL){
		/* For each vertex explore all possible paths.
		Ends when an articulation point is found or there's a dead end */
		if ((visited[iter->id] != 1) && (nodesList[iter->id].articulationPoint != 1)){
			visited[iter->id] = 1;
			maxSubgraphSize++;
			exploreGraph(&nodesList[iter->id], nodesList, visited);
		}

		iter = iter->next;
	}

	return maxSubgraphSize;
}


int maxSubgraph(Graph *graph){
	/* Helps when counting the size of the biggest subgraph that results in removing all the articulation points */
	int i = 0, max = 0, aux = 0;
	Node *nodesList = graph->nodesList;

	int *visited = calloc(graph->numberRouters+1, sizeof(int));

	for(i = 1; i < graph->numberRouters; i++){
		/* Starts exploring in the first unvisited vertex */
		if ((visited[i] != 1) && (nodesList[i].articulationPoint != 1)){
			visited[i] = 1;
			maxSubgraphSize = 1;
			aux = exploreGraph(&nodesList[i], nodesList, visited);
			/* Update the max value */
			if (aux > max)
				max = aux;
		}
	}

	free(visited);
	return max;
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


/****************************************************************
*																*
*						Main function							*
*																*
****************************************************************/

int main(){

	int i, count = 0;

	Graph *graph = (Graph *)malloc(sizeof(struct graph));

	/* Read the first and the second value from the stdin */
	scanf("%d", &graph->numberRouters);
	scanf("%d", &graph->numberConnections);

	/* Alloc a list representing the graph */
	Node *nodesList = allocList(graph);
	graph->nodesList = nodesList;

	graph->numArticulationPoints = 0;

	/* Read the input values */
	readInput(graph);

	/* outputArray to store in the first position the number of SCCs and then the subgraphs' id (The highest id in each subgraph) */
	int *outputArray = calloc(graph->numberRouters+1, sizeof(int));

	/* Calculate the number of sugraphs and each size */
	outputArray = findSCC(graph, outputArray);

	/* Printing the number of subgraphs */
	printf("%d\n", outputArray[0]);

	/* Printing the size of all subgraphs sorted */
	i = 1;
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

	/* printing the total number of articulation points */
	printf("%d\n", graph->numArticulationPoints);

	/* printing the size of the biggest subgraph that results in removing all the articulation points */
	printf("%d\n", maxSubgraph(graph));

	/* free all structures allocated */
	free(outputArray);
	freeList(graph);
	free(graph);

	return 0;
}