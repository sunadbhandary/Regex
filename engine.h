#include<stdio.h>
#define LP 10
#define RP 20
#define OPERATOR 30
#define OPERAND 40
#define SPLIT 256
#define FINAL 257
// Left parentheses precedence. Minimum of all
#define LPP 0

// Addition  precedence. Minimum among all operator precedence
#define PLUS 1 


// Multiplication  precedence.
#define DOT 2


// Star precedence.
#define STAR 3

#define NONE 9

#define N 100

#define START 0
#define ANY 1
#define COMPLETE 2
#define END 3
struct state//each state comprises of two pointers and an integer where integer signigies split,normal state,final state
{
       int c;
	   struct state *out;
       struct state *out1;
       int id;
       int visit;
};

typedef struct state state_t;
struct ptrlist//list of dangling ponters
{
       state_t **ptr;
       struct ptrlist *link;
};

typedef struct ptrlist ptrlist_t;

struct frag//fragment consist of multiple states and is signified by a starting state and list of dangling pointers
{
       state_t *start;
       ptrlist_t *out;
};

typedef struct frag frag_t;


struct stack
{
       char list[N];
       int top;
};

typedef struct stack stack_t;
struct state_list
{
       state_t *ptr;
       struct state_list *link;
};
typedef struct state_list state_list_t;

state_t * postfixtonfa(char *);

void init_stack(stack_t *);
void infixtopostfix(stack_t *,char *,char *);     /** POSTFIX CONVERSION FUNCTION **/
int gettype(char);             /** TYPE OF EXPRESSION GENERATOR **/
void push(stack_t *,char);               /** PUSH FUNCTION **/
char pop(stack_t *);                /** POP FUNCTION **/
int getprec(char);             /** PRECEDENCE CHECKER FUNCTION **/
ptrlist_t * list1(state_t **);
ptrlist_t * append(ptrlist_t *,ptrlist_t *);
void patch(ptrlist_t *,state_t *);
void pushf(frag_t *stack[],frag_t *temp,int *top);
frag_t * popf(frag_t *stack[],int *top);
void add_node(state_list_t **,state_t *);
char * match2(state_t *,char *);
char * match1(state_t *,char *);
state_list_t * find(state_t *,char *);
void display(state_t *,int a[][]);
void convertinfix(stack_t *,char *,char *);
void removed(char *,char *,stack_t *);
