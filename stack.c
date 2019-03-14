#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack* allocStack(int size){

	Stack *s = (Stack *)malloc(sizeof(struct stack));
	s->capacity = size;
	s->top = -1;
	s->array = (int *)malloc(sizeof(struct stack)*size);

	return s;
}

int isFull(Stack *s){
	return s->top == s->capacity - 1;
}

int isEmpty(Stack *s){
	return s->top == -1;
}

void push(int element, Stack *s){
	if (!isFull(s))
		s->array[++(s->top)] = element;
}

int pop(Stack *s){
	if (!isEmpty(s))
		return s->array[s->top--];
}

void emptyStack(Stack *s){
	while (!isEmpty(s))
		pop(s);
}

void freeStack(Stack *s){
	free(s->array);
	free(s);
}

void printStack(Stack *s){
	int aux = s->top;

	while (aux != -1){
		printf("%d\n", s->array[aux--]);
	}
}