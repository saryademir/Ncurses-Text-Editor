#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>

#define MAXLINE 25	
#define MAXCHAR 40

//row 0 reserved for menu

char strbuf[MAXCHAR];		// char buffer

struct node {
	char statement[MAXCHAR];		
	int next; 						
};

struct node textbuffer[MAXLINE]; 	

FILE *inFilePtr;	// file pointer
char *filename;		
char stat[40];		// same as strbuf, defined above
int c_cursor;		// holds cursor ROW position on screen (0 -- MAXLINE), 0 is for menu line
int f_lines;		// number of lines read from file or created on screen [0 - MAXLINE]

int free_head; 	// head node that points to the first free line in textbuffer[]
int inuse_head; // head node that points to the first used line in textbuffer[]

void edit(char* );
void insert(char*);
void delete(void);
void save(void);

void shiftLeft(struct node *, int);	// used for deletind an array element
void shiftRight(struct node *,int);	// used for inserting an array element
void display_buf(struct node *);	
void renum(struct node *);			// renumbers array NEXT elements starting from 1 to n
void disp_menu(void);		// prints menu in reverse color

void edit(char* filename){
	
	if ( ( inFilePtr = fopen( filename , "r" ) ) != NULL ) {

		int line=0;
		while ( fgets(strbuf, MAXCHAR, inFilePtr) != NULL ){
			
			if (line+1 > MAXLINE){ // if file larger than buffer terminate program!
				fclose(inFilePtr);
				exit (0);
			}else{
				strcpy(textbuffer[line].statement,strbuf); //copy strbuf to textbuffer
				textbuffer[line].next = line+1;
				line++;
			}
		}

		f_lines=line; //total lines 
		int temp;
		for(int i=0; i<MAXLINE;i++)
		{
			if(textbuffer[i].next!='\n') 
			{
				temp=textbuffer[i].next;
				break;
			}
		}
		if(temp-1=='\n')
		{
			inuse_head=temp;
		}
		else
		{
			inuse_head=temp-1;
		}
		for(int i=0; i<MAXLINE;i++)
		{
			if(textbuffer[i].next=='\n')
			{
				free_head=textbuffer[i].next;
				break;
			}
		}

		textbuffer[line-1].next = -1;	// mark TAIL on textbuffer
		
		fclose(inFilePtr);

	}else { // input file could not be opened
		inFilePtr = fopen (filename,"w");	//creates file
		fclose(inFilePtr);
	}
	display_buf(textbuffer);
}

void insert(char* stat){

	int i=0;
	int ch;
	char tmp[40];

	f_lines++;	
	c_cursor++;	// increase cursor position (1 line down)
	shiftRight(textbuffer,c_cursor); //shifts elements of the array 1 times starting from the c_cursor
	renum(textbuffer);

	display_buf(textbuffer);

	while ((ch = getch()) != '\n'){	// then get entry 
		tmp[i]=ch;
		i++;
	}

	tmp[i]='\n';
	tmp[i+1]='\0';
	refresh();
	strcpy((textbuffer+c_cursor)->statement,tmp);
	
	for(int i=0;i< f_lines;i++)
	{
		textbuffer[i].next=textbuffer[i+1].next;
		
		if(f_lines==c_cursor){ //if we reached the end of lines
		(textbuffer+c_cursor)->next=-1;
	}else{
		(textbuffer+c_cursor)->next=c_cursor+1; 
	}
	}

	renum(textbuffer);
	
	disp_menu();
	display_buf(textbuffer);
	return;

}
void delete(void){

	shiftLeft(textbuffer,c_cursor+1);
	
	
	f_lines--;	// deccrease #of lines in buffer
	renum(textbuffer);
	textbuffer[f_lines-1].next=-1;
	
	clear();					
	disp_menu();				
	display_buf(textbuffer);	
}


void save(void){

  	if ( ( inFilePtr = fopen( filename , "w" ) ) != NULL ){
		for (int i=0;i<f_lines+1;i++){
			fprintf(inFilePtr,"%s",textbuffer[i].statement); // '\n comes from buffer'
		}
		
		refresh();
  	}else{

		refresh();
  	}

	fclose(inFilePtr);

}

void shiftLeft(struct node * array, int cursor){
	/*
	 * Shifts array elements Left starting from cursor by 1
	 *
	 * 		Leftmost element overwritten - Delete arr(cursor-1)
	 *
	 * 		Cursor : 1 TO MAXLINE
	 *
	 *   	 0 1 2 3 4 5 6   MAXLINE=7
	 * ary=	{a,b,c,d,e,f,g}
	 *
	 *    shiftLeft(ary,2)
	 *
	 *    	 0 1 2 3 4 5 6   MAXLINE=7
	 * ary=	{a,C,d,e,f,g,-1}
	 *
	 * 		ary(cursor-1) deleted/overwritten by ary(cursor)
	 * 		ary(MAXLINE)=-1, denotes empty element
	 *
	 */
	    (array+f_lines)->next=-1; //inserts -1 to the end of the array
	    for (int i=cursor;i<MAXLINE;i++){
			*(array+i-1)=*(array+i);
		}

	    for (int i=f_lines-1;i<MAXLINE;i++){
	    			(array+i)->next=-1;
	    }

}

void shiftRight(struct node * array,int cursor){
	/*
	 * Shifts array elements Right starting from cursor by 1
	 *
	 * 		Rightmost element overwritten !
	 *
	 * 		Cursor : 0 TO MAXLINE-1
	 *
	 *   	 0 1 2 3 4 5 6   MAXLINE=7
	 * ary=	{a,b,c,d,e,f,g}
	 *
	 *    shitdRight(ary,2)
	 *
	 *    	 0 1  2 3 4 5 6   MAXLINE=7
	 * ary=	{a,b,-1,c,d,e,f}
	 *
	 */
	
	if (cursor<MAXLINE){
	    for (int i=MAXLINE;i>=cursor;i--){
			*(array+i)=*(array+i-1);
		}
	}
	strcpy((array+cursor)->statement,"\n");
	(array+cursor)->next=-1;


}

void display_buf(struct node * array){
	clear();
	disp_menu();
	refresh();
	move(1,0);
	for (int i=inuse_head;i<f_lines;i++){
	
		printw("%s",textbuffer[i].statement);// '\n' comes from the file

	}
	move(c_cursor+1,0);
	refresh();
}
void renum(struct node * array){
	for (int i=0;i<f_lines;i++){
		(array+i)->next=i+1;
	}
	(array+f_lines)->next=-1;
	(array+f_lines+1)->next=-1;
	
}

void disp_menu(void){
		int t_row,t_col;
		getyx(stdscr,t_row,t_col);
		
		attron(A_REVERSE);	// display menu items in reverse color
		mvprintw(0,0,"(W/w)-cursor up (Z/z)-cursor down (I/i)-Insert (D/d)-Delete (S/s)-Save (X/x)-Exit\n");	//print menu in reverse color
		attroff(A_REVERSE);	// turn off reverse color

		move(t_row,t_col);	//restore current cursor position
		refresh();
}

int main(int argc, char **argv)
{
	c_cursor=0;	
	f_lines=0;		

	initscr();		// initialize Ncurses screen

    if ( argc != 2) {
	   
	   	refresh();
	   	
	   	return 0;
   	}else {
   		filename =  argv[1];

   	}

	edit(filename);

	while(1){
		int ch;
		noecho();					// turn echo OFF
		while ((ch = getch()) != EOF){
			switch (ch)	{
				case 'w':			// cursor UP
				case 'W':
					c_cursor--;
					if (c_cursor <0)
						c_cursor=0;
					move(c_cursor+1,0);	// first line is for menu, entries begins from line 1
					
					break;
				case 'z':			// cursor DOWN
				case 'Z':
					c_cursor++;
					if (c_cursor > f_lines-1)
							c_cursor=f_lines-1;
					move(c_cursor+1,0); // first line is for menu, entries begins from line 1
				
					break;
				case 'I':
				case 'i':// INSERT
					echo();			// turn echo ON
					insert(stat);
					noecho();		// turn echo OFF again
				
					break;
				case 'D':
				case 'd':
					delete();
		
					break;
				case 'S':
				case 's':
					save();
					break;
	
				case 'X':
				case 'x':
					echo();			// turn echo ON
					endwin();	
					return (0);
				default:
					break;
			}

		}
		
    }
}
