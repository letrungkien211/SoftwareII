/* convert from prefix to infix expression*/

void to_infix(char *a,int len,char *b){
  struct s_stack *top; // using a string type stack.
  char tmp[STR_LEN],tmp1[STR_LEN],tmp2[STR_LEN];     // temporary strings
  int i;
  
  s_init(&top);        // initialization
  for(i=0; i<len; ++i){
    if(isdigit(a[i])){
      sprintf(tmp,"%c\0",a[i]);
      s_push(&top,tmp);
    }
    // if the number of arguments required is 2
    else if (a[i]=='+'||a[i]=='-'||a[i]=='*'||a[i]=='/'||a[i]=='^'||a[i]=='%'){  
      sprintf(tmp2,"%s",s_pop(&top));
      sprintf(tmp1,"%s",s_pop(&top));
      if(a[i]=='-')                               // ensure order is right.
	sprintf(tmp,"(%s %c %s)\0",tmp2,a[i],tmp1);
      else
	sprintf(tmp,"(%s %c %s)\0",tmp1,a[i],tmp2);
      s_push(&top,tmp);
    }
    // s: sqrt or sin
    else if(a[i]=='s'){
      ++i;
      if(a[i]=='q'){
	sprintf(tmp, "(%s %s)\0","√",s_pop(&top));
	s_push(&top,tmp);
	i+=2;
      }
      else if(a[i]=='i'){
	sprintf(tmp, "(%s %s)\0","sin",s_pop(&top));
	s_push(&top,tmp);
	++i;
      }
      else if(a[i]=='u'){
	sprintf(tmp, "(%s %s)\0","∑",s_pop(&top));
	s_push(&top,tmp);
	++i;
      }
      else {
	printf("Unkown expression\n");
	return;
      }
    }
    // factorial
    else if(a[i]=='!'){
      sprintf(tmp, "(%s %c)\0",s_pop(&top),a[i]);
      s_push(&top,tmp);
    }
    // cos
    else if(a[i]=='c'){
      sprintf(tmp, "(%s %s)\0","sin",s_pop(&top));
      s_push(&top,tmp);
      i+=2;
    }
    else if(a[i]=='.'){
      sprintf(tmp, "%c%s",a[i],s_pop(&top));
      s_push(&top,tmp);
    }
    // space, do nothing
    else if (a[i]==' ')
      continue;
    else{
      printf("Unknown expression\n");
      return;
    }
  }
  strcpy(b,s_pop(&top));
}
  
