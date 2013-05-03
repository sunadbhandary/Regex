#include"engine.h"


void init_stack(stack_t *s)//for infix to postfix
{
     s->top=-1;
}

int gettype(char c)//to know if infix char is operand or operator
{
    int temp;
    switch(c)
    {
    case '(':
         temp=LP;//left paranthesis
         break;
    case ')':
        temp=RP;//right paranthesis
        break;
    case '+':
    
    case '*':
    case '.':
    
        temp=OPERATOR;
        break;
    default :
        temp=OPERAND;
    }
    return temp;
}

void push(stack_t *s,char c)//infix to postfix
{
     if(s->top>N)
    {
        printf("\nStack is full\n");
        exit(0);
    }
    else
        s->list[++s->top]=c;
}
char pop(stack_t *s)//infix to postfix
{
     if(s->top<=-1)
    {
        printf("\nStack is empty\n");
        exit(0);
    }
    else
        return(s->list[s->top--]);
}
int getprec(char c)//for infix to postfix
{
    int temp;
    switch(c)
    {
    case '(':
        temp=LPP;//least precedence
        break;
    case '+':
        temp=PLUS;
        break;
    case '*':
        temp=STAR;
        break;
    case '.':
        temp=DOT;
        break;
    
    default :
        temp=NONE;
    }
    return temp;
}
void infixtopostfix(stack_t *s,char *infix,char *postfix)//as the name suggests
{
    int i,p,l,type,prec;
    char next;
    i=p=0;
    l=strlen(infix);
    while(i<l)
    {
        type=gettype(infix[i]);
        switch(type)
        {
        case LP:
            push(s,infix[i]);
            break;
        case RP:
            while((next=pop(s))!='(')
                postfix[p++]=next;
            break;
        case OPERAND:
            postfix[p++]=infix[i];
            break;
        case OPERATOR:
            prec=getprec(infix[i]);
            while(s->top>-1 && prec <= getprec(s->list[s->top]))
                postfix[p++]=pop(s);
            push(s,infix[i]);
            break;
        }
        i++;
    }
    while(s->top>-1)
        postfix[p++]=pop(s);
    postfix[p]='\0';
    
}
state_t * postfixtonfa(char *postfix)//as the name suggests
{
    int top=-1;
    char *p;
	frag_t *stack[1000], *e1, *e2, *e,*temp;
	state_t *s,*final_state;
    int count=0;
	int a[100][100];
	int k,l;
	
    final_state=(state_t *)malloc(sizeof(state_t));//creating final state
	
	
	for(p=postfix; *p; p++)//see diagrams for each case ull understand
    {
		switch(*p)
        {
			case '.':
	                 e2 = popf(stack,&top);
                     e1 = popf(stack,&top);
                     
                     patch(e1->out, e2->start);
	                 temp=(frag_t *)malloc(sizeof(frag_t));
	                 temp->start=e1->start;
	                 temp->out=e2->out;
	                 pushf(stack,temp,&top);
	                 
	                 break;
            case '+':
	                 e2 = popf(stack,&top);
                     e1 = popf(stack,&top);
	                 s = (state_t *)malloc(sizeof(state_t));
                     s->c=SPLIT;
                     s->out=e1->start;
                     s->out1=e2->start;
                     s->visit=0;
                     s->id=count;
                     count++;
                     temp=(frag_t *)malloc(sizeof(frag_t));
                     temp->start=s;
                     temp->out=append(e1->out,e2->out);
	                 pushf(stack,temp,&top);
	                 
	                 break;
            case '*':
	                 e = popf(stack,&top);
	                 s = (state_t *)malloc(sizeof(state_t));
                     s->c=SPLIT;
                     s->out=e->start;
                     s->out1=0;
                     s->visit=0;
                     s->id=count;
                     count++;
	                 patch(e->out, s);
	                 temp=(frag_t *)malloc(sizeof(frag_t));
	                 temp->start=s;
	                 temp->out=list1(&s->out1);
	                 pushf(stack,temp,&top);
	                 break;
            default:
	                s = (state_t *)malloc(sizeof(state_t));
                    s->c=*p;
                    s->out=0;
                    s->out1=0;
                    s->visit=0;
                    s->id=count;
                    count++;
                    temp=(frag_t *)malloc(sizeof(frag_t));
                    temp->start=s;
                    temp->out=list1(&s->out);
	                pushf(stack,temp,&top);
	                
	                
        }
	}
	
	e = popf(stack,&top);
	
	final_state->c=FINAL;
	final_state->out=0;
	final_state->out1=0;
	final_state->visit=0;
	final_state->id=count;
	count++;
		
	for(k=0;k<count;++k)
	{
            for(l=0;l<count;++l)
                    a[k][l]=0;
    }
    
	patch(e->out,final_state);//joining final state to last fragment
	display(e->start,a);
	printf("\nStart State::%d\n",e->start->id);
	printf("Final State::%d\n",final_state->id);
	
	for(k=0;k<count;++k)
	{
            for(l=0;l<count;++l)
            {
                                printf("%d\t",a[k][l]);
            }
            printf("\n");
    }

	return e->start;
}
ptrlist_t * list1(state_t **outp)//to create a list of dangling pointers of a fragment
{
          ptrlist_t *head=0;
          head=(ptrlist_t *)malloc(sizeof(ptrlist_t));
          head->ptr=outp;
          head->link=0;
          return head;
}
ptrlist_t * append(ptrlist_t *l1,ptrlist_t *l2)//to join pointer list of one fragment to another
{
     ptrlist_t *temp=l1;
     while(temp->link)
     {
                    temp=temp->link;
     }
     temp->link=l2;
     return l1;
}
void patch(ptrlist_t *l,state_t *s)//to make the dangling pointers of one fragment point to a particular state
{
     ptrlist_t *temp=l;
     while(temp!=0)
     {
                         *(temp->ptr)=s;
                         temp=temp->link; 
     }
}    
void pushf(frag_t *stack[],frag_t *temp,int *top)//for stack of fragments
{
     stack[++(*top)]=temp;
}

frag_t * popf(frag_t *stack[],int *top)//fragment stack
{
       return stack[(*top)--];
}
char * match1(state_t *start,char *p)
{
    state_list_t *nlist=0;
    char *res=0;
    if(start->c==FINAL)
    {                  
                          
                       res=p;
    }
    else if(start->c==SPLIT)
    {
         add_node(&nlist,start->out);
         add_node(&nlist,start->out1);
         
         while(nlist && res==0)
         {
                     res=match1(nlist->ptr,p);
                     if(res==0)
                     nlist=nlist->link;
         }
    }
    else
    {
         if(start->c==*p)
         {
                         
                         add_node(&nlist,start->out);
                         while(nlist && res==0)
                         {
                                     
                                     res=match1(nlist->ptr,p+1);                                     
                                     if(res==0)
                                     nlist=nlist->link;
                         }
         }
    }
    return res;
}
char * match2(state_t *start,char *p)
{
    state_list_t *nlist=0;
    char *res=0;
    
    
    if(start->c==FINAL && *p=='\0')
    {
                       res=p;
                       
    }
    else if(start->c==SPLIT)
    {
         add_node(&nlist,start->out);
         add_node(&nlist,start->out1);
         while(nlist && res==0)
         {
                     res=match2(nlist->ptr,p);
                     if(!res)
                     nlist=nlist->link;
         }
    }
    else
    {
         if(start->c==*p)
         {
                         
                         add_node(&nlist,start->out);
                         while(nlist && res==0)
                         {
                                     
                                     res=match2(nlist->ptr,p+1);
                                     if(res==0)
                                               nlist=nlist->link;
                         }
         }
    }
    return res;
}
int choice(char *reg,state_t *start,char *string)
{
     char *temp=reg;
     int res=0;
     
     if(*temp=='$')
     {
                 
                while(*temp!='\0')
                {
                        ++temp;
                }
                if(*(temp-1)=='^')
                {
                      
                      res=COMPLETE;
                      
                }
                else
                {
                     
                      res=START;
                      
                }
     }
     else
     {
         
                
                 while(*temp!='\0')                
                {
                        ++temp;
                }
                if(*(temp-1)=='^')
                {
                        
                        res=END;
                        
                }
                else
                {
                        
                        res=ANY;
                        
                
                }
     }            
     return res;
}          
void removed(char *reg,char *postfix,stack_t *s)
{
     char *temp=reg;
     if(*reg=='$')
     {
                  while(*reg!='\0')
                  {
                                   *reg=*(reg+1);
                                   reg++;
                  }
     }
     reg=temp;
     while(*reg!='\0')
     {
                  reg++;
     }
     if(*(reg-1)=='^')
     {
                      *(reg-1)='\0';
     }
     
     convertinfix(s,temp,postfix);
}   


void add_node(state_list_t **nlist,state_t *s)
{
     state_list_t *temp=(state_list_t *)malloc(sizeof(state_list_t));
     temp->ptr=s;
     temp->link=0;
     if(*nlist==0)
     {                 
                 *nlist=temp;
     }
     else
     {
                 (*nlist)->link=temp;
     }
}           
void display(state_t *s,int a[50][50])
{
     
     
     if(s!=0 && s->visit==0 && s->c!=FINAL)
     {
                   if(s->c==SPLIT)
                   {
                                     a[s->id][s->out->id]=SPLIT;
                                     a[s->id][s->out1->id]=SPLIT;
                   }
                   else 
                   {
                                     a[s->id][s->out->id]=s->c;
                   }
                   s->visit=1;
                   display(s->out,a);
                   display(s->out1,a);       
     
     }
}
void convertinfix(stack_t *s,char *string,char *postfix)
{
     int i=0,j=0;     
     int temp1;
     int temp2;
     char infix[100];
     while(string[i]!='\0')
     {
                            
                            temp1=gettype(string[i]);
                            temp2=gettype(string[i+1]);                            
                            if((temp1==OPERAND && temp2==OPERAND && string[i+1]!='\0')||(temp1=OPERAND && string[i+1]=='(' && string[i+1]!='\0')||(temp2==OPERAND && string[i]==')' && string[i+1]!='\0')||(string[i]=='*' && temp2==OPERAND && string[i+1]!='\0'))
                            {
                                              infix[j++]=string[i];
                                              infix[j++]='.';                                          
                                              
                            }
                            else
                            {                                               
                                              infix[j++]=string[i];        
                                              
                            }
                            ++i;
     }
     infix[j]='\0';
     printf("Infix::%s\n",infix);
     infixtopostfix(s,infix,postfix);
    
}
