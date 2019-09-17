#include<stdio.h>
#include<stdlib.h>

#include "moves_list.h"
#include "mainaux.h"

/**
* written by Yair at 29/07/2019
* This module consists the moves list.
*/




/**
 * initialize the moves list.
 * @param list {MovesList*} The moves list.
 * @param grid_height {int} the height of the board.
 * @param grid_width {int} the width of the board.
 */
void init_move_list(struct MovesList*  list, int grid_height, int grid_width){
	list->current_move=NULL;
    list->board_height = grid_height;
    list->board_width = grid_width;
}

/**
 * creates a new move to add to the list.
 * @param game_board {board} the current game board to be added to the move.
 * @param list {MovesList*} the moves list.
 * @return {Node*} the new move.
 */
struct Node* create_new_move (struct Cell** game_board, struct MovesList* list) {

	
	struct Node* new_node = NULL;
    board temp_board = NULL;
	new_node = (struct Node*)malloc(sizeof(struct Node));

	if(new_node == NULL){
        printf(FUNCTION_FAILED, "malloc");
        exit(EXIT_FAILURE);
    }

	temp_board = create_empty_board(list->board_height, list->board_height);
	copy_board(game_board, temp_board, list->board_height, list->board_width);
	new_node->data = temp_board;
	new_node->prev = NULL;
	new_node->next = NULL;
	return new_node;
}


/**
 * Finds the head of the list
 * @param list {MovesList*} The moves list
 * @return {Node*} the list head, NULL if it doesnt exist
 */
struct Node* find_list_head(struct MovesList*  list){
	struct Node* temp = list->current_move;

	if(list->current_move == NULL){
		return NULL;
	}

	while (temp->prev != NULL ){
		temp = temp->prev;
	}
	
	return temp;
}


/**
 * Free a list from memory, from the head.
 * @param list {movesList*} The moves list
 * @param board_height {int} the board height the list cells contain.
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
 * free the whole list from memory.
 * @param list {MovesList*} The moves list
 */
void free_whole_list(struct MovesList*  list){
	struct Node* head_to_free = find_list_head(list);
	if(list->current_move != NULL)
		free_partial_list(head_to_free, list->board_height);
}

/**
 * Add the board to the move list.
 * @param game_board {board} The game board
 * @param list {MovesList*} The moves list
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


/**
 * Undo the current move from the list only.
 * @param list {MovesList*} The moves list
 * @return {Node*} the current move if it exists, Null otherwise
 */
struct Node* psuedo_undo(struct MovesList*  list){
	
	if (list->current_move == NULL || list->current_move->prev == NULL){
		return NULL;
	} else {
		list->current_move = list->current_move->prev;
		return list->current_move;
	}
}


/**
 * Redo the current move from the list only.
 * @param list {MovesList*} The moves list.
 * @return {Node*} the current move if it exists, Null otherwise.
 */
struct Node* psuedo_redo(struct MovesList*  list){

	if (list->current_move == NULL || list->current_move->next == NULL){
		return NULL;
	} else {
		list->current_move = list->current_move->next;
		return list->current_move;
	}
}

/**
 * Reset the current list to the starting move.
 * @param list {MovesList*} The moves list.
 * @return {Node*} the current move.
 */
struct Node* psuedo_reset(struct MovesList*  list){
	list->current_move = find_list_head(list);
	return list->current_move;
}

/**
 * Restart the move list and free the whole list as well.
 * @param list {MovesList*} The moves list
 */
void restart_list(struct MovesList*  list){
	free_whole_list(list);
	list->current_move = NULL;
}
