#include <stdio.h>
#include "engine.h"

int main()
{
  int conti=1;
  state_t *start;
  stack_t s; 
  char *temp; 
  init_stack(&s);
  int res=0;
  char *res1=0;
  char string[100],postfix[100],string1[100];
  printf("Enter Regular expression\n");
  scanf("%s",string);
  
  
  res=choice(string);
  removed(string,postfix,&s);
  printf("Postfix::%s",postfix);
  start=postfixtonfa(postfix);
  
  while(conti==1)
  {  
     res1=0;
     printf("\nEnter string\n");
     scanf("%s",string1);
     temp=string1;
  
  
     switch(res)
     {          
             case COMPLETE:res1=match2(start,temp);
                           break;
             case START:res1=match1(start,temp);
                        break;
             case END:while(*(temp)!='\0' && res1==0)
                        {
                                        res1=match2(start,temp++);
                        }                       
                        break;
             case ANY:
                      while(*(temp)!='\0' && res1==0)
                        {
                                        
                                        res1=match1(start,temp++);
                        }
                        break;
     }
  
     if(res1)
     {
             printf("\nMatched String::");
             char *te=temp-1;
             while(te!=res1)
             {
                            printf("%c",*te);
                            te++;
             }
             printf("\n");
     
     }
     else
     printf("\nThe String Doesnt Match The Regular Expression!!!\n");
     printf("Do You Want to Contine??If yes,press 1.....\n");
     scanf("%d",&conti);
  } 
  printf("Press any key!!!\n");
  
  getch();
}
