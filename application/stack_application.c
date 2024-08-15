#include "stack.h"
#include<unistd.h>
#include<string.h>

//char expression_honour_system[] = "()0123456789+-*/^~ ";

char valid_expression_characters[] = "(){}[]abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRTSUVWXYZ0123456789+-*/~^ ";

//basically like a key,value pair
//keeps track of precedece of operators
typedef struct operator_precedence
{
 char operator;
 int precedence;
}operator_precedence;

operator_precedence precedence[] = {{'~',0},{'-',1},{'+',1},{'*',2},{'/',2},{'^',3}};

int infix_to_prefix(const char* const ptr);
int infix_to_postfix(const char * const ptr);
int get_precedence(char operator);
int store(char* , int ,Stack* );


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
    
   // infix_to_postfix(expression);
    infix_to_prefix(expression);
}





//function defns

int infix_to_postfix(const char * const ptr)
{
    int len = 0;

    Stack* postfix = intialize_stack(strlen(ptr));
    if(postfix == NULL)
    {
        return 3;
    }

    printf("Expression: ");

     for(char chr = ptr[len]; chr != '\0';chr = ptr[++len])
    {
        if(isspace(chr) != 0) //skip whitespace
        {
            continue;
        }
        else if(isalnum(chr) != 0) //print operand in stdout
        {
            printf("%c",chr);
        }
        else  // chr is a operator
        {
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
                    printf(" "); //whitespace to seperate the operator and operands in stdout
                    printf("%c ",object);
                }
            }
            else  //push coperator
            {
                int flag = 0;
                printf(" "); //whitespace to seperate the operands in stdout

                //get_precedence == -1 ie '('  push in stack
                //get_precedence(chr) > get_predence(operator) ex: operator: '-' & chr: '*' ; push chr
                //if stack is empty peek will return  char_max and get_precedence will return -1
                while(flag != 1)
                {
                    flag = (get_precedence(chr) > get_precedence(peek(postfix)))?push(postfix,chr) : printf("%c ",(pop(postfix)));
                }
            }
        }
    }

        while(!isEmpty(postfix)){
            printf("%c ",pop(postfix));
        }
        printf("\n"); //for looks when make file is executing multiple cases
    
    //free stack memory
    freeStack(postfix);
}



int infix_to_prefix(const char * const ptr)
{
    int len = strlen(ptr) - 1;
    
    int digit_size = 2;
    Stack* prefix = intialize_stack(strlen(ptr)); //prefix stack used to convert infix to prefix
    char* output = (char* )calloc(sizeof(char),strlen(ptr)*2); //contains the prefix output
    
    if(prefix == NULL || output == NULL)
    {
        return 4;
    }

    int output_pointer = 0;
    

    printf("Expression: ");

     for(char chr = ptr[len]; chr != '\0';chr = ptr[--len])
    {
        if(isspace(chr) != 0) //skip whitespace
        {
            continue;
        }
        else if(isalnum(chr) != 0)
        {
            output[output_pointer++] = chr;
        }
        else  // chr is a operator
        {
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
                    output[output_pointer++] = object;
                }
            }
            else  //push coperator
            {
                int flag = 0;

                //get_precedence == -1 ie '('  push in stack
                //get_precedence(chr) > get_predence(operator) ex: operator: '-' & chr: '*' ; push chr
                //if stack is empty peek will return  char_max and get_precedence will return -1
                while(flag != 1)
                {
                    flag = (get_precedence(chr) >= get_precedence(peek(prefix)))?push(prefix,chr) : store(output,output_pointer++,prefix);
                }
            }
        }
    }

        while(!isEmpty(prefix)){
            output[output_pointer++] = pop(prefix);
        }
 

        for(int i = output_pointer - 1; i >= 0; i--)
        {
            printf("%c ",output[i]);
        }
               printf("\n\n"); //for looks when makefile is executing multiple cases
    //free stack memory
    freeStack(prefix);
}


int get_precedence(char operator)
{
    for(int i = 0,len = sizeof(precedence)/sizeof(precedence[0]); i < len; i++)
    {
        if(operator == (precedence+i)->operator)
        {
            return (precedence+i)->precedence;
        }
    }
    return -1; //for opening bracket
}

int store(char* output,int position, Stack* notation)
{
    *(output + position) = pop(notation);
    return 0;
}