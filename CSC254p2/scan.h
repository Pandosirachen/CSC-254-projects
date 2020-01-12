
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <stdio.h>

char token_image[100];
typedef enum {
    t_read,
    t_write,
    t_id,
    t_literal,
    t_gets,
    t_add,
    t_sub,
    t_mul,
    t_div,
    t_lparen,
    t_rparen,
    t_eof,
    t_if,
    t_end,
    t_while,
    t_equal,
    t_notequal,
    t_lessthan,
    t_largerthan,
    t_lessequalthan,
    t_largerequalthan,
    t_procedure,
    t_addOp,
    t_mulOp,
    t_relop,
    t_expr,
    t_cond,
    t_starter} token;

extern char token_image[100];

extern token scan();
bool runAfter;

token scan() {
    runAfter=false;
    static int c = ' ';
    /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */
    
    /* skip white space */
    while (isspace(c))
    {
        c = getchar();
    }
    
    if(c == '$')
    {
        runAfter = true;
        return t_eof;
    }
    
    else if(c == EOF)
    {
        return t_eof;
    }

    if (isalpha(c))
    {
        do {
            token_image[i++] = c;
            if (i >= 100)
            {
                std::cout<<"max token length exceeded\n";
                exit(1);
            }
            c = getchar();
        }
        while (isalpha(c) || isdigit(c) || c == '_');
        token_image[i] = '\0';
        if (!strcmp(token_image, "read")) return t_read;
        else if (!strcmp(token_image, "write")) return t_write;
        else if (!strcmp(token_image, "if")) return t_if;
        else if (!strcmp(token_image, "while")) return t_while;
        else if (!strcmp(token_image, "end")) return t_end;
        else return t_id;
    }
    
    
    
    else if (isdigit(c))
    {
        do {
            token_image[i++] = c;
            c = getchar();
        } while (isdigit(c));
        token_image[i] = '\0';
        return t_literal;
    }
    
    else switch (c) {
        case ':':
            if ((c = getchar()) != '=') {
                std::cout<<"Syntax error: operator error\n";
                exit(1);
            } else {
                c = getchar();
                return t_gets;
            }
            break;
        case '+': c = getchar(); return t_add;
        case '-': c = getchar(); return t_sub;
        case '*': c = getchar(); return t_mul;
        case '/': c = getchar(); return t_div;
        case '(': c = getchar(); return t_lparen;
        case ')': c = getchar(); return t_rparen;
            
        case '=':
            if ((c = getchar()) != '=')
            {
                std::cout<<"Syntax error: operator error\n";
                exit(1);
            }
            else
            {
                c = getchar();
                return t_equal;
            }
            break;
       
        case '!':
            if ((c = getchar()) != '=')
            {
                std::cout<< "Syntax error: operator error\n";
                exit(1);
            }
            else
            {
                c = getchar();
                return t_notequal;
            }
            break;
        
        case '>':
            c = getchar();
            if(isspace(c))
            {
                return t_largerthan;
            }
            else if((c) != '=')
            {
                std::cout<<"Syntax Error: operator error\n";
                exit(1);
            }
            else
            {
                c = getchar();
                return t_largerequalthan;
            }
            break;
        
        case '<':
            c = getchar();
            if(isspace(c))
            {
                return t_lessthan;
            }
            else if((c) != '=')
            {
                std::cout<<"Syntax error: operator error\n";
                exit(1);
            }
            else
            {
                c = getchar();
                return t_lessequalthan;
            }
            break;
            
            
        default:
            std::cout << "Syntax error: no matching syntax\n";
            exit(1);
    }
}
