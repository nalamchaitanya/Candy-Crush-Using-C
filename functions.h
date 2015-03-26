#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//Defining a structure candy which simulates the basic element of the game
typedef struct _candy candy;
struct _candy
{
	int candy_type;		//describes the type of the candy
	int power_type;		//describes the power of the candy
};

//makes a matrix with given no of rows and columns and returns pointer.
candy** make_candy_matrix(int r,int c);					

//gives random values to each element from 1 to limit value which is given as argument.	
void give_random_candy(candy** matrix,int r,int c,int candy_types);

// if 5 power is swapped then it deletes all candies of that type.
int power_swap(candy** matrix,int r1,int c1,int r2,int c2,int candy_types);

//swaps the candies in two adjacent positions
int swap_candy(candy** matrix,int r1,int c1,int r2,int c2,int candy_types);

// removes candy and gives new candy at the top.
void delete_candy(candy** matrix,int r,int c,int candy_types);

//removes candy by checking its power value and do accordingly.
void remove_candy(candy** matrix,int r,int c,int candy_types);

//gives candy to the vacant position created at the top after candy is removed.
void give_candy(candy** matrix,int r,int c,int candy_types);

//prints the given candy matrix
void print_candy_matrix(candy** matrix,int r,int c);

//checks the horizontal lines to smash the candies.
int horizontal_check(candy** matrix,int r,int c,int candy_types);

//checks the vertical lines to smash the candies.
int vertical_check(candy** matrix,int r,int c,int candy_types);

#endif