// s_stack.h   string_stack

#define STR_LEN 200

struct s_stack{                            // String type stack
  char data[STR_LEN];
  struct s_stack *next;
};

void s_init(struct s_stack **top){
  *top=NULL;
}

// check if s_stack is empty
int s_empty(struct s_stack *top){
  if(top) return 0;
  return 1;
}

// allocate memory for s_stack
struct s_stack *s_alloc(){
  return (struct s_stack *)malloc(sizeof(struct s_stack));
}

// create a new s_stack
struct s_stack *new_s_stack(char *f){
  struct s_stack *p;
  p=s_alloc();
  if(p){
    strcpy(p->data,f);
    p->next=NULL;
  }
  return p;
}

// push new data to top
void s_push(struct s_stack **top, char *f){
  struct s_stack *p;
  p=new_s_stack(f);
  p->next=*top;
  *top=p;
}

// pop data from top
char *s_pop (struct s_stack **top){
  char *tmp;
  if(s_empty(*top)){
    printf("s_stack is empty\n");
    return NULL;
  }
  strcpy(tmp,(*top)->data);
  *top=(*top)->next;
  return tmp;
}

// get data from top (without pop)
char *s_get(struct s_stack *top){
  if(s_empty(top)){
    printf("s_stack is empty\n");
    return NULL;
  }
  return top->data;
}

// print_stack
void s_print_stack(struct s_stack *top){
  
  while(top&&top->data){
    printf("%s\n",top->data);
    top=top->next;
  }
  printf("***********************\n");
}
