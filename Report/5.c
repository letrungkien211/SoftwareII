#include<stdio.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define RANGE 200 
#define GROUPMAX 8           /* Biggest number of triangles whose areas are the same*/
#define MAXAREA 100000000    /* Biggest area allowed*/
/* list data structure */
struct list{
  int area;       /* area*/
  int x[GROUPMAX];  /* shorter edge*/
  int y[GROUPMAX];  /* longer edge*/
  int z[GROUPMAX];  /* hypotenuse*/
  int num;          /* number of triangles whose areas are equal to field area of struct list*/
  struct list *next;/* seft refrerence*/
} *head=NULL;
int n_list=0; /* number of list's nodes*/
/* Functions' declaration*/
struct list *search(int area);
int add(int a, int b, int c);
void delete_next(struct list *current);
void generate();
void print(int num, char *filename);
/*Main*/
int main(int argc, char **argv){
  clock_t start, end;
  char filename[20]="solution-5.txt";
  start=clock();
  generate(); /* generating*/
  if(argc>1)
    strcpy(filename, argv[1]);
  print(2, filename); /* save results into a .txt file*/
  printf("There were %d groups found. Results are saved in %s\n",n_list,filename);
  printf("Maximum of Area: %d\n", MAXAREA);
  printf("Enter 'p' to print all the results.\n");
  end=clock();
  if(getchar()=='p')
    print(2,NULL); /* print the result on terminal*/
  printf("Execution's time: %lf(s)\n", (double)((end-start)/(double)CLOCKS_PER_SEC));
  return 0;
}
/* search the last member whose area is not bigger than input argument*/
struct list *search(int area){
  struct list *p;
  p=head;
  if(!head)
    return NULL;
  if(p->area>area)
    return NULL;
  while(p->next){
    if(p->next->area>area)
      return p;
    p=p->next;
  }
  return p;
}
/* orderly add data into list*/
int add(int a,int b,int c){
  int i,j,area;
  struct list *p,*q,*tmp;
  /* calculate area*/
  area=(a*b)/2;
  /* create new node*/
  p=(struct list*)malloc(sizeof(struct list));
  if(!p){
    printf("Memory Error\n");
    exit(1);
  }
  p->area=area;
  p->x[0]=a;
  p->y[0]=b;
  p->z[0]=c;
  p->num=1;
  p->next=NULL;
  if(!head){
    head=p;
    n_list=1;
  }
  else{
    q=search(area);
    if(!q){ 
      /* q==NULL means head==NULL or head->area>area*/
      p->next=head;
      head=p;
      n_list++;
    }
    else if(q->area==area){
      free(p);   /* no new node is needed*/
      for(i=0; i<q->num && q->x[i]<a; ++i);
      /* find the first index i satisfying that q->x[i]>=a*/
      if(q->x[i]==a && q->y[i]==b)
	/* (a,b,c) is already on the list*/
	return 0;
      /* shift arrays one step to the right*/
      for(j=q->num;j>i; --j){
	q->x[j]=q->x[j-1];
	q->y[j]=q->y[j-1];
	q->z[j]=q->z[j-1];
      }
      /* store (a,b,c) so that (q->x[],y[],z[])are sorted in increment order of x[]*/
      q->x[i]=a;
      q->y[i]=b;
      q->z[i]=c;
      q->num++;
    }
    else{
      /* if area is not on the list*/
      tmp=q->next;
      q->next=p;
      p->next=tmp;
      n_list++;
    }
  }
  return 1;
}
/* delete next node of current node*/
void delete_next(struct list *current){
  struct list *tmp;
  if(!current){
    /* next of current is head*/
    tmp=head;
    if(head){
      /* head is not NULL*/
      head=head->next;
      free(tmp);
      n_list--;
    }
  }
  else if(current->next){
    /* delete current->next*/
    tmp=current->next;
    current->next=tmp->next;
    free(tmp);
    n_list--;
  }
}
/* generate triangles satisfying the conditions */
void generate(){
  int a,b,c,i,j;
  struct list *p,*q;
  /* Step 1. Store data to list
     Solutions' set of equation a^2+b^2=c^2 is
     {(2*i*j, abs(j*j-i*i), i*i+j*j)| i,j are integar}
     So consider (i,j) in [1,RANGE-1], i<j*/
  for(i=1; i<RANGE; ++i){
    for(j=i+1; j<RANGE; ++j){
      a=(2*i*j<j*j-i*i) ? (2*i*j): (j*j-i*i);
      b=(2*i*j>j*j-i*i) ? (2*i*j): (j*j-i*i);
      c=i*i+j*j;
      add(a,b,c);
    }
  }
  /* Step 2. 
     Remove nodes with only one triangle. 
     Generate more solutions from found ones*/
  q=NULL;
  p=head;
  while(p){
    if(p->num>1){
      /* if (a,b,c) is a solution then (2a,2b,2c) is also a solution*/
      if(p->area<MAXAREA){
	for(i=0; i<p->num; ++i){
	  add(2*p->x[i],2*p->y[i],2*p->z[i]);
	}
      }
      q=p;
      p=p->next;
    }
    else{
      /* delete node which include no solution*/
      p=p->next;
      delete_next(q);
    }
  }
}
/* print groups whose number of triangles is not less than num*/
void print(int num, char *filename){
  struct list *p;
  int i,j;
  FILE *fp;
  if(!filename)
    fp=stdout;
  else
    fp=fopen(filename, "w");
  if(!fp){
    fprintf(stderr, "File open error.\n");
    exit(1);
  }
  p=head;
  while(p){
    if(p->num>=num){
      fprintf(fp,"Area=%d, %d triangles: ",p->area,p->num);
      for(i=0; i<p->num; ++i){
	fprintf(fp,"(%d,%d,%d) ", p->x[i],p->y[i],p->z[i]);
      }
      fprintf(fp,"\n");
    }
    p=p->next;
  }
}
