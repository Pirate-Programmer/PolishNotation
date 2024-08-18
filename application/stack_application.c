#include "stack.h"
#include<unistd.h>
#include<string.h>


char valid_expression_characters[] = "()0123456789+-*/~^ ";



char** infix_to_prefix(const char* const ptr);
char** infix_to_postfix(const char * const ptr);
int get_precedence(char operator);
void free_result_array(char** , int);
void print_expression(char**);



int main(int argc, char* argv[])
{
     if(argc < 4)
    {
        printf("Usage: ./[path/file_name].exe -f [\"expression\"] -a/-p/-ap\n");
        return 1;
    }
    //Parsing Cmd Line Arguments
    int opt;
    bool a_flag = false, p_flag = false;
    char* expression;
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

    //Parse the Expression
    //check if the expression is sematically correct ie contains only the req characters
    if(strspn(expression, valid_expression_characters) < strlen(expression))
    {
        printf("Invalid Expression\n");
        return 2;
    }

    //iterate expression character by character and perform req operation ie pop, pop, print or store.
    
   char** postfix =  infix_to_postfix(expression);
   char** prefix = infix_to_prefix(expression);

   print_expression(postfix);
   print_expression(prefix);

   free_result_array(prefix,strlen(expression));
   free_result_array(postfix,strlen(expression));
}





//function defns

char** infix_to_postfix(const char * const ptr)
{
    int len = 0,count = 0,num = 0;
    bool number_checker = false;

    char **ans = (char**) calloc(strlen(ptr) + 1,sizeof(char*));
    Stack* postfix = intialize_stack(strlen(ptr));
    if(ans == NULL || postfix == NULL)
    {
        return NULL;
    }

    for(int i = 0; i < strlen(ptr); i++)
    {
        char* temp = (char* )malloc(sizeof(char)*5);
        if(temp == NULL)
        {
            return NULL;
        }
        ans[i] = temp;
    }

    for(char chr = ptr[len]; chr != '\0';chr = ptr[++len])
    {
        if(isspace(chr) != 0) //skip whitespace
        {
            continue;
        }
        else if(isalnum(chr) != 0) //print operand in stdout
        {
           number_checker = true;
           num *= 10;
           num = num + atoi(&chr);
        }
        else  // chr is a operator
        {
            if(number_checker)
            {
                sprintf(ans[count++],"%i",num);
                num = 0;
                number_checker = false;
            }

            if(chr == '(')
            {
                push(postfix,chr);
            }
            else if(chr == ')')  //pop till first open parenthesis is found
            {

                //***NOTE: This Expects a opening bracket is in the stack
                //POINT OF FAILURE WILL BREAK IF BRACKET NOT PRESENT NEED TO ADJUST FOR THAT
                //also expression not valid if open bracket present but no closing bracket viceversa

                for(char object = pop(postfix); object != '(' && !isEmpty(postfix); object = pop(postfix))
                {
                    sprintf(ans[count++],"%c",object);
                }
            }
            else  //push coperator
            {
                //get_precedence == -1 ie '('  push in stack
                //get_precedence(chr) > get_predence(operator) ex: operator: '-' & chr: '*' ; push chr
                //else pop stack
                //if stack is empty peek will return  char_max and get_precedence will return -1

                while(get_precedence(chr) <= get_precedence(peek(postfix)))
                {
                    sprintf(ans[count++],"%c",pop(postfix));
                }
                push(postfix,chr);
            }
        }
    }
    if(number_checker)
    {
        sprintf(ans[count++],"%i",num);
        num = 0;
    }
  
    while(!isEmpty(postfix))
    {
        sprintf(ans[count++],"%c",pop(postfix));
    }
    ans[count] = '\0';

    //free stack memory
    freeStack(postfix);
    return ans;
}



char** infix_to_prefix(const char * const ptr)
{
    int len = strlen(ptr) - 1,count = 0,num = 0;
    bool number_checker = false;

    Stack* prefix = intialize_stack(strlen(ptr)); //prefix stack used to convert infix to prefix
    char** output = (char**)calloc(strlen(ptr) + 2,sizeof(char*));
    if(prefix == NULL || output == NULL)
    {
        return NULL;
    }

    //setting up array of pointers
    for(int i = 0; i < strlen(ptr); i++)
    {
        char* temp = (char* )malloc(sizeof(char)*5);
        if(temp == NULL)
        {
            return NULL;
        }
        output[i] = temp;
    }
    

     for(char chr = ptr[len]; len >= 0 ;chr = ptr[--len])
    {
        if(isspace(chr) != 0) //skip whitespace
        {
            continue;
        }
        else if(isalnum(chr) != 0)
        {
           number_checker = true;
           num *= 10;
           num = num + atoi(&chr);
        }
        else  // chr is a operator
        {
            if(number_checker)
            {
                int temp = num;
                num = 0;
                while(temp)
                {
                    num = num*10 + temp%10;
                    temp /= 10;
                }
                sprintf(output[count++],"%i",num);
                num = 0;
                number_checker = false;
            }

            if(chr == ')')
            {
                push(prefix,chr);
            }
            else if(chr == '(')  //pop till first closing parenthesis is found
            {
                //***NOTE: This Expects a opening bracket is in the stack
                //POINT OF FAILURE WILL BREAK IF BRACKET NOT PRESENT NEED TO ADJUST FOR THAT
                //also expression not valid if open bracket present but no closing bracket viceversa

                for(char object = pop(prefix); object != ')' && !isEmpty(prefix); object = pop(prefix))
                {
                    sprintf(output[count++],"%c",object);
                }
            }
            else  //push coperator
            {

                //get_precedence == -1 ie ')'  push in stack
                //get_precedence(chr) < get_predence(operator) ex: operator: '-' & chr: '*' ; push chr
                //if stack is empty peek will return  char_max and get_precedence will return -1
                while(get_precedence(chr) < get_precedence(peek(prefix)))
                {
                    sprintf(output[count++],"%c",pop(prefix));
                }
                push(prefix,chr);
            }
        }
    }
    if(number_checker)
    {
        int temp = num;
        num = 0;
        while(temp)
        {
            num = num*10 + temp%10;
            temp /= 10;
        }
        sprintf(output[count++],"%i",num);
        num = 0;
    }

    while(!isEmpty(prefix))
    {
        sprintf(output[count++],"%c",pop(prefix));
    }
    output[count] = '\0';
    //reverse the array for prefix
    for(int i = 0; i < count/2; i++)
    {
        char* temp = output[i];
        output[i] = output[count - i - 1];
        output[count - i - 1] = temp;
    }

    //free stack memory
    freeStack(prefix);
    return output;
}


int get_precedence(char operator)
{
    switch (operator)
    {
        case '~':
            return 0;

        case '-':
            return 1; 

         case '+':
            return 1; 

         case '*':
            return 2; 

         case '/':
            return 2; 

         case '^':
            return 3;   
    
         default:  //for brackets
            return -1;
    }
    return -1;
}


void free_result_array(char** ans,int len)
{
    for(int i = 0; i < len; i++)
    {
        free(ans[i]);
    }
    free(ans);
}

void print_expression(char** expression)
{
    int i = 0;
    while(expression[i] != '\0')
    {
        printf("%s ",expression[i++]);
    }
    printf("\n");
}
