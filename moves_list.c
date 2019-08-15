#include<stdio.h>
#include<stdlib.h>

#include "moves_list.h"
#include "game.h"
#include "mainaux.h"

/**
* written by Yair at 29/07/2019
* This module consists the moves list.
*/





void init_move_list(struct List_of_moves*  list){

	list->current_move=NULL;
}

/***
* creates a new node.
*
* needs more parameters
*/
struct Node* create_new_move (struct Cell **grid, struct List_of_moves* list) {

	struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
	if(!new_node){
		/*ERROR HANDLING - MALLOC FAILED*/
		exit(-1);
	}
	struct Cell **temp_board = create_empty_board(list->board_height, list->board_width);
	copy_board(grid, temp_board, list->board_height, list->board_width);
	new_node->data = temp_board;
	new_node->prev = NULL;
	new_node->next = NULL;
	printf("created a new move \n");
	return new_node;
}






//Prints all the elements in linked list in forward traversal order
void Print_list(struct List_of_moves*  list) {
	struct Node* temp = list->current_move;
	printf("Forward: ");
	while(temp != NULL) {
		/* TODO: Align to the real print_board */
		print_board(temp->data,list->board_height, list->board_width,3,3);//TEMPORARY VALUES
		temp = temp->next;
	}
	printf("\n");
}

/**
*
*/
struct Node* find_list_head(struct List_of_moves*  list){
	struct Node* temp = list->current_move;

	while (temp->prev != NULL ){
		temp = temp->prev;
	}
	return temp;
}


/**
* A method that frees the list from the head down.
*
*/
void free_partial_list(struct Node* head, int board_height){
	struct Node* temp_node;

	while(head != NULL){

		temp_node = head;
		head = head->next;
		temp_node->prev=NULL;
		free_board(temp_node->data,board_height);//free the data
		free(temp_node);
	}
}

/**
* A method that frees the whole list.
*
*/
void free_whole_list(struct List_of_moves*  list){
	struct Node* head_to_free = find_list_head(list);
	free_partial_list(head_to_free, list->board_height);
}

/**
* Add a move to the list.
*/
void add_move ( struct Cell** grid, int grid_height, int grid_width, struct List_of_moves*  list){
	struct Node* new_move = create_new_move(grid, list);
	struct Node* curr_move = list->current_move;
	if(list->current_move == NULL){
			list->current_move = new_move;
	} else {
		if ( list->current_move->next == NULL ){

			list->current_move->next = new_move;
			new_move->prev = list->current_move;
			list->current_move = list->current_move->next;

		} else {

			/*if there exists a list after this current move we need to free it.*/
			free_partial_list(list->current_move->next,list->board_height);
			list->current_move->next = new_move;
			new_move->prev = list->current_move;
			list->current_move = list->current_move->next;

		}
	}
}



/*
*	A method to cast undo on the list of moves.
*	returns the previous move if succeed, NULL if cannot undo.
**/
struct Node* psuedo_undo(struct List_of_moves*  list){
	
	if (list->current_move == NULL || list->current_move->prev == NULL){
		return NULL;
	} else {
		list->current_move = list->current_move->prev;
		return list->current_move;
	}
}

struct Node* psuedo_redo(struct List_of_moves*  list){

	if (list->current_move == NULL || list->current_move->next == NULL){
		return NULL;
	} else {
		list->current_move = list->current_move->next;
		return list->current_move;
	}
}

struct Node* psuedo_reset(struct List_of_moves*  list){
	list->current_move = find_list_head(list);
	return list->current_move->data;
}


void restart_list(struct List_of_moves*  list){
	free_whole_list(list);
	list->current_move = NULL;
}