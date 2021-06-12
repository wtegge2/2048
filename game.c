#include "game.h"

// Introduction Paragraph
// This program allows the user to play the popular game 2048. The game gives a MxM grid with values in each grid, and the goal of the game is to 
// swipe either up, down, left, or right in order to combine the values. A new value with spawn randomly on the grid whenever the user combines 
// values. There are several functions in this program. There is the make_game functino which makes a new instance of the game. There is the remake_game 
// function which basically does the same thing as the make_game fucnction. There is the get_cell function which is used to access the game cells.
// Then there are the shift functions and move functions. The shift functions shift the whole row/column in the specified direction so that all the 
// cells with values in them are touching each other. The shift functions do not combine values. That is what the move functions do. After the shift,
// the move functions combine the adjacent cells in the specified direction. Lastly, the legal_move_check function checks if there are anymore legal 
// moves left for the user to do. If not, then the game ends. 

// partners: wtegge2, ddecla3, jcruda2


game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct

	mygame->rows = rows; // sets rows in the struct
	mygame->cols = cols; // sets columns in the struct
	mygame->score = 0; // sets score in the struct to 0

	for (int i = 0; i < cols; i++) { //for loop for columns
		for (int j = 0; j < rows; j++) { //for loop for rows
			(mygame->cells)[j*cols+i] = -1; // goes through setting up the game to be empty
		}
	}

    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
	
	game * mygame = *_cur_game_ptr; // sets mygame pointer to the address of current game 

	mygame->rows = new_rows; // sets rows in the struct
	mygame->cols = new_cols; // sets columns in the struct
	mygame->score = 0; // sets score in the struct to 0
	
	for (int i = 0; i < new_cols; i++) { //for loop for columns
		for (int j = 0; j < new_rows; j++) { //for loop for rows
			(mygame->cells)[j*new_cols+i] = -1; // goes through setting up the game to be empty
		}
	}

	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE

	if ((row >= 0) && (row < cur_game->rows)) { // checks if row of cell is in bounds
		if ((col >= 0) && (col < cur_game->cols)) { // checks if column of cell is in bounds
			return (cur_game->cells + row*cur_game->cols+col); // return cell
		}
	}
    		return NULL; // return nothing
}

int shift_up(game * cur_game){
// helper function for move_w (shifts every value up)
	int r = cur_game->rows;
	int c = cur_game->cols;
	int i;
	int j;
	int shift_flag = 0; //initializes flag

	for(j = 0; j<c; j++){ //loops through all columns
		int empty; //must find first empty cell
		for(empty=0; empty<r && *get_cell(cur_game, empty, j) != -1; empty++);
		for(i = empty+1; i<r; i++){ //loops through each cell that is able to shift up
			if(*get_cell(cur_game, i, j) != -1){ // checks if value needs to be shifted up
			*get_cell(cur_game, empty, j) = *get_cell(cur_game, i, j); // shifts value up to empty spot
			*get_cell(cur_game, i, j) = -1; // clear value's previous spot
			empty++; // increment empty spot because the empty spot has shifted
			shift_flag = 1; // sets flag to 1 if there is a shift
			}
		}
	}
	return shift_flag; 
}


int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can
   not merge twice in one turn. If sliding the tiles up does not cause any
   cell to change value, w is an invalid move and return 0. Otherwise, return 1.
*/
/* Get algorithm from lab video or on the wiki
   Go into each of the N cols in the MxN matrix and slide each column up
   Must make sure that the row above the target row* has not been combined yet
   Go through each pair, and if they match then combine them

*/
{
    //YOUR CODE STARTS HERE

	int r = cur_game->rows; // r = rows
	int c = cur_game->cols; // c = cols
	int i;
	int j;
	int s = cur_game->score; // s = score
	int comb_flag = 0; //initalizes variable
	int first_shift_flag = 0; //initalizes variable

	first_shift_flag = shift_up(cur_game); // call function to shift everything up (all nonempty now touching)

	for(i=0; i<r-1; i++){ //loop through the top of each pair
		for(j=0; j<c; j++){ //loop through each column
			if((*get_cell(cur_game, i, j) != -1) && (*get_cell(cur_game, i, j)==*get_cell(cur_game, i+1, j))){ //checks if matches the value below it
			*get_cell(cur_game, i, j) *= 2; //doubles the top value (combining them)
			*get_cell(cur_game, i+1, j) = -1; //clears the bottom value, making it empty
			cur_game->score = s + *get_cell(cur_game, i, j); // increases score by the value of new tile
			comb_flag = 1; //sets comb_flag to one 
			}
		}
	}
	if(shift_up(cur_game) || comb_flag || first_shift_flag) // once things combined, it shifts again and checks if overall move is valid based on the flags
    		return 1;
	else
		return 0;
};


int shift_down(game * cur_game){
// helper function for move_s (shifts every value down)

	int r = cur_game->rows;
	int c = cur_game->cols;
	int i;
	int j;
	int shift_flag = 0; //initializes flag

	for(j = 0; j<c; j++){ //loops through all columns
		int empty; //must find first empty cell
		for(empty=r-1; empty>=0 && *get_cell(cur_game, empty, j) != -1; empty--);
		for(i = empty-1; i>=0; i--){ //loops through each cell that is able to shift down
			if(*get_cell(cur_game, i, j) != -1){ // checks if value needs to be shifted down
				*get_cell(cur_game, empty, j) = *get_cell(cur_game, i, j); // shifts value down to empty spot
				*get_cell(cur_game, i, j) = -1; // clear value's previous spot
				empty--; // increment empty spot because the empty spot has shifted
				shift_flag = 1; // sets flag to 1 if there is a shift
			}
		}
	}
	return shift_flag;
}


int move_s(game * cur_game) //slide down
// same as move_w, but slide down. Basically same algorithm, but just adjust for difference in direction
{
    //YOUR CODE STARTS HERE

	int r = cur_game->rows; // r = rows
	int c = cur_game->cols; // c = cols
	int i;
	int j;
	int s = cur_game->score; // s = score
	int comb_flag = 0; //initializes variable
	int first_shift_flag = 0; //initializes variable

	first_shift_flag = shift_down(cur_game); // call function to shift everything down (all nonempty now touching)
	
	for(i=r-1; i>0; i--){ //loop through the top of each pair
		for(j=0; j<c; j++){ //loop through each column
			if((*get_cell(cur_game, i, j) != -1) && (*get_cell(cur_game, i, j)==*get_cell(cur_game, i-1, j))){ //checks if matches the value below it
				*get_cell(cur_game, i, j) *= 2; //doubles the bottom value (combining them)
				*get_cell(cur_game, i-1, j) = -1; //clears the top value, making it empty
				cur_game->score = s + *get_cell(cur_game, i, j); // increases score by the value of new tile
				comb_flag = 1; //sets comb_flag to 1
			}
		}
	}

	if(shift_down(cur_game) || comb_flag || first_shift_flag) // once things combined, it shifts again and checks if overall move is valid based on the flags
		return 1;
	else
		return 0;
};




int shift_left(game * cur_game){
// helper function for move_a function (shifts every value left)

	int r = cur_game->rows;
	int c = cur_game->cols;
	int i;
	int j;
	int shift_flag = 0; //initializes flag

	for(i = 0; i<r; i++){ //loops through all columns
		int empty; //must find first empty cell
		for(empty=0; empty<c && *get_cell(cur_game, i, empty) != -1; empty++);
		for(j = empty+1; j<c; j++){ //loops through each cell that is able to shift left
			if(*get_cell(cur_game, i, j) != -1){ // checks if value needs to be shifted left
				*get_cell(cur_game, i, empty) = *get_cell(cur_game, i, j); // shifts value left to empty spot
				*get_cell(cur_game, i, j) = -1; // clear value's previous spot
				empty++; // increment empty spot because the empty spot has shifted
				shift_flag = 1; // sets flag to 1 if there is a shift
			}
		}
	}
	return shift_flag;
}

int move_a(game * cur_game) //slide left
// same as move_w, but slide left. Basically same algorithm, but just adjust for difference in direction
{
    //YOUR CODE STARTS HERE

	int r = cur_game->rows; // r = rows
	int c = cur_game->cols; // c = cols
	int i;
	int j;
	int s = cur_game->score; // s = score
	int comb_flag = 0; //initializes variable
	int first_shift_flag = 0; //initializes variable

	first_shift_flag = shift_left(cur_game); // call function to shift everything left (all nonempty now touching)
	
	for(j=0; j<c-1; j++){ //loop through the left of each pair
		for(i=0; i<r; i++){ //loop through each row
			if((*get_cell(cur_game, i, j) != -1) && (*get_cell(cur_game, i, j)==*get_cell(cur_game, i, j+1))){ //checks if matches the value below it
				*get_cell(cur_game, i, j) *= 2; //doubles the left value (combining them)
				*get_cell(cur_game, i, j+1) = -1; //clears the right value, making it empty
				cur_game->score = s + *get_cell(cur_game, i, j); // increases score by the value of new tile
				comb_flag = 1;
			}
		}
	}
	if(shift_left(cur_game) || comb_flag || first_shift_flag) // call function to shift everything left (all nonempty now touching)
		return 1;
	else
		return 0;
};



int shift_right(game * cur_game){
// helper function that shifts everything right

	int r = cur_game->rows;
	int c = cur_game->cols;
	int i;
	int j;
	int shift_flag = 0; //initializes flag

	for(i = 0; i<r; i++){ //loops through all columns
		int empty; //must find first empty cell
		for(empty=c-1; empty>=0 && *get_cell(cur_game, i, empty) != -1; empty--);
		for(j = empty-1; j>=0; j--){ //loops through each cell that is able to shift left
			if(*get_cell(cur_game, i, j) != -1){ // checks if value needs to be shifted left
				*get_cell(cur_game, i, empty) = *get_cell(cur_game, i, j); // shifts value left to empty spot
				*get_cell(cur_game, i, j) = -1; // clear value's previous spot
				empty--; // increment empty spot because the empty spot has shifted
				shift_flag = 1; // sets flag to 1 if there is a shift
			}
		}
	}
	return shift_flag;
}



int move_d(game * cur_game){ //slide to the right
// same as move_w, but slide right. Basically same algorithm, but just adjust for difference in direction
    //YOUR CODE STARTS HERE


	int r = cur_game->rows; // r = rows
	int c = cur_game->cols; // c = cols
	int i;
	int j;
	int s = cur_game->score; // s = score
	int comb_flag = 0; //initializes variable
	int first_shift_flag = 0; //initializes variable

	first_shift_flag = shift_right(cur_game); // call function to shift everything left (all nonempty now touching)

	for(j=c-1; j>0; j--){ //loop through the left of each pair
		for(i=0; i<r; i++){ //loop through each row
			if((*get_cell(cur_game, i, j) != -1) && (*get_cell(cur_game, i, j)==*get_cell(cur_game, i, j-1))){ //checks if matches the value below it
				*get_cell(cur_game, i, j) *= 2; //doubles the left value (combining them)
				*get_cell(cur_game, i, j-1) = -1; //clears the right value, making it empty
				cur_game->score = s + *get_cell(cur_game, i, j); // increases score by the value of new tile
				comb_flag = 1;
			}
		}
	}
	if(shift_right(cur_game) || comb_flag || first_shift_flag) // once things combined, it shifts again and checks if overall move is valid based on the flags
		return 1;
	else
		return 0;
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE

	int r = cur_game->rows; // r = rows
	int c = cur_game->cols; // c = cols
	int i;
	int j;
	for (i = 0; i < r; i++) { //for loop for rows
		for (j = 0; j < c; j++) { //for loop for columns
			if(*get_cell(cur_game, i, j)==-1) { //valid move if cell is empty
				return 1;
			}
			if(*get_cell(cur_game, i, j)!=-1) { //if cell has number, it checks if cell can move based on the next if statements
		
				if((i>0 && i<r-1) && ((*get_cell(cur_game, i, j)== *get_cell(cur_game, i+1, j)) || (*get_cell(cur_game, i, j)== *get_cell(cur_game, i-1, j))))
					return 1;
				if((i == 0) && (*get_cell(cur_game, i, j)== *get_cell(cur_game, i+1, j)))
					return 1;
				if((i == r) && (*get_cell(cur_game, i, j)== *get_cell(cur_game, i-1, j)))
					return 1;
	
				if((j>0 && j<c-1) && ((*get_cell(cur_game, i, j)== *get_cell(cur_game, i, j+1)) || (*get_cell(cur_game, i, j)== *get_cell(cur_game, i, j-1))))
					return 1;
				if((j == 0) && (*get_cell(cur_game, i, j)== *get_cell(cur_game, i, j+1)))
					return 1;
				if((j == c) && (*get_cell(cur_game, i, j)== *get_cell(cur_game, i, j-1)))
					return 1;
			}
		}
	}

   return 0; //returns 0 if move is invalid
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
