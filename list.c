#include <stdio.h>
#include <string.h>


Node* allocList(int size){

	int i;
	/* Ignoring the list's first position */
	Node *nodesList = (Node *)malloc(sizeof(struct node)*(size + 1));

	for (i = 0; i < size; i++){
		nodesList[i].id = i;
		nodesList[i].discovered = -1;
		nodesList[i].low = -1;
		nodesList[i].onStack = false;
		nodesList[i].first = NULL;
	}

	return nodesList;
}


void addNode(int node1, int node2, Node *nodesList){

	Neighbour *newElement = (Neighbour*)malloc(sizeof(struct neighbour));
	newElement->id = node2;
	newElement->next = nodesList[node1].first;
	nodesList[node1].first = newElement;
}


void printList(Graph *graph){

	int i;
	Neighbour *iter;
	Node *nodesList = graph->nodesList;

	for (i = 0; i < graph->numberRouters; i++){
		iter = nodesList[i].first;
		printf("%d: ", i);

		if (iter == NULL){
			printf("\n");
			continue;
		}

		else {

			while(iter != NULL){
				printf("%d ", iter->id);
				iter = iter->next;
			}
			
			printf("\n");
		}
	}
}

void freeList(Graph *graph){

	int i;
	Neighbour *iter1, *iter2;
	Node *nodesList = graph->nodesList;

	for (i = 0; i < graph->numberRouters; i++){
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