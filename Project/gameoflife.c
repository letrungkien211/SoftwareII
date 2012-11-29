// The game of life.

#include<stdio.h>
#include<time.h>
#define ROW 50
#define COL 50

#define MAX 1000
int a[ROW+2][COL+2],x[ROW+2][COL+2];       // Two matrixes represents next and current states.


// Inititilization
void init(){
    int i,j;
    char line[100];
    for (i=0;i<=ROW+1;++i)
	for(j=0;j<=COL+1;++j)
	    a[i][j]=0;

    printf("%dx_5d board is given. Choose initial positions (row col):\nInput -1 to start.\n",ROW,COL);
    while (1){
	fgets(line,sizeof(line),stdin);
	sscanf(line,"%d%d",&i,&j);
	if(i==-1)
	    break;
	if(i<1 ||i>ROW || j<1 || j>COL)
	    printf("Wrong input!\n");
	else
	    a[i][j]=1;
    }
}

/* Random initialization.*/
// Swap two int type variables.
void swap(int *x,int *y){
    int temp=*x;
    *x=*y;
    *y=temp;
}

void rand_init(){
    int i,j,a1,b1,c1,d1,temp;
    a1=rand()%ROW+1;              // make a random assignment in a rectangle (a1,c1)(a1,d1)(b1,c1)(b1,d1)
    b1=rand()%ROW+1;              // a1,b1,c1,d1 are themself random numbers.
    c1=rand()%COL+1;
    d1=rand()%COL+1;
    if(a1>b1)                     // swap to make sure the rectangle is valid.
	swap(&a1,&b1);
    if(c1>d1)
	swap(&c1,&d1);
    for(i=a1;i<=b1;++i)
	for(j=c1;j<=d1;++j)
	    a[i][j]=rand()%2;
}

// Make a copy of current state.
void copy(){
    int i,j;
    for(i=1;i<=ROW;++i)
	for(j=1;j<=COL;++j)
	    x[i][j]=a[i][j];
}

// Check the number of neighbours of a cell being in life.
int neighbour(int m,int n){
    int sum;
    int i,j;
    sum=-x[m][n];
    for(i=m-1;i<=m+1;++i)
	for(j=n-1;j<=n+1;++j)
	    sum+=x[i][j];
    return sum;
}  

// Check all the cells.
void check_all(){
    int m,n;
    copy();
    for(m=1;m<=ROW;++m){
	for(n=1;n<=COL;++n){
	    if(x[m][n]==1&&(neighbour(m,n)<=1||neighbour(m,n)>=4))
		a[m][n]=0;
	    if(x[m][n]==0&&neighbour(m,n)==3)
		a[m][n]=1;
	}
    }
}

// Print the board.
void print(){
    int i,j;
    for(i=1;i<=ROW;++i){
	for(j=0;j<=COL+1;++j){
	    if(x[i][j]==1)
		printf("*");
	    else
		printf(".");
	}
	printf("\n");
    }      
}
  
/* MAIN*/
int main(){
    int i;
    char c;
    srand(time(NULL));
    printf("r: Random initialization\nn: Manual initialization\n");
    do{
	c=getc(stdin);
	switch(c){
	case 'r':
	    rand_init();
	    break;
	case 'm':
	    init();
	    break;
	default:
	    printf("Wrong choice! Choose again!\n");
	    break;
	}
    }
    while(c!='r'&&c!='m');
    for(i=0;i<MAX;++i){
	printf("Generation %4d\n",i+1);
	check_all();
	print();
	sleep(0);
    }
}
