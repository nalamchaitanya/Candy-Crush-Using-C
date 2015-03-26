/*
* Author: Nalam V S S Krishna Chaitanya
* Roll No: CS13B021
*/

#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include <sys/time.h>

#define BOARD_SIZE 9
#define CANDY_TYPE 6
#define MOVES 10
#define TIME 40
#define TIME_CANDY_TYPES 6

//Global variables.
candy** board;
GtkWidget* wid[BOARD_SIZE][BOARD_SIZE], *window,*table;
int click_no,score,hscore,moves,CANDY_TYPES,timer,start_time,end_time;
int coordinates[4];
struct timeval start,end;	//defining the required structures to call the gettimeofday function.
struct timezone tzp;

//starts the game by defining the board and the initial settings
void play_callback(GtkWidget* widget,gpointer data);

//creates the window with updated high scores from the files
void hscore_callback(GtkWidget* widget,gpointer data);

//quits and destroys the window
void quit_callback(GtkWidget* widget,gpointer data);

//displays the score
void score_widget(GtkWidget* widget,gpointer data);

//it detects that the candies are being clicked and takes the values of positions and calls the swap_candy function
void swap_click(GtkWidget* widget,gpointer data);

//It prints gui board after every swapping
void print_gui_board(GtkWidget* widget,gpointer data);

//calls the main menu wherever the Main Menu button is clicked
void main_menu_callback(GtkWidget* widget,gpointer data);

//Basing on the values in the candy struct it decides the image to be put in place
void put_candy_image(int r,int c);

//Displays the score after the game is completed
void score_display();

//Checks the corresponding level of the game and gets the high score from the corresponding file and gives to local variable
void hscore_no();

//This is just for myself
void credits();

//This opens a window for the user to select the level and gives arguments accordingly
void level_callback();

//Frees the matrix after the score is displayed
void free_matrix(int N,candy** board);

int main(int argc, char *argv[])
{
	GtkWidget* button;
	GtkWidget* candy_crush;
	GdkColor window_colour;//button_colour;
	gdk_color_parse("pink",&window_colour);
	gtk_init(&argc,&argv);
	
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"Candy Crush");
	gtk_widget_modify_bg(window,GTK_STATE_NORMAL,&window_colour);
	gtk_window_set_default_size(GTK_WINDOW(window),440,440);
	//gtk_window_set_resizable(GTK_WINDOW(window),TRUE);

	table=gtk_table_new(9,9,TRUE);
	gtk_container_add(GTK_CONTAINER(window),table);

	g_signal_connect(window, "delete-event", G_CALLBACK(quit_callback), NULL);
	
	button=gtk_button_new();
	candy_crush=gtk_image_new_from_file("icon.png");
	gtk_button_set_image(GTK_BUTTON(button),candy_crush);
	g_signal_connect(button,"clicked",G_CALLBACK(main_menu_callback),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),button,0,9,0,9);
	gtk_widget_show(button);

	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}

//Not used
void score_widget(GtkWidget* widget,gpointer data)
{
	GtkWidget* button1;
	gchar* b=g_strdup_printf("%i",score);
	button1=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button1,11,13,7,8);
	gtk_widget_show(button1);
	gtk_widget_show_all(window);
	gtk_main();
	return;
}

//starts the game by defining the board and the initial settings
void play_callback(GtkWidget* widget,gpointer data)
{
	gtk_widget_destroy(table);
	table=gtk_table_new(11,14,TRUE);
	gtk_container_add(GTK_CONTAINER(window),table);
	moves=MOVES;
	score=0;
	start_time=0;
	end_time=TIME;
	timer=0;
	CANDY_TYPES=GPOINTER_TO_INT(data);
	if(CANDY_TYPES==1)
	{
		CANDY_TYPES=TIME_CANDY_TYPES;
		timer=1;
	}
	hscore_no();

	int i,j;
	board=make_candy_matrix(BOARD_SIZE,BOARD_SIZE);
	give_random_candy(board,BOARD_SIZE,BOARD_SIZE,CANDY_TYPES);
	while(horizontal_check(board,BOARD_SIZE,BOARD_SIZE,CANDY_TYPES)||vertical_check(board,BOARD_SIZE,BOARD_SIZE,CANDY_TYPES))
		continue;	//Makes sure that the matrix has been removed of any combinations

	gchar* b;
	gpointer m;
	int coord;
	for(i=0;i<BOARD_SIZE;i++)
	{
		for(j=0;j<BOARD_SIZE;j++)
		{
			coord=10*i+j;
			m=GINT_TO_POINTER(coord);
			wid[i][j]=gtk_button_new();
			put_candy_image(i,j);
			gtk_widget_set_size_request(wid[i][j],40,40);
			g_signal_connect(wid[i][j],"clicked",G_CALLBACK(swap_click),m);			//connecting all the buttons with signals by
			gtk_table_attach_defaults(GTK_TABLE(table),wid[i][j],j+1,j+2,i+1,i+2);	//defining the arguments as the co-ordinates
			gtk_widget_show(wid[i][j]);
		}
	}
	GtkWidget* button;

	button=gtk_button_new_with_label("Main Menu");
	g_signal_connect_swapped (button, "clicked",G_CALLBACK (main_menu_callback),window);
	gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,9,10);	
	gtk_widget_show(button);

	b=g_strdup_printf("Score\n%i",score);
	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,3,4);
	gtk_widget_show(button);

	b=g_strdup_printf("High Score\n\t%i",hscore);
	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,1,2);
	gtk_widget_show(button);

	if(timer!=0)		//Basing on the timer level or moves level the button is created
	{
		b=g_strdup_printf("Time\n%i",TIME);
		button=gtk_button_new_with_label(b);
		gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,5,6);
		gtk_widget_show(button);
	}
	else
	{
		b=g_strdup_printf("Moves\n%i",moves);
		button=gtk_button_new_with_label(b);
		gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,5,6);
		gtk_widget_show(button);
	}

	button=gtk_button_new_with_label("New Game");
	gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,7,8);
	g_signal_connect(button,"clicked",G_CALLBACK(level_callback),NULL);
	gtk_widget_show(button);

	gtk_widget_show_all(window);
	gtk_main();
	return;
}

//creates the window with updated high scores from the files
void hscore_callback(GtkWidget* widget,gpointer data)
{
	gtk_widget_destroy(table);
	table=gtk_table_new(7,8,TRUE);
	gtk_container_add(GTK_CONTAINER(window),table);
	
	int highscore;
	gchar* b;
	GtkWidget* button;
	
	FILE *easy,*medium,*hard,*tim;

	easy=fopen("easy.o","rb");
	medium=fopen("medium.o","rb");
	hard=fopen("hard.o","rb");
	tim=fopen("timer.o","rb");
	
	size_t len;
	char* line;
	len=0;
	getline(&line,&len,easy);
	highscore =atoi(line);

	b=g_strdup_printf("High Score In Easy Level Is %i",highscore);
	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,7,1,2);
	gtk_widget_show(button);

	len=0;
	getline(&line,&len,medium);
	highscore =atoi(line);

	b=g_strdup_printf("High Score In Medium Level Is %i",highscore);
	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,7,2,3);
	gtk_widget_show(button);

	len=0;
	getline(&line,&len,hard);
	highscore =atoi(line);

	b=g_strdup_printf("High Score In Hard Level Is %i",highscore);
	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,7,3,4);
	gtk_widget_show(button);

	len=0;
	getline(&line,&len,tim);
	highscore =atoi(line);

	b=g_strdup_printf("High Score In Timer Level Is %i",highscore);
	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,7,4,5);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("Main Menu");
	gtk_table_attach_defaults(GTK_TABLE(table),button,3,5,5,6);
	g_signal_connect_swapped(button,"clicked",G_CALLBACK(main_menu_callback),NULL);
	gtk_widget_show(button);
	gtk_widget_show_all(window);
	gtk_main();
	
	return;
}

//Checks the corresponding level of the game and gets the high score from the corresponding file and gives to local variable
void hscore_no()
{
	FILE* fp;
	if(timer==1)
	{
		fp=fopen("timer.o","rb");
	}
	else
	{
		switch(CANDY_TYPES)
		{
			case(4):
				fp=fopen("easy.o","rb");
				break;
			case(5):
				fp=fopen("medium.o","rb");
				break;
			case(6):
				fp=fopen("hard.o","rb");
				break;
		}
	}
	size_t len;
	len=0;
	char* line;
	getline(&line,&len,fp);
	hscore =atoi(line);

	return;
}

//calls the main menu wherever the Main Menu button is clicked
void main_menu_callback(GtkWidget* widget,gpointer data)
{
	GtkWidget* button,*candy_crush;
	gtk_widget_destroy(table);
	
	table=gtk_table_new(8,9,TRUE);
	gtk_container_add(GTK_CONTAINER(window),table);

	g_signal_connect(window, "delete-event", G_CALLBACK(quit_callback), NULL);
	

	button=gtk_button_new();
	candy_crush=gtk_image_new_from_file("title.png");
	gtk_button_set_image(GTK_BUTTON(button),candy_crush);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,8,1,3);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("Play");
	g_signal_connect(button,"clicked",G_CALLBACK(level_callback),NULL);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,4,4,5);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("High Score");
	gtk_table_attach_defaults(GTK_TABLE(table),button,5,8,4,5);
	g_signal_connect(button,"clicked",G_CALLBACK(hscore_callback),NULL);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("Quit");
	gtk_table_attach_defaults(GTK_TABLE(table),button,5,8,6,7);
	g_signal_connect(button,"clicked",G_CALLBACK(quit_callback),NULL);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("Credits");
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,4,6,7);
	g_signal_connect(button,"clicked",G_CALLBACK(credits),NULL);
	gtk_widget_show(button);

	gtk_widget_show_all(window);

	gtk_main();
	return;

}

//quits and destroys the window
void quit_callback(GtkWidget* widget,gpointer data)
{
	gtk_widget_destroy(window);
	gtk_main_quit();
	return;
}

//it detects that the candies are being clicked and takes the values of positions and calls the swap_candy function
void swap_click(GtkWidget* widget,gpointer data)
{
	int coord=GPOINTER_TO_INT(data);
	int vc,hc,loop_start;
	vc=0;
	hc=0;
	loop_start=1;
	click_no++;

	//if it is timer level then it will not go to this block
	if(moves==0&&timer!=2)
	{
		score_display();
		return;
	}
	
	//if it is timer level it goes into this
	if(timer==2)
	{
		gettimeofday(&end,&tzp);
		end_time=end.tv_sec;
		if(TIME-end_time+start_time<=0)
		{
			score_display();
			return;
		}
	}	
	
	if(click_no==1)
	{
		if(timer==1)		//this happens only once at the starting as after the timer will be defined as 2
		{	
			gettimeofday(&start,&tzp);
			start_time=start.tv_sec;	//starts the timer after the first click
			timer=2;
		}
		coordinates[0]=coord/10;
		coordinates[1]=coord%10;		//takes the coordinates for sending to swap function
	}
	else if(click_no==2)
	{
		coordinates[2]=coord/10;
		coordinates[3]=coord%10;		//takes the coordinates for sending to swap function
		score+=swap_candy(board,coordinates[0],coordinates[1],coordinates[2],coordinates[3],CANDY_TYPES);
		while(1)
		{
			hc=horizontal_check(board,BOARD_SIZE,BOARD_SIZE,CANDY_TYPES);
			vc=vertical_check(board,BOARD_SIZE,BOARD_SIZE,CANDY_TYPES);
			score+=vc+hc;
			if(!(vc||hc)&&loop_start==1)
			{
				swap_candy(board,coordinates[0],coordinates[1],coordinates[2],coordinates[3],CANDY_TYPES);

				GtkWidget* window1,*button1;
				GdkColor button1_colour;

				window1=gtk_window_new(GTK_WINDOW_TOPLEVEL);
				gtk_window_set_title(GTK_WINDOW(window1),"Invalid");
				gtk_window_set_default_size(GTK_WINDOW(window1),100,100);
				//gtk_window_set_resizable(GTK_WINDOW(window1),TRUE);

				button1=gtk_button_new_with_label("Sorry Its An\n\nInvalid Swap");
				gdk_color_parse("green",&button1_colour);
				gtk_widget_modify_bg(GTK_WIDGET(button1),GTK_STATE_NORMAL,&button1_colour);
				g_signal_connect_swapped(button1,"clicked",G_CALLBACK(gtk_widget_destroy),window1);

				gtk_container_add(GTK_CONTAINER(window1),button1);
				gtk_widget_show_all(window1);

				moves++;

				break;
			}
			else if(!(vc||hc))
			{
				break;
			}
			loop_start=0;
			vc=0;
			hc=0;
		}
		click_no=0;
		moves--;
		//GtkWidget* unwanted;
		print_gui_board(NULL,NULL);
	}
	return;
}

//It prints gui board after every swapping
void print_gui_board(GtkWidget* widget,gpointer data)
{
	gtk_widget_destroy(table);
	table=gtk_table_new(11,14,TRUE);
	gtk_container_add(GTK_CONTAINER(window),table);

	gchar* b;
	gpointer m;
	int coord,i,j;
	for(i=0;i<BOARD_SIZE;i++)
	{
		for(j=0;j<BOARD_SIZE;j++)
		{
			coord=10*i+j;
			m=GINT_TO_POINTER(coord);
			wid[i][j]=gtk_button_new();
			put_candy_image(i,j);
			gtk_widget_set_size_request(wid[i][j],40,40);
			g_signal_connect(wid[i][j],"clicked",G_CALLBACK(swap_click),m);
			gtk_table_attach_defaults(GTK_TABLE(table),wid[i][j],j+1,j+2,i+1,i+2);
			gtk_widget_show(wid[i][j]);
		}
	}
	GtkWidget* button;

	button=gtk_button_new_with_label("Main Menu");
	g_signal_connect_swapped (button, "clicked",G_CALLBACK (main_menu_callback),window);
	gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,9,10);	
	gtk_widget_show(button);

	b=g_strdup_printf("Score\n%i",score);
	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,3,4);
	gtk_widget_show(button);

	b=g_strdup_printf("High Score\n\t%i",hscore);
	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,1,2);
	gtk_widget_show(button);
	
	if(timer!=0)
	{	
		gettimeofday(&end,&tzp);
		end_time=end.tv_sec;
		b=g_strdup_printf("Time\n%i",TIME-end_time+start_time);
		button=gtk_button_new_with_label(b);
		gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,5,6);
		gtk_widget_show(button);
	}
	else
	{
		b=g_strdup_printf("Moves\n%i",moves);
		button=gtk_button_new_with_label(b);
		gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,5,6);
		gtk_widget_show(button);
	}

	button=gtk_button_new_with_label("New Game");
	gtk_table_attach_defaults(GTK_TABLE(table),button,11,13,7,8);
	g_signal_connect(button,"clicked",G_CALLBACK(level_callback),NULL);
	gtk_widget_show(button);

	gtk_widget_show_all(window);
	gtk_main();
	return;
}

//Basing on the values in the candy struct it decides the image to be put in place
void put_candy_image(int r,int c)
{
	int candi,power;
	GtkWidget* image;
	candi=(*(*(board+c)+r)).candy_type;
	power=(*(*(board+c)+r)).power_type;
	switch(candi)
	{
		case(0):
			image=gtk_image_new_from_file("z_bomb.png");
			gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
			break;
		case(1):
			switch(power)
			{
				case(0):
					image=gtk_image_new_from_file("b_normal.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(1):
					image=gtk_image_new_from_file("b_h.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(2):
					image=gtk_image_new_from_file("b_v.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
			}
			break;
		case(2):
			switch(power)
			{
				case(0):
					image=gtk_image_new_from_file("r_normal.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(1):
					image=gtk_image_new_from_file("r_h.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(2):
					image=gtk_image_new_from_file("r_v.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
			}
			break;
		case(3):
			switch(power)
			{
				case(0):
					image=gtk_image_new_from_file("y_normal.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(1):
					image=gtk_image_new_from_file("y_h.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(2):
					image=gtk_image_new_from_file("y_v.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
			}
			break;
		case(4):
			switch(power)
			{
				case(0):
					image=gtk_image_new_from_file("o_normal.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(1):
					image=gtk_image_new_from_file("o_h.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(2):
					image=gtk_image_new_from_file("o_v.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
			}
			break;
		case(5):
			switch(power)
			{
				case(0):
					image=gtk_image_new_from_file("g_normal.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(1):
					image=gtk_image_new_from_file("g_h.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(2):
					image=gtk_image_new_from_file("g_v.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
			}
			break;
		case(6):
			switch(power)
			{
				case(0):
					image=gtk_image_new_from_file("p_normal.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(1):
					image=gtk_image_new_from_file("p_h.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
				case(2):
					image=gtk_image_new_from_file("p_v.png");
					gtk_button_set_image(GTK_BUTTON(wid[r][c]),image);
					break;
			}
			break;
	}
}

//Displays the score after the game is completed
void score_display()
{
	gtk_widget_destroy(table);
	table=gtk_table_new(7,7,TRUE);
	gtk_container_add(GTK_CONTAINER(window),table);

	gchar* b;
	GtkWidget* button;

	b=g_strdup_printf("Yours Score is\n\t%i",score);
	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,6,1,2);
	gtk_widget_show(button);

	free_matrix(BOARD_SIZE,board);

	if(timer==2)
	{
		if(hscore<score)
		{	
			b=g_strdup_printf("Wow Great You Have Beaten The High Score");
			FILE* fp=fopen("timer.o","wb");
			fprintf(fp,"%d\n",score);
			fflush(fp);
			fclose(fp);
		}
		else
		{
			b=g_strdup_printf("High Score In This Level Is\n\t\t%i",hscore);
		}
	}
	
	switch(CANDY_TYPES)
	{
		case(4):
			if(hscore<score)
			{
				b=g_strdup_printf("Wow Great You Have Beaten The High Score");
				FILE* fp=fopen("easy.o","wb");
				fprintf(fp,"%d\n",score);
				fflush(fp);
				fclose(fp);	
			}
			else
			{
				b=g_strdup_printf("High Score In This Level Is\n\t\t%i",hscore);
			}
			break;
		case(5):
			if(hscore<score&&timer!=2)
			{
				b=g_strdup_printf("Wow Great You Have Beaten The High Score");
				FILE* fp=fopen("medium.o","wb");
				fprintf(fp,"%d\n",score);
				fflush(fp);
				fclose(fp);	
			}
			else if(timer!=2)
			{
				b=g_strdup_printf("High Score In This Level Is\n\t\t%i",hscore);
			}
			break;
		case(6):
			if(hscore<score)
			{
				b=g_strdup_printf("Wow Great You Have Beaten The High Score");
				FILE* fp=fopen("hard.o","wb");
				fprintf(fp,"%d\n",score);
				fflush(fp);
				fclose(fp);	
			}
			else
			{
				b=g_strdup_printf("High Score In This Level Is\n\t\t%i",hscore);
			}
			break;
	}

	button=gtk_button_new_with_label(b);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,6,2,3);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("Play On");
	gtk_table_attach_defaults(GTK_TABLE(table),button,3,4,4,5);
	g_signal_connect(button,"clicked",G_CALLBACK(level_callback),NULL);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("Quit");
	gtk_table_attach_defaults(GTK_TABLE(table),button,3,4,5,6);
	g_signal_connect(button,"clicked",G_CALLBACK(quit_callback),NULL);
	gtk_widget_show(button);

	gtk_widget_show_all(window);
	gtk_main();
	return;
}

//This is just for myself
void credits()
{
	GtkWidget* button,*chaitanya;
	gtk_widget_destroy(table);
	
	table=gtk_table_new(10,11,TRUE);
	gtk_container_add(GTK_CONTAINER(window),table);

	g_signal_connect(window, "delete-event", G_CALLBACK(quit_callback), NULL);

	button=gtk_button_new_with_label("Project: Candy Crush");
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,7,1,2);
	gtk_widget_show(button);
	
	button=gtk_button_new_with_label("Developer: Nalam V S S Krishna Chaitanya");
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,7,2,3);
	gtk_widget_show(button);
	
	button=gtk_button_new_with_label("Inspiration: Computer Programming Lab Team");
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,7,3,4);
	gtk_widget_show(button);
	
	button=gtk_button_new_with_label("Dedication: To My Parents");
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,10,4,5);
	gtk_widget_show(button);
	
	button=gtk_button_new_with_label("Special Thanks To: Jeevan Jala and Deepak Padamatha");
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,10,5,6);
	gtk_widget_show(button);
	
	button=gtk_button_new_with_label("Thank You");
	gtk_table_attach_defaults(GTK_TABLE(table),button,3,8,6,7);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("Main Menu");
	gtk_table_attach_defaults(GTK_TABLE(table),button,4,7,8,9);
	g_signal_connect(button,"clicked",G_CALLBACK(main_menu_callback),NULL);
	gtk_widget_show(button);

	button=gtk_button_new();
	chaitanya=gtk_image_new_from_file("chaitanya.png");
	gtk_button_set_image(GTK_BUTTON(button),chaitanya);
	gtk_table_attach_defaults(GTK_TABLE(table),button,7,10,1,4);
	gtk_widget_show(button);


	gtk_widget_show_all(window);
	gtk_main();
	return;
}

//This opens a window for the user to select the level and gives arguments accordingly
void level_callback()
{
	GtkWidget* button;//*candy_crush;
	gtk_widget_destroy(table);
	
	table=gtk_table_new(7,5,TRUE);
	gtk_container_add(GTK_CONTAINER(window),table);

	g_signal_connect(window, "delete-event", G_CALLBACK(quit_callback), NULL);
	
	int level;
	gpointer* m;

	button=gtk_button_new_with_label("Easy");
	level=4;
	m=GINT_TO_POINTER(level);
	g_signal_connect(button,"clicked",G_CALLBACK(play_callback),m);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,4,1,2);
	gtk_widget_show(button);
	
	button=gtk_button_new_with_label("Medium");
	level=5;
	m=GINT_TO_POINTER(level);
	g_signal_connect(button,"clicked",G_CALLBACK(play_callback),m);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,4,2,3);
	gtk_widget_show(button);
	
	button=gtk_button_new_with_label("Hard");
	level=6;
	m=GINT_TO_POINTER(level);
	g_signal_connect(button,"clicked",G_CALLBACK(play_callback),m);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,4,3,4);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("Time");
	level=1;
	m=GINT_TO_POINTER(level);
	g_signal_connect(button,"clicked",G_CALLBACK(play_callback),m);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,4,4,5);
	gtk_widget_show(button);

	button=gtk_button_new_with_label("Main Menu");
	gtk_table_attach_defaults(GTK_TABLE(table),button,2,3,5,6);
	g_signal_connect(button,"clicked",G_CALLBACK(main_menu_callback),NULL);
	gtk_widget_show(button);

	
	gtk_widget_show_all(window);

	gtk_main();
	return;

}

//Frees the matrix after the score is displayed
void free_matrix(int N,candy** A)
{
	int i;
	for(i=0;i<N;i++)
	{
		free(*(A+i));
	}
	free(A);
	return;
}