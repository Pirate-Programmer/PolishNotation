#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<limits.h>
#include<unistd.h>
#include<ctype.h>

typedef struct{
    int top;
    int *values;
    unsigned int capacity;
}Stack;


char valid_expression_characters[] = "()0123456789+-*/~^ ";


Stack* intializeStack(unsigned int capacity);
bool push(Stack * stack, int element);
int pop(Stack* stack);
bool isFull(Stack * stack);
bool isEmpty(Stack * stack);
void freeStack(Stack *);
char peek(Stack* stack);
int eval(char*expression, bool);



int main(int argc, char* argv[])
{
    if(argc != 4)
    {
        printf("Usage: ./[executable_filename] -f [\" expresion \"]  -a/p\n");
        printf("-f for expression -a for postfix and -p for prefix\n");
        return 1;
    }

    int opt;
    char* expression = NULL;
    bool a_flag = false, p_flag = false;
    while((opt = getopt(argc, argv, ":apf:")) != -1)
    {
        switch(opt)
        {
        case 'f':
            expression = optarg;
            break;
 
        case 'a':
            a_flag = true;
             break;

        case 'p':
            p_flag = true;
            break;

        case ':':
            printf("Missing Req Argument for %c\n",optopt);
            exit(EXIT_FAILURE);

        defaut:
            printf("Invalid Flag --%c\n",optopt);
        }
    }
    if(a_flag == p_flag || strspn(expression,valid_expression_characters) < strlen(expression))
        return 3;
    // char expression[] = "- / 10 2 * 3 4";
    // //scanf("%[^'\n']",expression); 

       // printf("expression: %s",expression);
        printf("%i\n",eval(expression,a_flag));
}



int eval(char* expression, bool a_flag)
{
    if(!a_flag)
    {
        for(int i = 0, len = strlen(expression); i < (len)/2; i++)
        {
            char temp = expression[i];
            expression[i] = expression[len- i -1];
            expression[len- i -1] = temp;
        }
        for(int i = 0,end = 0,k = 0; i < strlen(expression);i++)
        {
            if(expression[i] >= '0' && expression[i] <= '9')
            {
                 end = strspn((expression + i),"0123456789") + i -1;
                int idx = i;
                i = end + 1;
                while(idx < end)
                {
                    char temp = expression[idx];
                    expression[idx] = expression[end];
                    expression[end] = temp;
                    end--;
                    idx++;
                }
            }
        }
    }
    bool number_checker = false;
    int firstOperand = 0 , secondOperand = 0;
    Stack* stack = intializeStack(strlen(expression));
    if(stack == NULL)
        return 4;

        int i = 0;
        for(char c = expression[0] ; i < strlen(expression);c = expression[++i])
        {
            if(isspace(expression[i]) != 0)
            {
                if(number_checker)
                {
                    push(stack, firstOperand);
                    number_checker = false;
                    firstOperand = 0;
                }
            }
            else if('0' <= expression[i] && expression[i] <= '9')
            {
                number_checker = true;
                firstOperand *= 10;
                firstOperand += atoi(&c);
            }
            else //operator found
            {
                if(number_checker)
                {
                    push(stack, firstOperand);
                    number_checker = false;
                    firstOperand = 0;
                }

                if(!a_flag)  //prefix
                {
                    firstOperand = pop(stack);
                    secondOperand = pop(stack);
                }
                else //postfix
                {
                    secondOperand = pop(stack);
                    firstOperand = pop(stack);
                }

                switch(expression[i])
                {
                    case '+':
                    {
                        push(stack,firstOperand + secondOperand);
                        break;
                    }
                    case '-':
                    {
                        push(stack,firstOperand - secondOperand);
                        break;
                    }
                    case '*':
                    {
                        push(stack,firstOperand*secondOperand);
                        break;
                    }
                    case '/':
                    {
                        push(stack, (int)firstOperand/secondOperand);
                        break;
                    }
                }
                firstOperand = 0;
            }
        }
    int result = pop(stack);
    if(!isEmpty(stack))
    {
        printf("Invalid Expression\n");
        return INT_MAX;
    }
    freeStack(stack);
    return result;
}




Stack* intializeStack(unsigned int capacity)
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    int* temp = (int*)malloc(sizeof(int)*capacity);

    if(stack == NULL || temp == NULL)
        return NULL;
    
    stack->values = temp;
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

bool push(Stack * stack, int element)
{  
   if(isFull(stack))
        return false;
   stack->values[++stack->top] = element;
   return true;
}

int pop(Stack* stack)
{
  if(isEmpty(stack))
  	    return (int)'\0';
  return stack->values[stack->top--];
}

char peek(Stack* stack)
{
      if(isEmpty(stack))
  	    return (int)'\0';
    return stack->values[stack->top];
}

bool isEmpty(Stack* stack)
{
    if(stack->top < 0)
        return true;
    return false;
}

bool isFull(Stack* stack)
{
    if(stack->top == stack->capacity - 1)
        return  true;
    return false;
}

void freeStack(Stack* stack)
{
    free(stack->values);
    free(stack);
}





