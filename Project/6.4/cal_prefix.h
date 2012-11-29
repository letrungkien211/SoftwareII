#define ERROR 1000000.0

#define fac_max 11          // maximum valid number, if n>fac_mac, skip calculating 
#define sig_max 31          // maximum valid number, if n>sig_mac, skip calculating 

int factorial(int n);       // n!
int fac[fac_max];


int sigma(int n);           // sum(n)=1+2+...+n
int sig[sig_max];

// store sigma(n) into element sig[n] of array sig[sig_max]
void sig_init(){
  int i;
  for(i=0; i<sig_max; ++i)
    sig[i]=sigma(i);
}

// store factorial(n) into element fac[n] of array fac[fac_max]
void fac_init(){
  int i;
  for(i=0;i<fac_max; ++i){
    fac[i]=factorial(i);
  }
}

int is_integer(double x);   // check if x is integer

/* Calculating prefix expression*/
double cal_prefix(char *a, int len){
  double op1,op2;
  struct d_stack *top;
  int i;
  d_init(&top);             // init d_stack
  
  for(i=0; i<len; ++i){
    if(isdigit(a[i]))
      d_push(&top,(double)(a[i]-'0'));
    else
      switch(a[i]){
      case '+': d_push(&top,d_pop(&top)+d_pop(&top));
	break;
      case '-': d_push(&top,-d_pop(&top)+d_pop(&top));
	break;
      case '*': d_push(&top,d_pop(&top)*d_pop(&top));
	break;
      case '/': 
	op2=d_pop(&top);
	if(op2==0.0) return ERROR;
	d_push(&top,d_pop(&top)/op2);
	break;
	/*mod*/
      case '%': 
	op2=d_pop(&top);
	op1=d_pop(&top);
	if(op2==0.0||!is_integer(op1)||!is_integer(op2)) return ERROR;
	d_push(&top,(int)op1%(int)op2);
	break;
	/*Factorial*/
      case '^':
	op2=d_pop(&top);
	op1=d_pop(&top);
	d_push(&top,pow(op1,op2));
	break;
      case '!': 
	i+=3;
	op1=d_pop(&top);
	if(op1<0||!is_integer(op1)||op1>8) return ERROR;
	d_push(&top,fac[(int)op1]);
	break;
	/*sqrt(x) or sin(x) or sum(x)*/
      case 's':
	switch(a[i+1]){
	case 'q':
	  i+=3;
	  op1=d_pop(&top);
	  if(op1<0||!is_integer(sqrt(op1))) return ERROR;
	  d_push(&top,sqrt(op1));
	  break;
	case 'i':
	  i+=3;
	  d_push(&top,sin(d_pop(&top)));
	  break;
	case 'u':
	  i+=3;
	  op1=d_pop(&top);
	  if(op1<0||!is_integer(op1)||op1>30) return ERROR;
	  d_push(&top,sig[(int)op1]);
	  break;
	default:
	  return ERROR;
	}
	break;
	/*cos*/
      case 'c':
	i+=3;
	d_push(&top,cos(d_pop(&top)));
	break;
      case '.':
	if(a[i+1]=='~'){
	  i+=3;
	  op1=d_pop(&top);
	  if(op1!=4) return ERROR;
	  d_push(&top, (double)op1/9);
	}
	else{
	  i+=3;
	  op1=d_pop(&top);
	  if(op1!=4) return ERROR;
	  d_push(&top, (double)op1/10);
	}
	break;
      case ' ':
	break;
      default:
	return ERROR;
      }
  }
  return d_pop(&top);
}

/* Iterating index to calculate*/
int factorial(int n){
  int i;
  int fac=1;
  for(i=2; i<=n;++i){
    fac*=i;
  }
  return fac;
}

int sigma(int n){
  int sig=0,i;
  for(i=1; i<=n; ++i)
    sig+=i;
  return sig;
}

/* Simple check*/
int is_integer(double x){
  int tmp;
  tmp=x;        // tmp=integer part of x
  return !(x-tmp) ? 1 : 0;
}
