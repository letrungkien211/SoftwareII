#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#define MAXLEN 50
#define ERROR 100000.0
int n_ans=0;   /*number of answers*/
const char express[]="123456789";
const char op[4]="+-*/";    /* operators*/
int NUMBER=100;
int part[4]; /* Divide 123456789 into four parts*/
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
int to_integer(int i, int j);
int is_integer(double x);
void create();
/*Main*/
int main(int argc, char **argv){
  int i,j,k;
  d_top=(D_STACK*)malloc(sizeof(D_STACK)); /*allocate memory for d_top*/ 
  /*Divide string express[] to sets of four parts
    Then create postfix expressions for each set*/
  for(i=0; i<6; ++i){
    part[0]=to_integer(0,i); /* First part*/
    for(j=i+1; j<7; j++){
      part[1]=to_integer(i+1,j); /* Second part*/
      for(k=j+1; k<8; k++){ 
	part[2]=to_integer(j+1,k); /* Third part*/
	part[3]=to_integer(k+1,8); /* Fouth part*/
	create(); /* create postfix expressions*/
      }
    }
  }
  printf("%d answers found.\n",n_ans);
  return 0;
}
/* Create postfix expressions:
   With four numbers and three operators, there are 5 possible expressions
   01$2$3$ 012$3$$ 01$23$$  012$$3$  0123$$$*/
void create(){
  int i,j,k;
  char a[10],b[20];
  double result;
  for(i = 0;i < 4;i++){
    for(j = 0;j < 4;j++){
      for(k = 0;k < 4;k++){
	/* Case 1*/
	sprintf(a,"01%c2%c3%c\0",op[i],op[j],op[k]);
	result=cal_postfix(a,7);
	if(is_integer(result) && (int)result==NUMBER){
	  n_ans++;
	  to_infix(a,7,b);
	  printf("%d=%s\n",NUMBER,b);
	}
	/* Three operators have the same priority.*/
	if(pricmp(op[i],op[j])==0 && pricmp(op[j],op[k])==0)
	  continue;
	/* Case 2*/
	if(pricmp(op[i],op[j])){
	  sprintf(a,"012%c3%c%c\0",op[i],op[j],op[k]);
	  result=cal_postfix(a,7);
	  if(is_integer(result) && (int)result==NUMBER){
	    n_ans++;
	    to_infix(a,7,b);
	    printf("%d=%s\n",NUMBER,b);
	  }
	}
	/* Case 3*/
	if(pricmp(op[j],op[k])){
	  sprintf(a,"01%c23%c%c\0",op[i],op[j],op[k]);
	  result=cal_postfix(a,7);
	  if(is_integer(result) && (int)result==NUMBER){
	    n_ans++;
	    to_infix(a,7,b);
	    printf("%d=%s\n",NUMBER,b);
	  }
	}
	/* Case 4*/
	if(pricmp(op[j],op[k])){
	  sprintf(a,"012%c%c3%c\0",op[i],op[j],op[k]);
	  if(is_integer(result) && (int)result==NUMBER){
	    n_ans++;
	    to_infix(a,7,b);
	    printf("%d=%s\n",NUMBER,b);
	  }
	}
	/* Case 5*/
	if(pricmp(op[j],op[k])){
	  sprintf(a,"0123%c%c%c\0",op[i],op[j],op[k]);
	  result=cal_postfix(a,7);
	  if(is_integer(result) && (int)result==NUMBER){
	    n_ans++;
	    to_infix(a,7,b);
	    printf("%d=%s\n",NUMBER,b);
	  }
	}
      }
    }
  }
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
/* Example: "/*"  precede "+-" */
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
      sprintf(tmp,"%d\0",part[j++]);
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
/* Convert express from index i to j to integer*/
/* Eg: to_integer(0,2)=123*/
int to_integer(int i,int j){
  int k;
  char s[9];
  for(k=0; k<j-i+1; ++k){
    s[k]=i+k+1+'0';
  }
  s[k]='\0';
  return atoi(s);
}
/* Check if double type x is integer or not*/
int is_integer(double x){
  return (x-(int)x==0);
}
