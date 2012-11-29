#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<math.h>
#define N 50 /* Number of items*/
#define W 45  /* Biggest weight allowed*/
#define STIMES 1000   /* Simulation's times*/
int SA=1;  /* SA=1: Simulated Annealing, SA=0: Hill Climbing*/
/* item structure*/
typedef struct item{
  int weight, price;
} ITEM;
/* solution structure*/
typedef struct state{
  int num, weight, price;
  int list[N],flag[N];
} STATE;
STATE optimum; /* optimum solution*/
const ITEM NULL_item={0,0}; /* an invalid item*/
ITEM list[N]=
  {{2, 21},{10, 22},{7, 28},{2, 21},{4, 12},{9, 24},{10, 15},{7, 2},{8, 25},{5, 28},
   {3, 4},{10, 22},{9, 36},{8, 2},{8, 7},{5, 40},{7, 14},{3, 40},{9, 33},{7, 21},
   {2, 28},{10, 22},{7, 14},{9, 36},{7, 28},{2, 21},{10, 18},{4, 12},{9, 24},{10, 15},
   {4, 21},{7, 2},{8, 25},{5, 28},{2, 28},{3, 4},{10, 22},{9, 36},{7, 31},{8, 2},
   {8, 7},{5, 40},{7, 14},{5, 4},{7, 28},{3, 40},{9, 33},{7, 35},{7, 21},{9, 20}};
/* variables used in simulated annealing*/
double Starttemperature=100.0;
double coolingRate=0.8;  
double Endtemperature=0.0001;
double temperature; 
/* functions' declaration*/
int rand_int(int n);
double rand_double(double min, double max);
int test(ITEM x, ITEM y, double T);
ITEM nextitem(STATE s, int *next);
STATE search();
int partition(int a[], int left, int right);
void quicksort(int a[], int left, int right);
/**Main**/
int main(){
  int s_times, i,endpoint;
  STATE ans;
  clock_t start,end;
  start=clock();
  srand(time(NULL));  
  temperature=Starttemperature; /* Initialize temperature*/
  for(s_times=0; s_times<STIMES; ++s_times){
    ans=search();
    if(ans.price>optimum.price){
      optimum=ans;
    }
  }
  quicksort(optimum.list,0,optimum.num-1); /* sorting*/
  printf("Best solution found:\n");
  printf("Weight sum= %d\nPrice sum= %d\nNumber of items= %d\n",
	 optimum.weight,optimum.price,optimum.num);
  printf("Index: (W,P)\n");
  for(i=0; i<optimum.num; ++i){
    printf("%-2d: #%-2d (%d, %d)\n", i+1, 
	   optimum.list[i]+1,list[optimum.list[i]].weight,list[optimum.list[i]].price);
  }
  end=clock();
  printf("Execution time: %lf(s)\n",(double)((end-start)/(double)CLOCKS_PER_SEC));
  return 0;
}
/* produce a random int type number in [0,n-1]*/
int rand_int(int n){
  return (int)((double)rand()/(double)(RAND_MAX+1.0)*n);
}
/* return a random double type number in [min,max]*/
double rand_double(double min, double max){
  return (min+((max-min)*rand())/RAND_MAX);
}
/* Test function*/
int test(ITEM x, ITEM y,double T){
  double m;
  if(!x.weight)
    return 0;
  if(!y.weight)
    return 1;
  return (exp(x.price/(double)x.weight-y.price/(double)y.weight)>rand_double(0,1.0));
}
/* Find the next item to put into collection*/
ITEM nextitem(STATE s,int *next){
  ITEM tmp, tmp2;
  int d1,d2,i,j;
  tmp=NULL_item;
  if(SA)
    temperature*=coolingRate;
  for(i=0; i<N; ++i){
    if(s.flag[i]==0 &&
       list[i].weight+s.weight<=W &&
       (!tmp.weight||list[i].price/(double)list[i].weight>tmp.price/(double)tmp.weight)){
      *next=i;
      tmp=list[i];
    }
  }
  if(!tmp.weight)
    return NULL_item;
  if(!SA)
    return tmp;
  d1=rand_int(N);
  d2=d1+N;
  for(i=d1; i<d2; ++i){
    j=i%N;
    if(!s.flag[j] &&
       list[j].weight+s.weight<=W &&
       (SA && test(list[j],tmp,temperature))){
      *next=j;
      tmp=list[j];
      if(SA)
	return list[j];
    }
  }
  if(SA && temperature<Endtemperature)
    return NULL_item;
  return tmp;
}
/* Search for the optimum collection*/
STATE search(){
  STATE s;
  ITEM tmp;
  int i,cnt;
  /* Initialize*/
  for(cnt=0; cnt<N; ++cnt){
    s.flag[cnt]=0;
    s.list[cnt]=0;
  }
  s.weight=0;
  s.price=0;
  s.num=0;
  /* Start Annealing*/
  while(s.weight<W){
    tmp=nextitem(s,&i);
    if(tmp.weight==0)
      break;
    s.num++;
    s.weight+=tmp.weight;
    s.price+=tmp.price;
    s.list[s.num-1]=i;
    s.flag[i]=1;
  }
  return s;
}
/* Partition used in Quicksort*/
int partition(int a[],int left, int right){
  int pivot,i,j,k,tmp;
  if(left>=right)
    return left;
  pivot=a[left];
  k=left;
  for(i=left+1; i<=right; ++i){
    if(a[i]<pivot){
      tmp=a[i];
      for(j=i; j>k; --j){
	a[j]=a[j-1];
      }
      a[k++]=tmp;
    }
  }
  return k;
}
/* Quick sort*/
void quicksort(int a[], int left, int right){
  int c;
  if(left>=right)
    return;
  c=partition(a,left,right);
  quicksort(a,left,c-1);
  quicksort(a,c+1,right);
}
