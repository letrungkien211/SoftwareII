/*
  Project: Where are black masses?
  Description: Refer to Iba sensei's book, 157th page
  Date: 11/12/29
  Author: Le Trung Kien
  Student number: g041172
*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define SIZE 9    // table's size
#define GRAY 0    // unjustified
#define WHITE -2   // white fixed
#define BLACK -3   // black fixed

// GRAY=0 is convenient because when an array is declared,
// initial values of its member are all 0

/*** Variables' and structures' declaration ***/

int problem[SIZE][SIZE];  // Problem
int n_ans;  // number of answers
int blank_table[SIZE][SIZE];  // a blank table (all elements are 0)

/* Position*/
struct position{
  int row;
  int col;
};

/* Position with value*/
struct problem_number_position{
  int value;
  struct position pos;
} pro_num_pos[SIZE*SIZE];
/* This array hold the value and correspondent position
   of problem table*/

int number_of_initial_values=0; // Number of masses filled with numbers

/* Number of white mass around on the left,right,up,down*/
struct around{
  int left;
  int right;
  int up;
  int down;
};

/* STACK*/
struct data{
  int table[SIZE][SIZE];
  int step;
};

struct stack{
  int table[SIZE][SIZE];
  int step;
  struct stack *next;
};

/* Stack functions*/
void init_stack(struct stack **top);
struct data pop(struct stack **top);
void push(struct stack **top, int table[][SIZE],int step);
void print_stack(struct stack *top);

/* Input-output functions*/
void copy_table(int to[][SIZE],int from[][SIZE]);
void input(FILE* fp);
void print_table(int table[][SIZE]);

/* Initilization functions*/
void create_initial_path_list(struct stack **initial_path_list);
void init_pro_num(); // Initialize pro_num_pos array from problem table

/* Check functions*/
int check(int table[][SIZE], struct position current_position, struct around aro);
int around_is_black(int table[][SIZE],int row,int col);
int to_black(int table[][SIZE],int row,int col);
int to_white(int table[][SIZE],int row,int col);

/* Solution's finding functions*/
void update_table(int table[][SIZE],struct position current_position,struct around aro);
struct stack* extend(struct stack **path_list,int table[][SIZE],int step);
int search_one_solution(struct stack *path_list);
void search_all_solution(struct stack *path_list);

/*******************************MAIN***************************/
int main(){
  FILE *fp;
  struct stack *initial_path_list;
  fp=fopen("q16.txt","r");   // read the text file
  input(fp);        // store into problem[SIZE][SIZE]
  fclose(fp);
  init_pro_num();   // create pro_num_pos[] array

  init_stack(&initial_path_list);    // initilize stack
  create_initial_path_list(&initial_path_list);   // create the initial path list
  n_ans=0;    // initialize number of solutions
  // Search for all solutions
  search_all_solution(initial_path_list);
  if(!n_ans){
    printf("There is no solution\n");
    return 0;
  }
  printf("There are %d solution(s)\n",n_ans);
  return 0;
}
/****************************END MAIN*************************/

/* Search for one solution
   return 1 if a solution is found, otherwise, return 0*/
int search_one_solution(struct stack *path_list){
  struct data tmp;
  if(!path_list) return 0;
  
  tmp=pop(&path_list);
  // when the last mass with value is checked
  if(tmp.step==number_of_initial_values){
    printf("This is a solution\n");
    print_table(tmp.table);
    return 1;
  }
  // recursion
  return search_one_solution(extend(&path_list,tmp.table,tmp.step));
}

void search_all_solution(struct stack *path_list){
  struct data tmp;
  if(!path_list) return;
  tmp=pop(&path_list);
  // when the last mass with value is checked
  if(tmp.step==number_of_initial_values){
    printf("#%d solution\n",++n_ans);
    print_table(tmp.table);
  }
  // recursion
  search_all_solution(extend(&path_list,tmp.table,tmp.step));
}

/* Extend path_list, depth-first*/
struct stack* extend(struct stack **path_list,int table[][SIZE],int step){
  int tmp_table[SIZE][SIZE];
  struct around aro;
  int value=pro_num_pos[step].value;
  struct position current_position=pro_num_pos[step].pos;
  for(aro.left=0; aro.left<value; ++aro.left){
    for(aro.right=0; aro.right<value-aro.left; ++aro.right){
      for(aro.up=0; aro.up<value-aro.left-aro.right; ++aro.up){
	// assure the number of white mass is equal to value
	aro.down=value-1-aro.left-aro.right-aro.up;
	if(check(table,current_position,aro)){
	  copy_table(tmp_table,table);
	  update_table(tmp_table,current_position,aro);
	  push(path_list,tmp_table,step+1);
	}
      }
    }
  }
  return *path_list;
}

void update_table(int table[][SIZE],struct position current_position,struct around aro){
  int row,col,left,right,up,down,i;
  
  row=current_position.row;
  col=current_position.col;
  
  left=col-aro.left;
  right=col+aro.right;
  up=row-aro.up;
  down=row+aro.down;
  
  for(i=left; i<=right; ++i)
    table[row][i]=WHITE;
  for(i=up; i<=down; ++i)
    table[i][col]=WHITE;
  if(left>0) table[row][left-1]=BLACK;
  if(right<SIZE-1) table[row][right+1]=BLACK;
  if(up>0) table[up-1][col]=BLACK;
  if(down<SIZE-1) table[down+1][col]=BLACK;
}

/*****Check functions*****/
/* Check if a new path is valid or not*/
int check(int table[SIZE][SIZE],struct position current_position, struct around aro){
  int row=current_position.row;
  int col=current_position.col;
  int i;
  /* check to black*/
  if(!to_black(table, row, col-1-aro.left))
    return 0;
  if(!to_black(table, row, col+1+aro.right))
    return 0;
  if(!to_black(table, row-1-aro.up, col))
    return 0;
  if(!to_black(table, row+1+aro.down, col))
    return 0;
  /* check to white*/
  for(i=-aro.left; i<=aro.right; ++i)
    if(!to_white(table,row,col+i))
      return 0;
  for(i=-aro.up; i<=aro.down; ++i)
    if(!to_white(table,row+i,col))
      return 0;
  return 1;
}



// check if there is any black mass around a mass
// return 1 if there is at least one, otherwise, return 0
int around_is_black(int table[][SIZE],int row,int col){
  if(row>0)
    if(table[row-1][col]==BLACK) return 1;
  if(row<SIZE-1)
    if(table[row+1][col]==BLACK) return 1;
  if(col>0)
    if(table[row][col-1]==BLACK) return 1;
  if(col<SIZE-1)
    if(table[row][col+1]==BLACK) return 1;
  return 0;
}

/* Check if a mass can be changed to black
   return 1 if possible, 0 if not*/
int to_black(int table[][SIZE],int row,int col){
  if(row<-1||col<-1||row>SIZE||col>SIZE)
    return 0;
  if(row==-1||col==-1||row==SIZE||col==SIZE)
    return 1;
  if(around_is_black(table,row,col)) return 0;
  if(table[row][col]==WHITE) return 0;
  if(problem[row][col]>0) return 0;
  return 1;
}

/* Check if a mass can be changed to white
   return 1 if possible, 0 if not*/
int to_white(int table[][SIZE],int row,int col){
  if(row<0||col<0||row>=SIZE||col>=SIZE)
    return 0;
  if(table[row][col]==BLACK) return 0;
  return 1;
}

/*******STACK functions*******/
// Initilize a stack
void init_stack(struct stack **top){
  *top=NULL;
}
// Push data into stack
void push(struct stack **top, int table[][SIZE],int step){
  struct stack *sp;
  sp=(struct stack *)malloc(sizeof(struct stack));
  copy_table(sp->table, table);
  sp->step=step;
  sp->next=*top;
  *top=sp;
}
// Pop data from stack
struct data pop(struct stack **top){
  struct data data;
  struct stack *sp;
  sp=*top;
  copy_table(data.table,(*top)->table);
  data.step=(*top)->step;
  *top=(*top)->next;
  free(sp);
  return data;
}

// Print all elements of a stack
void print_stack(struct stack *top){
  while(top){
    print_table(top->table);
    printf("%d\n",top->step);
    top=top->next;
  }
}

/*******IN-OUT functions********/
/*store the problem read from a file into table problem[SIZE][SIZE] */
void input(FILE *fp){
  int i,j;
  for(i=0; i<SIZE; ++i){
    for(j=0; j<SIZE; ++j){
      fscanf(fp,"%d ",&problem[i][j]);
    }
    fscanf(fp,"\n");
  }
}
/* print a table*/
void print_table(int table[][SIZE]){
  int i,j;
  for(i=0; i<SIZE; ++i){
    for(j=0; j<SIZE; ++j){
      if(table[i][j]==BLACK)
	printf("*  ");	
      else
	printf("%-3d",problem[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
    
/* Copy tables*/
void copy_table(int to[][SIZE],int from[][SIZE]){
  int i,j;
  for(i=0; i<SIZE; ++i)
    for(j=0; j<SIZE; ++j)
      to[i][j]=from[i][j];
}

/********Initilization group*******/
/* Create the initial path list for solution's finding*/
void create_initial_path_list(struct stack **initial_path_list){
  int tmp_table[SIZE][SIZE];
  struct around aro;
  int value=pro_num_pos[0].value;
  struct position current_position=pro_num_pos[0].pos;
  
  for(aro.left=0; aro.left<value; ++aro.left){
    for(aro.right=0; aro.right<value-aro.left; ++aro.right){
      for(aro.up=0; aro.up<value-aro.left-aro.right; ++aro.up){
	aro.down=value-1-aro.left-aro.right-aro.up;
	
	if(check(blank_table,current_position,aro)){
	  copy_table(tmp_table,blank_table);
	  update_table(tmp_table,current_position,aro);
	  push(initial_path_list,tmp_table,1);
	}
      }
    }
  }
}

/* Initilize pro_num_pos array*/
void init_pro_num(){
  int i,j;
  for(i=0; i<SIZE; ++i)
    for(j=0; j<SIZE; ++j)
      if(problem[i][j]>0){
	pro_num_pos[number_of_initial_values].value=problem[i][j];
	pro_num_pos[number_of_initial_values].pos.row=i;
	pro_num_pos[number_of_initial_values].pos.col=j;
	++number_of_initial_values;
      }
}
