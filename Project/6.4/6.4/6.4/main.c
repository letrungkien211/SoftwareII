/* Project: Generating prefix expression
   Description: Use four number 4 and common operators to create operands that produce number in a given interval [0,RANGE)
   Reference: Iba Hitoshi professor's book, chapter 6.
   
   Author: Le Trung Kien
   Student number: g041172
   Date: 2011/12/17   */

/* Header file*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
#include<string.h>

#include "s_stack.h"           	// string type stack
#include "d_stack.h"           	// double type stack
#include "cal_prefix.h"        	// calculating prefix expression
#include "to_infix.h"          	// convert prefix expression to infix


/* Calculating time (roughly)
RANGE    OP_MAX       OP_MAX1          Time(s)   
114       4            4               2
150       4            4               3
200       4            4               7

114       4            5               4
200       4            5               infinite??

114       6            4               7
 */
#define  RANGE 114            	// Range of results



/* The following is about two kinds of operators:
   KIND2: Need two arguments
   KIND1: Need one argument
   By adding new operators, we can create as much as desired different 
   prefix expression with four number 4.
---->>>>>>>EXTENDABLE
   In this case, RANGE=114 so 4 operators of kind2 and 4 operators of kind1 are used
*/


/* KIND 2: these operators required two arguments
   +: addition, -: subtraction, *: multiply, \: division, %: mod, ^: power*/

const char op[]="+-*/%^\0";     // 6 common operators                    
#define OP_MAX 4                // the number of KIND 2's operators will be used

//ATENTION: There are 6 operators in string op[], but 
//when RANGE=114, the last two operators are unnecessary.

/* KIND 1: these operators required only one arguments
   "sqrt" square of root , 
   "sum" eg: sum(4)=1+2+3+4*, 
   "!" factorial
   "." : divided by 10
   " " Space
   ATTENTION: this include "space" operator, which also means nothing is done
   The length of all strings equals the maximum required length, in this case 4 for "sqrt"
   The fact that all strings have the same length makes calculating process faster 
   (no need to calculate the length of input string)
*/
const char op1[][10]={"    ","sqrt","!   ","sum ", ".   "};     // 5 common operators.
#define OP_MAX1 4               // the number of KIND 2's operators will be used

void search();               	// search through as much as possible arthmetic expression

void main(){
  /* creating an array sig[sig_max=30] to store sum(n) 
     and an array fac[fac_mac=10] to store fac(n) (factorial !)
     with those arrays, we only need to calculate fac(n),sum(n) one time*/
  sig_init();  
  fac_init();

  /* Start searching*/
  search();
}


/*Global variables used in function search*/
char a[STR_LEN]; 		// hold prefix expressions
char b[STR_LEN]; 		// hold infix expression
char result[RANGE][STR_LEN];	// hold infix expressions satisfying the problem.
int l;				// l belonging to [0,RANGE) is index of table result[RANGE][STR_LEN]
double ans;			// hold the result of each prefix expression
int n_ans=0; 			// number of answers (when n_ans=RANGE, the process will finish right away.

/* Suplementary functions of function search*/
void init_result(char result[][STR_LEN],int range);     // init the table result.
void generate(char *a);                                 // generate as much as possible pre-fix expression from string a
int check_ans(double l);				// check if result of a pre-fix expression is valid or not.



/* First we generate 5x6^3=1080 pre-fix expression
   by shuffling possition of three Kind2 operators.
   Then by generating each of the above pre-fix expression ,
   we can produce 
   1080x4^7=17694720
   different prefix expression. Because we use filter to
   terminate calculating process, so only a small part of
   them will be evaluated.
*/
void search()
{
  int i,j,k;                                            // three different positions need three counters
  init_result(result,RANGE);				// init the result table
  for(i = 0;i < OP_MAX;i++){
    for(j = 0;j < OP_MAX;j++){
      for(k = 0;k < OP_MAX;k++){
	sprintf(a,"44%c4%c4%c\0",op[i],op[j],op[k]); 	// create pre-fix an expression
	generate(a);					// generate as much as possible prefix expression from the above expression
	sprintf(a,"44%c4%c4%c\0",op[i],op[j],op[k]); 	// above process is repeated 4 more times.
	generate(a);
	sprintf(a,"44%c44%c%c\0",op[i],op[j],op[k]);
	generate(a);
	sprintf(a,"444%c%c4%c\0",op[i],op[j],op[k]);
	generate(a);
	sprintf(a,"444%c4%c%c\0",op[i],op[j],op[k]);
	generate(a);
	sprintf(a,"4444%c%c%c\0",op[i],op[j],op[k]);
	generate(a);
      }
    }
  }
  /* Print the result*/
  for(i=0;i<RANGE;i++){
    printf("%d = ",i);
    printf("%s\n",result[i]);
  }
}

/* Make all element of table result[][] an empty string*/
void init_result(char result[][STR_LEN],int range){
  int i;
  for(i=0; i<range;++i)
    result[i][0]='\0';
}

/* Check the ans*/
int check_ans(double ans ){
  if(!is_integer(ans)) return 1;	// if ans is not integer or (this function is included in cal_prefix.h
  if(ans<0 || ans>=RANGE) return 1;     // if ans does not belong to [0,RANGE) or
  if(result[(int)ans][0]!='\0') return 1;      // if an expression that produces ans is already found return 1
  return 0;                        	// otherwise, return 0
}

/* Generate function
   For instance: a=44$4$4$, in which each $ stands for an operator of kind 2 (which requires two arguments)
   Now we put an operator of kind 1 (which requires 1 argument) into every interval between two characters of string a
   and into the last symbol
   a1=4#4#$#4#$#4#$#
   By doing that, we can generate 4^7=16384 different expressions from string a.
   Off course, not all of them will be calculated due to filtering process in function cal_prefix
*/

void generate(char *a){
  int i0,i1,i2,i3,i4,i5,i6;                  // there are 7 different positions, so 
  char a1[STR_LEN];

  for(i0=0;i0<OP_MAX1; ++i0){
    for(i1=0;i1<OP_MAX1; ++i1){
      for(i2=0;i2<OP_MAX1; ++i2){
	for(i3=0;i3<OP_MAX1; ++i3){
	  for(i4=0;i4<OP_MAX1; ++i4){
	    for(i5=0;i5<OP_MAX1; ++i5){
	      for(i6=0;i6<OP_MAX1; ++i6){
		if(n_ans==RANGE) return;
		// create pre-fix expression
		sprintf(a1,"%c%s%c%s%c%s%c%s%c%s%c%s%c%s",a[0],op1[i0],a[1],op1[i1],a[2],op1[i2], a[3],op1[i3],a[4],op1[i4],a[5],op1[i5],a[6],op1[i6]); 
		// calculating
		ans = cal_prefix(a1,35);                       
		// check the answers
		if(!check_ans(ans)){
		  ++n_ans;                        // increment n_ans
		  to_infix(a1,35,b);              // convert to infix expression
		  strcpy(result[(int)ans],b);     // keep valid expression into result table
		}
	      }
	    }
	  }
	}
      }
    }
  }
}	     
