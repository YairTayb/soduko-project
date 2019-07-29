#include<stdio.h>
#include<stdlib.h>

#include "moves_list.h"

/**
* written by Yair at 29/07/2019
* This module consists the moves list.
* A double linked list, the current move the game is at is marked
* with the current_move pointer.
* we insert the board into the list BEFORE every move occurs.
*/
struct Node  {
	int data;
	struct Node* next;
	struct Node* prev;
};

struct Node* current_move;


void init_move_list(){
	//data = NULL
	current_move->next = NULL;
	current_move->prev = NULL;
}

/***
* creates a new node.
*
* needs more parameters
*/
struct Node* create_new_move (int x) {
	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
	new_node->data = x;// change to board later
	new_node->prev = NULL;
	new_node->next = NULL;
	printf("created a new move \n");
	return new_node;
}






//Prints all the elements in linked list in forward traversal order
void Print_list() {
	struct Node* temp = current_move;
	printf("Forward: ");
	while(temp != NULL) {
		printf("%d ",temp->data);
		temp = temp->next;
	}
	printf("\n");
}

/**
*
*/
struct Node* find_list_head(){
	struct Node* temp = current_move;

	while (temp->prev != NULL ){
		temp = temp->prev;
	}
	return temp;
}


/**
* A method that frees the list from the head down.
*
*/
void free_partial_list(struct Node* head){
	struct Node* temp_node;

	while(head != NULL){

		temp_node = head;
		head = head->next;
		temp_node->prev=NULL;
		//free(temp_node->data ); later on
		free(temp_node);
	}
}

/**
* A method that frees the whole list.
*
*/
void free_whole_list(){
	struct Node* head_to_free = find_list_head();
	free_partial_list(head_to_free);
}

/**
* Add a move to the list.
*/
void add_move (int x){
	struct Node* new_move = create_new_move(x);
	
	if(current_move == NULL){
			current_move = new_move;
	} else {
		if ( current_move->next == NULL ){

			current_move->next = new_move;
			new_move->prev = current_move;
			current_move = current_move->next;

		} else {

			/*if there exists a list after this current move we need to free it.*/
			free_partial_list(current_move->next);
			current_move->next = new_move;
			new_move->prev = current_move;
			current_move = current_move->next;

		}
	}
}



/*-----------------might to change to return data -----------------------------------*/
/*
*	A method to cast undo on the list of moves.
*	returns the previous move if succeed, NULL if cannot undo.
**/
int psuedo_undo(){
	
	if (current_move->prev == NULL){
		return NULL;
	} else {
		current_move = current_move ->prev;
		return current_move->data;
	}
}

int psuedo_redo(){

	if (current_move->next == NULL){
		return NULL;
	} else {
		current_move = current_move->next;
		return current_move->data;
	}
}

void psuedo_reset(){
	current_move = find_list_head();
}


/*-----------------might to change to return data -----------------------------------*/

int main(){
	printf("test start: \n");
	add_move(1);
	add_move(2);
	add_move(3);
	add_move(4);
	add_move(5);
	add_move(6);
	printf("finished adding \n");
	//current_move = find_list_head();
	//Print_list();
	/**********************/
	printf("starting to undo: \n");
	psuedo_undo();
	psuedo_undo();
	psuedo_undo();
	psuedo_undo();
	psuedo_undo();
	psuedo_undo();
	if(psuedo_undo() == NULL){
		printf("could not undo anymore! \n");
	}
	Print_list();

	printf("starting to redo: \n");
	psuedo_redo();
	psuedo_redo();
	psuedo_redo();
	psuedo_redo();
	psuedo_redo();
	psuedo_redo();
	if(psuedo_redo() == NULL){
		printf("could not redo anymore! \n");
	}
	Print_list();
	printf("resetted the moves: \n");
	psuedo_reset();
	Print_list();

	printf("split on the lsit move! \n");
	psuedo_redo();
	psuedo_redo();
	psuedo_redo();
	psuedo_redo();
	add_move(7);
	psuedo_reset();
	Print_list();
	printf("finished test \n");
}