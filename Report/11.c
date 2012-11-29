#include<stdio.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#define MAX 10
#define MAXLEN 40
#define PI 3.14159265 /* PI*/
#define E 2.71828183  /* base of natural logarithms*/
#define R 1.61803399 /* golden ratio*/
#define ERROR 0.00001 /* allowed error*/
double data[4][MAX];
double part[4]; /* four parts in an expression*/
char *string[4]; /* used in converting postfix's expression into infix's one*/
const char op[4]="+-*/"; /* operators*/    
/* string type stack*/
typedef struct s_stack{
  char data[MAXLEN];
  struct s_stack *next;
}S_STACK;
S_STACK *s_top;
/* double type stack*/
typedef struct d_stack{
  double data[MAXLEN];
  int last;
} D_STACK;
D_STACK *d_top;
/* Functions' declaration*/
void s_push(char *f);
char *s_pop();
int pricmp(char op1, char op2);
void to_infix(char *a, int len, char *b);
int d_push(double data);
double d_pop();
double cal_postfix(char *a, int len);
int almostInteger(double x);
void initialize();
void create();
/*****************Main*************************/
int main(int argc, char **argv){
  int i,j,k,l;
  double result;
  initialize(); /* initializing*/
  /* Start generating*/
  for(i=1; i<MAX; ++i){
    part[0]=data[0][i];
    string[0][3]=i+'0';
    for(j=1; j<MAX; ++j){
      part[1]=data[1][j];
      string[1][2]=j+'0';
      for(k=1; k<MAX; ++k){
	part[2]=data[2][k];
	string[2][2]=k+'0';
	for(l=1; l<MAX; ++l){
	  part[3]=l;
	  string[3][0]=l+'0';
	  create(); /* create postfix expressions*/
	}
      }
    }
  }
  return 0;
}
/* Initialize data*/
void initialize(){
  int i;
  d_top=(D_STACK*)malloc(sizeof(D_STACK)); /*allocate memory for d_top*/ 
  for(i=0; i<4; i++)
    string[i]=(char*)malloc(sizeof(char)*20);
  string[0][0]='P'; string[0][1]='I'; string[0][2]='^';string[0][4]='\0';
  string[1][0]='e'; string[1][1]='^'; string[1][3]='\0';
  string[2][0]='r'; string[2][1]='^'; string[2][3]='\0';
  string[3][1]='\0';
  for(i=0; i<MAX; ++i){
    data[0][i]=pow(PI,i);
    data[1][i]=pow(E,i);
    data[2][i]=pow(R,i);
    data[3][i]=i;
  }
}
/* Create postfix expressions
   With four numbers and three operators, there are 5 possible expressions
   01$2$3$  012$3$$  01$23$$  012$$3$  0123$$$*/
void create(){
  int i,j,k;
  char a[10],b[20];
  double result;
  for(i = 0;i < 4;i++){
    for(j = 0;j < 4;j++){
      for(k = 0;k < 4;k++){
	if(i==3 && j==3 && k==3)
	  continue;
	/* Case 1*/
	sprintf(a,"01%c2%c3%c\0",op[i],op[j],op[k]);
	result=cal_postfix(a,7);
	if(almostInteger(result) && result >100){
	  to_infix(a,7,b);
	  printf("%lf=%s\n",result,b);
	}
	/* Three operators have the same priority.*/
	if(pricmp(op[i],op[j])==0 && pricmp(op[j],op[k])==0)
	  continue;
	/* Case 2*/
	if(pricmp(op[i],op[j])){
	  sprintf(a,"012%c3%c%c\0",op[i],op[j],op[k]);
	  result=cal_postfix(a,7);
	  if(almostInteger(result)&& result >100){
	    to_infix(a,7,b);
	    printf("%lf=%s\n",result,b);
	  }
	}
	/* Case 3*/
	if(pricmp(op[j],op[k])){
	  sprintf(a,"01%c23%c%c\0",op[i],op[j],op[k]);
	  result=cal_postfix(a,7);
	  if(almostInteger(result) && result >100){
	    to_infix(a,7,b);
	    printf("%lf=%s\n",result,b);
	  }
	}
	/* Case 4*/
	if(pricmp(op[j],op[k])){
	  sprintf(a,"012%c%c3%c\0",op[i],op[j],op[k]);
	  if(almostInteger(result) && result >100){
	    to_infix(a,7,b);
	    printf("%lf=%s\n",result,b);
	  }
	}
	/* Case 5*/
	if(pricmp(op[j],op[k])){
	  sprintf(a,"0123%c%c%c\0",op[i],op[j],op[k]);
	  result=cal_postfix(a,7);
	  if(almostInteger(result) && result >100){
	    to_infix(a,7,b);
	    printf("%lf=%s\n",result,b);
	  }
	}
      }
    }
  }
}
/* Check if a number is a almost integer*/
int almostInteger(double x){
  return (fabs(x-(int)x)<ERROR) ? 1 : 0;
}
/* push new data to s_top*/
void s_push(char *f){
  struct s_stack *p;
  p=(struct s_stack *)malloc(sizeof(struct s_stack));
  if(!p){
    printf("Memory Error\n");
    exit(1);
  }
  strcpy(p->data,f);
  p->next=s_top;
  s_top=p;
}
/* pop data from s_top*/
char *s_pop (){
  S_STACK *tmp;
  char *s;
  if(!s_top){
    printf("s_stack is empty\n");
    return NULL;
  }
  tmp=s_top;
  s=(char*)malloc(MAXLEN);
  strcpy(s,tmp->data);
  s_top=tmp->next;
  free(tmp);
  return s;
}
/* Operators' priority comparation*/
int pricmp(char op1,char op2){
  if(op1=='+'|| op1=='-'){
    if(op2=='+'|| op2=='-')
      return 0;
    else 
      return -1;
  }
  else{
    if(op2=='+'|| op2=='-')
      return 1;
    else
      return 0;
  }
}
/* convert from postfix to infix expression*/
void to_infix(char *a,int len,char *b){
  char tmp[MAXLEN],tmp1[MAXLEN],tmp2[MAXLEN];     /* temporary strings*/
  int i,j;
  j=0; /* initialize part's counter*/
  s_top=NULL;
  for(i=0;i<len; ++i){
    if(isdigit(a[i])){
      sprintf(tmp,"%s\0",string[j++]);
      s_push(tmp);
    }
    else{
      sprintf(tmp2,"%s",s_pop());
      sprintf(tmp1,"%s",s_pop());
      sprintf(tmp,"(%s %c %s)\0",tmp1,a[i],tmp2);
      s_push(tmp);
    }
  }
  strcpy(b,s_pop());
}
/* push data to d_top*/
int d_push(double data){
  /* if stack is full*/
  if(d_top->last>=MAXLEN-1)
    return 0;
  else
    d_top->data[d_top->last++]=data;
  return 1;
}
/* pop data from d_top*/
double d_pop(){
  /* if stack is empty*/
  if(!d_top->last)
    return 0;
  else
    return d_top->data[--d_top->last];
}
/* Calculating postfix expression*/
double cal_postfix(char *a,int len){
  double op1,op2;
  int i,j=0;
  d_top->last=0;
  for(i=0; i<len; ++i){
    if(isdigit(a[i]))
      d_push(part[j++]);
    else{
      op2=d_pop();
      op1=d_pop();
      switch(a[i]){
      case '+': d_push(op1+op2);
	break;
      case '-': d_push(op1-op2);
	break;
      case '*': d_push(op1*op2);
	break;
      case '/': 
	if(op2==0.0) return ERROR;
	d_push((double)op1/op2);
	break;
      default:
	break;
      }
    }
  }
  return d_pop();  
}
