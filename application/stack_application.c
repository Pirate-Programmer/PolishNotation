#include "stack.h"
#include<unistd.h>
#include<string.h>

char expression_honour_system[] = "(){}[]0123456789+-*/^~ ";
char operators[] = "-+*/^";
//char valid_expression[] = "(){}[]abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRTSUVWXYZ0123456789+-*/~^ ";


void infix_to_postfix(const char * const ptr);

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
                break;

            defaut:
                printf("Invalid Flag --%c\n",optopt);
        }
    }

    //Parse the Expression

    //check if the expression is sematically correct ie contains only the req characters
    if(strspn(expression, expression_honour_system) < strlen(expression))
    {
        printf("Invalid Expression\n");
        return 2;
    }

    //iterate expression character by character and perform req operation ie pop, pop, print or store.
    
    infix_to_postfix(expression);
}

void infix_to_postfix(const char * const ptr)
{
    int len = 0;
    char* operand = (char* )malloc(5);
    strcpy(operand,"");

    Stack* postfix = intialize_stack(strlen(ptr));

    printf("Expression: ");

     for(char chr = ptr[len]; chr != '\0';chr = ptr[++len])
    {
        if(isspace(chr) != 0) //skip whitespace
        {
            continue;
        }
        else if(isalnum(chr) != 0) //store number/alpha in operand
        {
            strncat(operand,&chr,1);
        }
        else  // chr is a operator
        {
            printf("%s ",operand); //print the operand
            strcpy(operand,""); //reset the operand

            //check operator precedence if another operator at top of stack
            char operator = peek(postfix);
            if(strspn(&operator,operator_precedence))
        }
    }
    printf("%s",operand); //print the last operand
    free(operand);
    freeStack(postfix);
}
