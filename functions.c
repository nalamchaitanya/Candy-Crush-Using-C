#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include "functions.h"

int score;	//defining a global variable for score.

//makes a matrix with given no of rows and columns and returns pointer.
candy** make_candy_matrix(int r,int c)
{
	int i;
	candy** array=(candy**)(malloc(c*sizeof(candy*)));	//declaring a double pointer which has array of pointers.
	for (i = 0; i < c; ++i)
	{
		*(array+i)=(candy*)(malloc(r*sizeof(candy)));	//each point in array is being allocated a column.
	}
	return array;
}

//gives random values to each element from 1 to limit value which is given as argument.	
void give_random_candy(candy** matrix,int r,int c,int candy_types)
{
	int i,j;
	time_t seconds;
	time(&seconds);
	srand((unsigned int)seconds);		//seeding the random function
	for(i=0;i<r;i++)
	{
		for(j=0;j<c;j++)
		{
			(*(*(matrix+j)+i)).candy_type=(rand()%candy_types)+1;	//calls random function and uses modulo function to limit value to k.
			(*(*(matrix+j)+i)).power_type=0;
		}
	}
	return;
}

//prints the given candy matrix
void print_candy_matrix(candy** matrix,int r,int c)
{
	int i,j;
	for (i = 0; i < r; ++i)
	{
		printf("%d  ",i );
		for (j = 0; j <c; ++j)
		{
			printf(" %d ",(*(*(matrix+j)+i)).candy_type);
		}
		printf("\n");
	}
	return;
}

// if 5 power is swapped then it deletes all candies of that type.
int power_swap(candy** matrix,int r1,int c1,int r2,int c2,int candy_types)
{
	int i,j,cand,flag;
	flag=0;
	if((*(*(matrix+c1)+r1)).power_type==4)
	{
		cand=(*(*(matrix+c2)+r2)).candy_type;
		for(i=0;i<9;i++)
		{
			for (j=0;j<9;j++)
			{
				if((*(*(matrix+j)+i)).candy_type==cand)
				{
					delete_candy(matrix,i,j,candy_types);
					flag++;
				}
			}
		}
		delete_candy(matrix,r1,c1,candy_types);
	}
	else if((*(*(matrix+c2)+r2)).power_type==4)
	{
		cand=(*(*(matrix+c1)+r1)).candy_type;
		for(i=0;i<9;i++)
		{
			for (j=0;j<9;j++)
			{
				if((*(*(matrix+j)+i)).candy_type==cand)
				{
					flag++;
					delete_candy(matrix,i,j,candy_types);
				}
			}
		}
		delete_candy(matrix,r1,c1,candy_types);
	}
	return flag;
}

//swaps the candies in two adjacent positions
int swap_candy(candy** matrix,int r1,int c1,int r2,int c2,int candy_types)
{
	int r,c,flag;
	r=r2-r1;
	c=c2-c1;
	if(r<0)
		r=-r;
	if(c<0)
		c=-c;
	if(!((r==1&&c!=1)||(r!=1&&c==1)))	//checking whether the candies are adjacent or not
	{
		return 0;	
	}
	if((((*(*(matrix+c1)+r1)).power_type==4)||((*(*(matrix+c2)+r2)).power_type==4)))
	{
		return power_swap(matrix,r1,c1,r2,c2,candy_types);
	}
	candy temp;
	temp=(*(*(matrix+c1)+r1));
	(*(*(matrix+c1)+r1))=(*(*(matrix+c2)+r2));		//Swapping the candies
	(*(*(matrix+c2)+r2))=temp;
	return 0;
}

// removes candy and gives new candy at the top.
void delete_candy(candy** matrix,int r,int c,int candy_types)
{
	while(r!=0)
	{
		swap_candy(matrix,r,c,r-1,c,candy_types);		//swaps until it reaches the start and then replaces with new candy
		r--;
	}
	give_candy(matrix,r,c,candy_types);
	return;
}

//removes candy by checking its power value and do accordingly.
void remove_candy(candy** matrix,int r,int c,int candy_types)
{
	int i;
	if((*(*(matrix+c)+r)).power_type==0)
	{	
		delete_candy(matrix,r,c,candy_types);
		return;
	}
	else if((*(*(matrix+c)+r)).power_type==1)
	{
		for(i=0;i<9;i++)
		{
			delete_candy(matrix,r,i,candy_types);
		}
	}
	else if((*(*(matrix+c)+r)).power_type==2)
	{
		for(i=8;i>=0;i--)
		{
			give_candy(matrix,i,c,candy_types);
		}
	}
	return;
}

//gives candy to the vacant position created at the top after candy is removed
void give_candy(candy** matrix,int r,int c,int candy_types)
{
	struct timeval seed;	//defining the required structures to call the gettimeofday function.
	struct timezone tzp;
	gettimeofday(&seed,&tzp);
	srand((unsigned int)(seed.tv_usec));
	(*(*(matrix+c)+r)).candy_type=(rand()%candy_types)+1;
	(*(*(matrix+c)+r)).power_type=0;
	return;	
}

//checks the horizontal lines to smash the candies
int horizontal_check(candy** matrix,int r,int c,int candy_types)
{
	int i,j,flag,cand;
	flag=0;
	for(i=0;i<r;i++)
	{
		for(j=2;j<c-2;j++)
		{
			//checking whether five candies are matching
			if(((*(*(matrix+j-2)+i)).candy_type==(*(*(matrix+j-1)+i)).candy_type)
			&&((*(*(matrix+j-1)+i)).candy_type==(*(*(matrix+j)+i)).candy_type)
			&&((*(*(matrix+j)+i)).candy_type==(*(*(matrix+j+1)+i)).candy_type)
			&&((*(*(matrix+j+1)+i)).candy_type==(*(*(matrix+j+2)+i)).candy_type))
			{
				cand=(*(*(matrix+j-2)+i)).candy_type;
				remove_candy(matrix,i,j-2,candy_types);
				remove_candy(matrix,i,j-1,candy_types);
				(*(*(matrix+j)+i)).power_type=4;
				(*(*(matrix+j)+i)).candy_type=0;
				remove_candy(matrix,i,j+1,candy_types);
				remove_candy(matrix,i,j+2,candy_types);
				flag=flag+3;	//score		
			}
		}
	}
	for(i=0;i<r;i++)
	{
		for(j=1;j<c-2;j++)
		{
			//checking whether four candies are matching
			if(((*(*(matrix+j-1)+i)).candy_type==(*(*(matrix+j)+i)).candy_type)
				&&((*(*(matrix+j)+i)).candy_type==(*(*(matrix+j+1)+i)).candy_type)
				&&((*(*(matrix+j+1)+i)).candy_type==(*(*(matrix+j+2)+i)).candy_type))
			{
				remove_candy(matrix,i,j-1,candy_types);
				(*(*(matrix+j)+i)).power_type=2;
				remove_candy(matrix,i,j+1,candy_types);
				remove_candy(matrix,i,j+2,candy_types);
				flag=flag+2;	//score			
			}
		}
	}
	for(i=0;i<r;i++)
	{
		for(j=1;j<c-1;j++)
		{
			//checking whether three candies are matching
			if(((*(*(matrix+j-1)+i)).candy_type==(*(*(matrix+j)+i)).candy_type)
				&&((*(*(matrix+j)+i)).candy_type==(*(*(matrix+j+1)+i)).candy_type))
			{
				remove_candy(matrix,i,j-1,candy_types);
				remove_candy(matrix,i,j,candy_types);
				remove_candy(matrix,i,j+1,candy_types);
				flag++;		//score
			}
		}
	}
	return flag;
}

int vertical_check(candy** matrix,int r,int c,int candy_types)
{
	int i,j,flag,cand;
	flag=0;
	for(i=0;i<c;i++)
	{
		for(j=2;j<r-2;j++)
		{
			//checking whether five candies are matching
			if(((*(*(matrix+i)+j-2)).candy_type==(*(*(matrix+i)+j-1)).candy_type)
			&&((*(*(matrix+i)+j-1)).candy_type==(*(*(matrix+i)+j)).candy_type)
			&&((*(*(matrix+i)+j)).candy_type==(*(*(matrix+i)+j+1)).candy_type)
			&&((*(*(matrix+i)+j+1)).candy_type==(*(*(matrix+i)+j+2)).candy_type))
			{
				cand=(*(*(matrix+i)+j-2)).candy_type;
				remove_candy(matrix,j-2,i,candy_types);
				remove_candy(matrix,j-1,i,candy_types);
				remove_candy(matrix,j,i,candy_types);
				remove_candy(matrix,j+1,i,candy_types);
				(*(*(matrix+i)+j+2)).power_type=4;
				(*(*(matrix+i)+j+2)).candy_type=0;
				flag=flag+3;	//score		
			}
		}
	}
	for(i=0;i<c;i++)
	{
		for(j=1;j<r-2;j++)
		{
			//checking whether four candies are matching
			if(((*(*(matrix+i)+j-1)).candy_type==(*(*(matrix+i)+j)).candy_type)
			&&((*(*(matrix+i)+j)).candy_type==(*(*(matrix+i)+j+1)).candy_type)
			&&((*(*(matrix+i)+j+1)).candy_type==(*(*(matrix+i)+j+2)).candy_type))
			{
				remove_candy(matrix,j-1,i,candy_types);
				remove_candy(matrix,j,i,candy_types);
				remove_candy(matrix,j+1,i,candy_types);
				(*(*(matrix+i)+j+2)).power_type=1;
				flag=flag+2;	//score		
			}
		}
	}
	for(i=0;i<c;i++)
	{
		for(j=1;j<r-1;j++)
		{
			//checking whether three candies are matching
			if(((*(*(matrix+i)+j-1)).candy_type==(*(*(matrix+i)+j)).candy_type)
			&&((*(*(matrix+i)+j)).candy_type==(*(*(matrix+i)+j+1)).candy_type))
			{
				remove_candy(matrix,j-1,i,candy_types);
				remove_candy(matrix,j,i,candy_types);
				remove_candy(matrix,j+1,i,candy_types);
				flag++;		//score	
			}
		}
	}
	return flag;
}