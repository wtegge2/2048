#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>



/*! Defines data type cell, which functions identically to type int. */
typedef int cell;

/*! Defines a game, which is a just a grid of cells and game score. */
typedef struct 
{
	/* Number of rows in the grid */
    int rows;
	
	/* Number of columns in the grid */
	int cols;
	
	/* Pointer to the beginning of the board cell data.  The cells are organized
       in a 1D array in a row-major layout */
    cell * cells;
	
	/* Score of current game. */
	int score;
	
} game;



// added helper functions
int shift_up(game * cur_game);
int shift_down(game * cur_game);
int shift_left(game * cur_game);
int shift_right(game * cur_game);





game * make_game(int rows, int cols);
void destroy_game(game * cur_game);

cell * get_cell(game * cur_game, int row, int col);

int move_w(game * cur_game);
int move_s(game * cur_game);
int move_a(game * cur_game);
int move_d(game * cur_game);

int legal_move_check(game * cur_game);
void remake_game(game ** _cur_game,int new_rows,int new_cols);

int process_turn(const char input_char, game* cur_game);
int print_game(game * cur_game);
void rand_new_tile(game * cur_game);
