/* Program to assist in the challenge of solving sudoku puzzles.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2021, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is donely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Kaveesh Gollaheennalage  1270280
   Dated:     02/09/21

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define NDIM 3		/* sudoku dimension, size of each inner square */

#define NDIG (NDIM*NDIM)
			/* total number of values in each row */
			
#define NGRP 3		/* number of sets each cell is a member of */
#define NSET (NGRP*NDIG)
			/* total number of sets in the sudoku */
#define NCLL (NDIG*NDIG)
			/* total number of cells in the sudoku */

#define ERROR	(-1)	/* error return value from some functions */

/* these global constant arrays provided as part of the initial skeleton,
   you may use them in your code but must not alter them in any way,
   regard them as being donely fixed. They describe the relationships
   between the cells in the sudoku and provide a basis for all of the
   sudoku processing loops */

/* there are 27 different different sets of elements that need to be
   checked against each other, this array converts set numbers to cells,
   that's why its called s2c */
int s2c[NSET][NDIM*NDIM] = {
	/* the first group of nine sets describe the sudoku's rows */
	{  0,  1,  2,  3,  4,  5,  6,  7,  8 },
	{  9, 10, 11, 12, 13, 14, 15, 16, 17 },
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26 },
	{ 27, 28, 29, 30, 31, 32, 33, 34, 35 },
	{ 36, 37, 38, 39, 40, 41, 42, 43, 44 },
	{ 45, 46, 47, 48, 49, 50, 51, 52, 53 },
	{ 54, 55, 56, 57, 58, 59, 60, 61, 62 },
	{ 63, 64, 65, 66, 67, 68, 69, 70, 71 },
	{ 72, 73, 74, 75, 76, 77, 78, 79, 80 },
	/* the second group of nine sets describes the sudoku's columns */
	{  0,  9, 18, 27, 36, 45, 54, 63, 72 },
	{  1, 10, 19, 28, 37, 46, 55, 64, 73 },
	{  2, 11, 20, 29, 38, 47, 56, 65, 74 },
	{  3, 12, 21, 30, 39, 48, 57, 66, 75 },
	{  4, 13, 22, 31, 40, 49, 58, 67, 76 },
	{  5, 14, 23, 32, 41, 50, 59, 68, 77 },
	{  6, 15, 24, 33, 42, 51, 60, 69, 78 },
	{  7, 16, 25, 34, 43, 52, 61, 70, 79 },
	{  8, 17, 26, 35, 44, 53, 62, 71, 80 },
	/* the last group of nine sets describes the inner squares */
	{  0,  1,  2,  9, 10, 11, 18, 19, 20 },
	{  3,  4,  5, 12, 13, 14, 21, 22, 23 },
	{  6,  7,  8, 15, 16, 17, 24, 25, 26 },
	{ 27, 28, 29, 36, 37, 38, 45, 46, 47 },
	{ 30, 31, 32, 39, 40, 41, 48, 49, 50 },
	{ 33, 34, 35, 42, 43, 44, 51, 52, 53 },
	{ 54, 55, 56, 63, 64, 65, 72, 73, 74 },
	{ 57, 58, 59, 66, 67, 68, 75, 76, 77 },
	{ 60, 61, 62, 69, 70, 71, 78, 79, 80 },
};


/* there are 81 cells in a dimension-3 sudoku, and each cell is a
   member of three sets, this array gets filled by the function 
   fill_c2s(), based on the defined contents of the array s2c[][] */
int c2s[NCLL][NGRP];

void
fill_c2s() {
	int s=0, d=0, c;
	for ( ; s<NSET; s++) {
		/* record the first set number each cell is part of */
		for (c=0; c<NDIM*NDIM; c++) {
			c2s[s2c[s][c]][d] = s;
		}
		if ((s+1)%(NGRP*NDIM) == 0) {
			d++;
		}
	}
#if 0
	/* this code available here if you want to see the array
	   cs2[][] that gets created, just change that 0 two lines back
	   to a 1 and recompile */
	for (c=0; c<NCLL; c++) {
		printf("cell %2d: sets ", c);
		for (s=0; s<NGRP; s++) {
			printf("%3d", c2s[c][s]);
		}
		printf("\n");
	}
	printf("\n");
#endif
	return;
}

/* find the row number a cell is in, counting from 1
*/
int
rownum(int c) {
	return 1 + (c/(NDIM*NDIM));
}

/* find the column number a cell is in, counting from 1
*/
int
colnum(int c) {
	return 1 + (c%(NDIM*NDIM));
}

/* find the minor square number a cell is in, counting from 1
*/
int
sqrnum(int c) {
	return 1 + 3*(c/NSET) + (c/NDIM)%NDIM;
}

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to done the
   Authorship Declaration 
*/
#define HORIZ_DIVIDER_CELL 27
#define GRID_LENGTH 21
#define CORNER1 6
#define CORNER2 14
#define VERT_LINE1 4
#define VERT_LINE2 7
#define MAX_ROW 9
#define MAX_COL 18
/****************************************************************/
void read_data(int sudoku[]);
void dispsudoku(int sudoku[], int done);
void dashline(void);
void gridscan(int sudoku[]);
void error_message(int nset_vio, int num_vio);
void strat1(int sudoku[]);
int setscan(int setnum, int sudoku[], int *num_vio);
int gridfill(int sudoku[]);
/****************************************************************/

/* main program controls all the action
*/
int
main(int argc, char *argv[]) {
	int sudoku[NCLL];
	fill_c2s();
	read_data(sudoku);
	dispsudoku(sudoku,0);
	gridscan(sudoku);
	strat1(sudoku);
	dispsudoku(sudoku,1);
	
	/* all done, so pack up bat and ball and head home */
	return 0;
}

/****************************************************************/

/*Reads in the sudoku grid*/
void
read_data(int sudoku[]) {
	int i;
	for (i=0;i<NCLL;i++) {
		scanf("%d",sudoku+i);
	}
	return;
}
/****************************************************************/

/*
Helper func. Creates a horizontal line using +- symbols, 
below rows 3 and 6
*/
void
dashline(void) {
	int i;
	
	for (i=0;i<GRID_LENGTH;i++) {
		/*Insert + to the right of columns 3 and 6*/ 
		if (i==CORNER1 || i==CORNER2) {
			printf("+");
		}else {
			printf("-");
        }
        
    }
    printf("\n");
    return;
}

/*****************************************************************/

/*Displays the current sudoku grid in the terminal window and reports
the number of unknown cells*/
void
dispsudoku(int sudoku[], int done) {
	int i,unknown=0;
	
	printf("\n");
	
	for (i=0;i<NCLL;i++) {
		  
		/*Creates a horizontal line, below rows 3 and 6*/
	    if (!(i%HORIZ_DIVIDER_CELL)&&i) {
	    	dashline();
	    /*Creates vertical bars to the right of columns 3 and 6*/	
	    }else if(colnum(i)==VERT_LINE1 || colnum(i)==VERT_LINE2) {
			printf("| ");
		}
		
		/*If the last element in each row is non-zero*/
		if (sudoku[i] && !((i+1)%NDIG)) {
			printf("%d\n",sudoku[i]);
		/*If the number is non-zero and not the last element of a row*/
		}else if (sudoku[i]) {
			printf("%d ",sudoku[i]);
		/*Unknown value in the sudoku grid*/	
		} else {
			unknown++;
			/*Last element in a row*/
			if (!((i+1)%NDIG)) {
				printf(".\n");
			}else { 
			    printf(". ");
			}
			
		}
		
	}
	
	printf("\n%2d cells are unknown\n",unknown);
	
	/*Print ta daa!!!, if the program completed the sudoku*/
	if (done&&!unknown) {
		printf("\nta daa!!!\n");
    }
    return;
}

/******************************************************************/

/*Helper func. Does a full set scan and makes sure there isn't any repeated
numbers*/
int
setscan(int setnum, int sudoku[], int *num_vio) {
	int i,cell,curr_value,freq[NDIG],flag=0;
	
	/*Create a array of nine zeros	*/
	for (i=0;i<NDIG;i++) {
	    freq[i] = 0;
    }
	
    
	for (i=0;i<NDIG;i++) {
		cell = s2c[setnum][i];
		curr_value = sudoku[cell];
		
		if (!curr_value) {
			continue;
		}
		    
	 	freq[curr_value-1] +=1;
    }
    
    for (i=0;i<NDIG;i++) {
    	    
  	    if (freq[i]-1>0) {
    	    	
	   	    *num_vio = *num_vio+1;
	   	    flag=1;
	   		
	    	if (setnum<MAX_ROW) {
		    	printf("\nset %2d (row %d): %d instances of %d",
		    		setnum,setnum+1,freq[i],i+1);
		    }else if (setnum<MAX_COL) {
	 		    printf("\nset %2d (col %d): %d instances of %d",
	 		    	setnum,setnum-MAX_ROW+1,freq[i],i+1);
	 	    }else {
			    printf("\nset %2d (sqr %d): %d instances of %d",
			    	setnum,setnum-MAX_COL+1,freq[i],i+1);
			}
		}
		    		
    }
        
    return flag;
}	
	
/****************************************************************/

/*Does a full sudoku grid scan to see if there are any */
void
gridscan(int sudoku[]) {
	int i,set_flag=0,violat_flag=0,num_vio=0,nset_vio=0;
	
	for (i=0;i<NSET;i++) { 
		set_flag = setscan(i,sudoku,&num_vio);
		
		/*Is there a violation in the set*/
		if (set_flag) {
			nset_vio++;
			violat_flag = 1;
		}
			
	}
	
	if (violat_flag) {
		error_message(nset_vio,num_vio);
	}
	
	return;
}

/*****************************************************************/

/*strat1 continues to use stratergy one in order to
solve the sudoku grid*/
void
strat1(int sudoku[]) {
	int success_flag=1;
	
	while (success_flag) {
		success_flag = gridfill(sudoku);
	}
    return;
}

/*****************************************************************/

/*Helper func. Gives the steps to solve a sudoku using stratergy one*/
int
gridfill(int sudoku[]) {
	int i,j,k;
	int poss_value,nposs_value=0,cell_value,cell_num,set_num;
	int success_flag = 0;
	int set_freq[NDIG], tmpsudoku[NCLL];
	
	
	/*Create a temporary sudoku grid*/
	for (i=0;i<NCLL;i++) {
		tmpsudoku[i] = 0;
	}
	
	/*Look into each cell*/
	for (i=0;i<NCLL;i++) {
		nposs_value = 0;
		poss_value = 0;
		
		if (sudoku[i]) {
			continue;
		}
		
		/*Create a array of zeros	*/
	    for (j=0;j<NDIG;j++) {
	        set_freq[j] = 0;
        }
		
        /*Go through each set that cell belongs to*/
        for (j=0;j<NGRP;j++){
            set_num = c2s[i][j];
           
            
            /*Look at neighbour cells*/
            for (k=0;k<NDIG;k++) {
                cell_num = s2c[set_num][k];
                cell_value = sudoku[cell_num];
        	
        	    /*Known cells in the set*/
        	    if (cell_value) {	
        		    set_freq[cell_value-1] += 1;
        	    }
            }
        }
        
        
        	
        /*Linear scan of frequency array*/
        for (j=0;j<NDIG;j++) {
        	
        	/*Number appears in one of the sets*/
        	if (set_freq[j]) {
        		continue;
        	}
        	
        	nposs_value++;
        	poss_value = j+1;
        }
       
        /*We known the value of that cell*/
        if (nposs_value==1) {
        	
        	if (!success_flag) {
        		printf("\nstrategy one\n");
        	}
            success_flag = 1;
        	tmpsudoku[i] = poss_value;        	
			printf("row %d col %d must be %d\n",rownum(i),
				colnum(i), poss_value);
        }
    }
    
    /*Copy data from the temporary sudoku grid and paste it into the
    soduku grid once the pass is complete*/
    for (i=0;i<NCLL;i++) {
    	if (!tmpsudoku[i]) {
    		continue;
    	}
    	sudoku[i] = tmpsudoku[i];
    }
    return success_flag;
}
        	
/*****************************************************************/\
/*Prints out error messages if there is a violation*/
void 
error_message(int nset_vio, int num_vio) {
	
	printf("\n");
    printf("\n%d different sets have violations\n",nset_vio);
    printf("%d violations in total\n",num_vio);
    exit(EXIT_FAILURE);
	
	return;
}

/*algorithms are fun!*/
