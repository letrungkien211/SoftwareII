#include<stdio.h>
#include<time.h>
#define STIMES 1000000.0 /* Simulating times*/
#define absolute(x) x>0 ? x : -x
/* Main*/
int main(){
  int i,A_and_I=0, I=0;
  double p;
  srand(time(NULL));
  /* A_and_I: Number of times both event A and I occur
     I: Number of times event I occurs*/
  for(i=0; i<STIMES; ++i){
    switch(rand()%3){
    case 0:/* Event A occurs*/
      switch(rand()%2){
      case 0: 	/* Event I occurs*/
	A_and_I++; /* increment A_and_I*/
	I++;       /* increment I */
	break;
      case 1: 	/* Event I doesn't occur*/
	break;
      }
      break;
    case 1:  /* Event I doesn't occur */
      break;
    case 2:  /* Event I occures but event A does not*/
      I++; /* Increment I*/
      break;
    }
  }
  p=A_and_I/(double)I;
  printf("P(A is pardoned)=%lf\n",p);
  if(absolute(p-1/3.0)<0.01)
    printf("A's probability of surviving doesn't change.\n");
  else
    printf("A's probability of surviving changes.\n");
  return 0;
}
