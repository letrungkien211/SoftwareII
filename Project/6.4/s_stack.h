// s_stack.h   string_stack

typedef struct e_stack{                            // String type stack
  char expression[MAX];
  struct e_stack *next;
}E_STACK;

int e_push(E_STACK **top, char *s){
  E_STACK *p;
  p=(E_STACK*)malloc(sizeof(E_STACK));
  if(!p){
    printf("Memory Error\n");
    exit(1);
  }
  strcpy(p->expression, s);
  p->next=*top;
  *top=p;
  return 1;
}

int *e_pop (E_STACK **top, char *s){
  if(!top){
    return 0;
  }
  strcpy(s,*top->expression);
  *top=(*top)->next;
  return 1;
}
