// d_stack.h   string_stack


struct d_stack{                            // String type stack
  double data;
  struct d_stack *next;
};

void d_init(struct d_stack **top){
  *top=NULL;
}

// check if d_stack is empty
int d_empty(struct d_stack *top){
  if(top) return 0;
  return 1;
}

// allocate memory for d_stack
struct d_stack *d_alloc(){
  return (struct d_stack *)malloc(sizeof(struct d_stack));
}

// create a new d_stack
struct d_stack *new_d_stack(double f){
  struct d_stack *p;
  p=d_alloc();
  if(p){
    p->data=f;
    p->next=NULL;
  }
  return p;
}

// push data to top
void d_push(struct d_stack **top, double f){
  struct d_stack *p;
  p=new_d_stack(f);
  p->next=*top;
  *top=p;
}

// pop data from top
double d_pop (struct d_stack **top){
  double tmp;
  if(d_empty(*top)){
    printf("d_stack is empty\n");
    return -1;
  }
  tmp=(*top)->data;
  *top=(*top)->next;
  return tmp;
}

// get data from top without pop
double  d_get(struct d_stack *top){
  if(d_empty(top)){
    printf("d_stack is empty\n");
    return -1;
  }
  return top->data;
}

// print stack
void d_print_stack(struct d_stack *top){
  while(top){
    printf("%lf\n",top->data);
    top=top->next;
  }
  printf("***************\n");
}
