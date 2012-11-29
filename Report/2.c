#include<stdio.h>
#include<time.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#define RANGE 200 
#define GROUPMAX 8 /* Biggest number of triangles whose circumferences are the same*/
#define MAXCIRCUM 1000000 /* Biggest circumference allowed*/
/* list data structure */
struct list{
    int circum;       /* circumference*/
    int x[GROUPMAX];  /* shorter edge*/
    int y[GROUPMAX];  /* longer edge*/
    int z[GROUPMAX];  /* hypotenuse*/
    int num;          /* number right of triangles whose circumferences are equal circum*/
    struct list *next;/* seft refrerence*/
} *head=NULL;
int n_list=0; /* number of list's nodes*/
/* Functions' declaration*/
struct list *search(int circum);
int add(int a, int b, int c);
void delete_next(struct list *current);
void generate(int num);
void print(int num, char *filename);
/* Main*/
int main(int argc, char **argv){
    clock_t start, end;
    char filename[20]="solution-2.txt";
    start=clock();
    generate(3); /* generate satisfying groups of trianlges*/
    if(argc>1)
	strcpy(filename, argv[1]);
    print(3,filename); /* save the results to a .txt file*/
    printf("There were %d groups found. Results are save in %s\n",n_list, filename);
    printf("Maximum of Circumference: %d\n", MAXCIRCUM);
    printf("Enter 'p' to print all the results.\n");
    end=clock();
    if(getchar()=='p')
	print(3,NULL);
    printf("Execution's time: %lf(s)\n", (double)((end-start)/(double)CLOCKS_PER_SEC));
    return 0;
}
/* search the last member whose circumference is not bigger than input argument*/
struct list *search(int circum){
    struct list *p;
    p=head;
    if(!head)
	return NULL;
    if(p->circum>circum)
	return NULL;
    while(p->next){
	if(p->next->circum>circum)
	    return p;
	p=p->next;
    }
    return p;
}
/* orderly add data into list*/
int add(int a,int b,int c){
    int i,j,circum;
    struct list *p,*q,*tmp;
    /* calculate circumference*/
    circum=a+b+c;
    /* create new node*/
    p=(struct list*)malloc(sizeof(struct list));
    if(!p){
	printf("Memory Error\n");
	exit(1);
    }
    p->circum=circum;
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
	q=search(circum);
	if(!q){ 
	    /*head==NULL or head->circum>circum*/
	    p->next=head;
	    head=p;
	    n_list++;
	}
	else if(q->circum==circum){
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
	    /* store (a,b,c) so that q->x[],y[],z[] arrays are*/
	    /* sorted in increment order of x[]*/
	    q->x[i]=a;
	    q->y[i]=b;
	    q->z[i]=c;
	    q->num++;
	}
	else{
	    /* if circum is not on the list*/
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
void generate(int num){
    int a,b,c,i,j;
    struct list *p,*q;
    /* Step 1. Store data to list
       Set of positive integral solutions of equationsa^2+b^2=c^2 is 
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
       Remove nodes with only one triangle (field num=1)
       Generate more solutions from found ones*/
    q=NULL;
    p=head;
    while(p){
	if(p->num>=num){
	    /* if (a,b,c) is a solution then (2a,2b,2c) is also a solution*/
	    if(p->circum<MAXCIRCUM){
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
/* print groups whose number of triangles is not smaller than num*/
void print(int num,char *filename){
    struct list *p;
    int i,j;
    FILE *fp;
    if(!filename)
	fp=stdout; /* print to standard output*/
    else
	fp=fopen(filename, "w");
    if(!fp){
	fprintf(stderr, "File open error.\n");
	exit(1);
    }
    p=head;
    while(p){
	if(p->num>=num){
	    fprintf(fp,"Circumference=%d, %d triangles: ",p->circum,p->num);
	    for(i=0; i<p->num; ++i){
		fprintf(fp,"(%d,%d,%d) ", p->x[i],p->y[i],p->z[i]);
	    }
	    fprintf(fp,"\n");
	}
	p=p->next;
    }
    fclose(fp);
}
