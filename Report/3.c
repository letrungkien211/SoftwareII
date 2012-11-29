#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#define cube(x) (x*x*x)
/* Using macro is faster than using pow() function in math.h*/
#define RANGE 1000
#define MAX 200   
int data[RANGE+1][4];   /* store valid expressions*/
int flag[RANGE+1];      /* flag indicates found or not-found state*/
int n_data=0;           /* number of valid expressions found*/
void check_one(int a,int b,int c,int d,int i,int j,int k,int l);
void check_all(int a,int b,int c,int d,int i,int j,int k,int l);
void search();
void print_one(int sum);
void print_all();
/* Main*/
int main(){
  clock_t start,end;
  start=clock();
  search(); /* searching*/
  print_all(); /* show the results*/
  if(n_data==RANGE+1)
    printf("SUCESS.\n");    
  else
    printf("FAILURE.\n");
  end=clock();
  printf("Execution's time: %lf(s)\n", (double)((end-start)/(double)CLOCKS_PER_SEC));
  return 0;
}
/* Check if sum=absolute(a+b+c+d) is in [0,RANGE]*/
void check_one(int a,int b,int c,int d,int i,int j,int k,int l){
  int tmp, cubesum;
  tmp=a+b+c+d;
  cubesum=abs(tmp);
  /* If cubesum is in [0,RANGE] and flag[cubesum]==0*/
  if(cubesum<=RANGE && !flag[cubesum]){
    /* If tmp is positive*/
    if(tmp>0){
      data[cubesum][0]=i;
      data[cubesum][1]=j;
      data[cubesum][2]=k;
      data[cubesum][3]=l;
    }
    /* else, flip signs of i,j,k,l*/
    else{
      data[cubesum][0]=-i;
      data[cubesum][1]=-j;
      data[cubesum][2]=-k;
      data[cubesum][3]=-l;
    }
    flag[cubesum]=1;   /* change found-unfound state*/
    n_data++;          /* increment number of valid expressions found*/
  }
}
/* Ignore sign of sum=a+-b+-c+-d then there are 8 different sums from (a,b,c,d)
   a+b+c+d;
   a+b+c-d; a+b-c+d; a-b+c+d;-a+b+c+d;
   a+b-c-d; a-b+c-d; a-b-c+d;*/
void check_all(int a,int b,int c,int d,int i,int j,int k,int l){
  check_one(a,b,c,d,i,j,k,l);
  check_one(a,b,c,-d,i,j,k,-l);
  check_one(a,b,-c,d,i,j,-k,l);
  check_one(a,-b,c,d,i,-j,k,l);
  check_one(-a,b,c,d,-i,j,k,l);
  check_one(a,b,-c,-d,i,j,-k,-l);
  check_one(a,-b,c,-d,i,-j,k,-l);
  check_one(a,-b,-c,d,i,-j,-k,l);
}
/* Search through set A={(i,j,k,l)| i,j,k,l is in [0,RANGE]}*/
void search(){
  int i,j,k,l;
  int a,b,c,d;   /* cubes of i,j,k,l respectively*/
  for(i=0; i<=MAX; ++i){
    a=cube(i);   
    /* calculate here, not inside inner loop to avoid calculating a more than once.*/
    for(j=i; j<=MAX; ++j){
      b=cube(j);
      for(k=j; k<=MAX; ++k){
	c=cube(k);
	for(l=k; l<=MAX; ++l){
	  if(n_data==RANGE+1)
	    return ;
	  d=cube(l);
	  check_all(a,b,c,d,i,j,k,l);
	}
      }
    }
  }
}
/* Print one valid arithmetic expression whose result is sum*/
void print_one(int sum){
  char sign[4];
  int i,flag=0;
  for(i=0; i<4; ++i){
    if(data[sum][i]==0)
      sign[i]=0;
    else if(data[sum][i]>0)
      sign[i]='+';
    else
      sign[i]='-';
  }
  printf("%d=", sum);
  for(i=0; i<4; ++i){
    if(data[sum][i]){
      if(flag==0){
	printf(" %d^3 ", data[sum][i]);
	flag=1;
      }
      else
	printf("%c%d^3 ",sign[i],abs(data[sum][i]));
    }
  }
  printf("\n");
}
/* Print all results*/
void print_all(){
  int i;
  for(i=0; i<=RANGE; ++i){
    if(flag[i]){
      print_one(i);
    }
    else
      printf("NOT found: %d\n",i);
  }
}
