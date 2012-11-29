#include<stdio.h>
#include<time.h>
#define TRUE 1
#define FALSE 0
#define square(x) x*x   /* Macro for square of x*/
#define N 30   /* Number of items*/
#define L 10   /* Biggest weight allowed for a box*/
#define STIMES 100000 /* Simulation's time*/ 
/* Data structure*/
struct data{
  int useless[STIMES];
  long int useless_total, useless_square_total;
  double useless_average, useless_variance;
} next, fast, sortnext, sortfast;
int a[N];  /* Items' weights*/
int totalweight;
int is_sorted;
/* Functions' declaration*/
int partition(int left, int right);
void quicksort(int left, int right);
void bubblesort();
void print_array(int a[], int len);
void initialize();
void nextfit(int k);
void fastfit(int k);
void calculate(struct data *x);
/*Main*/
int main(){
  int i;
  clock_t start,end;
  start=clock();
  srand(time(NULL));
  for(i=0; i<STIMES; ++i){
    initialize();
    is_sorted=FALSE;
    nextfit(i);
    fastfit(i);
    quicksort(0,N-1);
    is_sorted=TRUE;
    nextfit(i);
    fastfit(i);
  }
  /* Calculate useless space's average and variance*/
  calculate(&next);
  calculate(&fast);
  calculate(&sortnext);
  calculate(&sortfast);
  /* Print the result*/
  printf("Result                  NextFit FastFit SortNext SortFast\n");
  printf("Useless space average:  %2.3f  %2.3f  %2.3f   %2.3f\n",
	 next.useless_average, fast.useless_average, 
	 sortnext.useless_average, sortfast.useless_average);
  printf("Useless space variance: %.3f   %.3f   %.3f    %.3f\n",
	 next.useless_variance, fast.useless_variance, 
	 sortnext.useless_variance, sortfast.useless_variance);
  end=clock();
  printf("Execution time: %lf (s)\n", (double)((end-start)/(double)CLOCKS_PER_SEC));
  return 0;
}
/* Partition used in Quicksort*/
int partition(int left, int right){
  int pivot,i,j,k,tmp;
  if(left>=right)
    return left;
  pivot=a[left];
  k=left;
  for(i=left+1; i<=right; ++i){
    if(a[i]>pivot){
      tmp=a[i];
      for(j=i; j>k; --j){
	a[j]=a[j-1];
      }
      a[k++]=tmp;
    }
  }
  return k;
}
/* Quicksort*/
void quicksort(int left, int right){
  int c;
  if(left>=right)
    return;
  c=partition(left,right);
  quicksort(left,c-1);
  quicksort(c+1,right);
}
/* Bubblesort*/
void bubblesort(){
  int i,j,tmp,flag;
  do{
    flag=0;
    for(i=0; i<N-1; ++i){
      if(a[i]<a[i+1]){
	tmp=a[i];
	a[i]=a[i+1];
	a[i+1]=tmp;
	flag=1;
      }
    }
  }
  while(flag);
}
/* Print array*/
void print_array(int a[],int len){
  int i;
  for(i=0; i<len; ++i){
    printf("%d ",a[i]);
  }
  printf("\n");
}
/* Random Initialize*/
void initialize(){
  int i;
  totalweight=0;
  for(i=0; i<N; ++i){
    a[i]=rand()%(L-2)+1;
    totalweight+=a[i];
  }
}
/* Next fit method*/
void nextfit(int k){
  int i,num,sum,useless;
  num=0; /* number of boxes*/
  sum=0; /* current box's weight*/
  for(i=0; i<N; ++i){
    if(sum+a[i]<=L){
      sum+=a[i];
    }
    else{
      num++;
      sum=a[i];
    }
  }
  useless=num*L-totalweight;
  if(is_sorted){
    sortnext.useless[k]=useless;
    sortnext.useless_total+=useless;
    sortnext.useless_square_total+=square(useless);
  }
  else{
    next.useless[k]=useless;
    next.useless_total+=useless;
    next.useless_square_total+=square(useless);
  }
}
/* Fast fit method*/
void fastfit(int k){
  int i,j,num,sum[N],useless;
  num=0; /* number of boxes*/
  for(i=0; i<N; ++i){
    for(j=0; j<num; ++j){
      if(sum[j]+a[i]<=L){
	sum[j]+=a[i];
	break;
      }
    }
    if(j==num)
      sum[num++]=a[i];
  }
  useless=num*L-totalweight;
  if(is_sorted){
    sortfast.useless[k]=useless;
    sortfast.useless_total+=useless;
    sortfast.useless_square_total+=square(useless);
  }
  else{
    fast.useless[k]=useless;
    fast.useless_total+=useless;
    fast.useless_square_total+=square(useless);
  }
}
/* Calculate useless space's average and variance*/
void calculate(struct data *x){
  x->useless_average=x->useless_total/(double)STIMES;
  x->useless_variance=
    ((x->useless_square_total)/(double)STIMES-square(x->useless_average))/(double)STIMES;
}
