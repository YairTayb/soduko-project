#include<stdio.h>
#include<stdlib.h>

#include "moves_list.h"
#include "mainaux.h"

/**
* written by Yair at 29/07/2019
* This module consists the moves list.
*/





void init_move_list(struct MovesList*  list, int grid_height, int grid_width){
	list->current_move=NULL;
    list->board_height = grid_height;
    list->board_width = grid_width;
}

/***
* creates a new node.
*
* needs more parameters
*/
struct Node* create_new_move (struct Cell** game_board, struct MovesList* list) {

	
	struct Node* new_node ;
    board temp_board = NULL;
	new_node = (struct Node*)malloc(sizeof(struct Node));
    if(!new_node){
        /*ERROR HANDLING - MALLOC FAILED*/
        /* TODO: Add the new errors handling here */
        exit(-1);
    }
	temp_board = create_empty_board(list->board_height, list->board_height);
	copy_board(game_board, temp_board, list->board_height, list->board_width);
	new_node->data = temp_board;
	new_node->prev = NULL;
	new_node->next = NULL;
	printf("created a new move \n");
	return new_node;
}






/* Prints all the elements in linked list in forward traversal order */
void Print_list(struct MovesList*  list) {
	struct Node* temp = list->current_move;
	printf("Forward: ");
	while(temp != NULL) {
		/* TODO: Align to the real print_board */
		print_board(temp->data,list->board_height, list->board_width,3,3,solve_mode,0);
		temp = temp->next;
	}
	printf("\n");
}

/**
*
*/
struct Node* find_list_head(struct MovesList*  list){
	struct Node* temp = list->current_move;

	/* TODO - when temp is NULL - failing */
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
		free_board(temp_node->data,board_height);
		free(temp_node);
	}
}

/**
* A method that frees the whole list.
*
*/
void free_whole_list(struct MovesList*  list){
	struct Node* head_to_free = find_list_head(list);
	free_partial_list(head_to_free, list->board_height);
}

/**
* Add a move to the list.
*/
void add_move_to_list (board game_board, struct MovesList*  list){

	struct Node* new_move = create_new_move(game_board, list);
	
	if(list->current_move == NULL){
			list->current_move = new_move;
	} else {
		if ( list->current_move->next == NULL ){
			list->current_move->next = new_move;
			new_move->prev = list->current_move;
			list->current_move = list->current_move->next;

		} else {
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
struct Node* psuedo_undo(struct MovesList*  list){
	
	if (list->current_move == NULL || list->current_move->prev == NULL){
		return NULL;
	} else {
		list->current_move = list->current_move->prev;
		return list->current_move;
	}
}

struct Node* psuedo_redo(struct MovesList*  list){

	if (list->current_move == NULL || list->current_move->next == NULL){
		return NULL;
	} else {
		list->current_move = list->current_move->next;
		return list->current_move;
	}
}

struct Node* psuedo_reset(struct MovesList*  list){
	list->current_move = find_list_head(list);
	return list->current_move;
}


void restart_list(struct MovesList*  list){
	free_whole_list(list);
	list->current_move = NULL;
}
