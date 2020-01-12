#include <iostream>
#include <cstdlib>
#include <exception>
#include "node.h"

const string names[] = {"read","write","id","literal",":=","+","-","*","/","(",")","eof","if","end","while","==","!=","<",">","<=",">=","procedure","add operator","multiply operator","relational operator","expression","condition","starter"};

static token input_token;
string old_image = ""; //Used to save ID's for after match has scanned for next token
bool outputTree = true;//Output syntax tree
long int numOfToken = 0; //Number of tokens. Used for error reporting

void error (token expected) {
    outputTree = false;
    throw "Syntax error: expected \""  + names[expected] + "\" at Token #: " + to_string(numOfToken);
        return;
}

void match (token expected) {
    if (input_token == expected)
    {
        if(input_token == t_id || input_token == t_literal)
        {
        }
        old_image = token_image;
        numOfToken++;
        if(input_token != t_eof)
        {
            input_token = scan ();
        }
    }
    else
    {
        error(expected);
    }
}


Node* program ();
Node* stmt_list ();
Node* stmt ();
Node* expr ();
Node* cond ();
Node* term_tail ();
Node* term ();
Node* factor_tail ();
Node* factor ();
string rel_op ();
string add_op ();
string mul_op ();
void printTree(Node*);
bool first(string);
bool follow(string);

Node* program () {
    Node* tmp;
    try
    {
        switch (input_token) {
            case t_id:
            case t_read:
            case t_write:
            case t_if:
            case t_while:
            case t_eof:
                tmp = stmt_list ();
                match (t_eof);
                return tmp;
            default:
                error (t_starter);
        }
    }
    catch(const string msg){
        cout << msg << endl;
        while(1){
            numOfToken++;
            input_token = scan();
            if(first("program"))
            {
                return program();
            }
            else
            {
                numOfToken++;
                input_token = scan();
            }
        }
    }
    return NULL;
}

Node* stmt_list () {
    Node* tmp;
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
            tmp = stmt ();
            tmp->child[3] = stmt_list ();
            return tmp;
        case t_eof:
        case t_end:
            return NULL;
        default: error (t_starter);
            return NULL;
    }
}

Node* stmt () {
    string id = "";
    Node* tmp; //node to be returned
    Node* rhs; //right hand side of statement
    try
    {
        switch (input_token)
        {
            case t_id:
                match (t_id);
                match (t_gets);
                id = old_image;
                rhs = expr();
                tmp = new Node(":=", 0, t_procedure);
                tmp->child[1] = rhs;
                tmp->child[0] = new Node(id, 0, t_id);
                return tmp;
            case t_read:
                match (t_read);
                match (t_id);
                tmp = new Node("read", 0, t_procedure);
                tmp->child[0] = new Node(old_image, 0, t_id);
                return tmp;
            case t_write:
                match (t_write);
                tmp = new Node("write", 0, t_procedure);
                tmp->child[0] = expr();
                return tmp;
            case t_if:
                match(t_if);
                tmp = new Node("if", 0, t_procedure);
                tmp->child[0] = cond();
                tmp->child[1] = stmt_list();
                match(t_end);
                return tmp;
            case t_while:
                match(t_while);
                tmp = new Node("while", 0, t_procedure);
                tmp->child[0] = cond();
                tmp->child[1] = stmt_list();
                match(t_end);
                return tmp;
            default: error (t_starter);
                return NULL;
        }
    }
    catch (const string msg)
    {
        cout << msg << endl;
        while(1)
        {
            numOfToken++;
            input_token = scan();
            if(first("stmt"))
            {
                return stmt();
            }
            else if (follow("stmt"))
            {
                return NULL;
            }
            else
            {
                numOfToken++;
                input_token = scan();
            }
        }
    }
    return NULL;
}

Node* cond () {
    Node* lhs; //left hand side of cond
    Node* rhs; //right hand side of cond
    Node* tmp; //Node to be returned
    string ro = "";
    try{
        switch(input_token) {
            case t_lparen:
            case t_id:
            case t_literal:
                lhs = expr();
                ro = rel_op();
                rhs = expr();
                if(ro == "<")
                    tmp = new Node(ro, 0, t_lessthan);
                if(ro == ">")
                    tmp = new Node(ro, 0, t_largerthan);
                if(ro == "<=")
                    tmp = new Node(ro, 0, t_lessequalthan);
                if(ro == ">=")
                    tmp = new Node(ro, 0, t_largerequalthan);
                if(ro == "==")
                    tmp = new Node(ro, 0, t_equal);
                if(ro == "!=")
                    tmp = new Node(ro, 0, t_notequal);
                tmp->setChildren(lhs, rhs, 0, 0);
                return tmp;
            default: error (t_cond);
                return NULL;
        }
    }
    catch (const string msg)
    {
        cout << msg << endl;
        while(1)
        {
            numOfToken++;
            input_token = scan();
            if(first("cond"))
            {
                return cond();
            }
            else if (follow("cond"))
            {
                return NULL;
            }
            else
            {
                if (input_token == t_eof)
                {
                    exit(1);
                }
                numOfToken++;
                input_token = scan();
            }
        }
    }
}

Node* expr () {
    Node* lhs; //left hand side of expression
    Node* rhs; // right hand side of expression
    try
    {
        switch (input_token)
        {
            case t_id:
            case t_literal:
            case t_lparen:
                lhs = term ();
                rhs = term_tail ();
                if(rhs != NULL)
                { //In case rhs evaulated to epsilon
                    rhs->child[0] = lhs;
                    return rhs;
                }
                else
                    return lhs;
            default: error (t_expr);
                return NULL;
        }
    }
    catch (const string msg)
    {
        cout << msg << endl;
        while(true)
        {
            numOfToken++;
            input_token = scan();
            if(first("expr"))
            {
                //trying again with next token
                return expr();
            }
            else if (follow("expr"))
            {
                return NULL; //meaning we've parsed all available expr"s and can return
            }
            else
            {
                numOfToken++;
                input_token = scan(); //get next_token
            }
        }
    }
}

Node* term_tail () {
    string op; // add operator
    Node* rhs; //right hand side of term_tail
    Node* lhs; //left hand side of term_tail
    Node* tmp; //Node to be returned
    switch (input_token) {
        case t_add:
        case t_sub:
            op = add_op ();
            lhs = term();
            rhs = term_tail();
            if(op == "+")
                tmp = new Node(op,0, t_add);
            if(op == "-")
                tmp = new Node(op,0, t_sub);
            tmp->child[1] = lhs;
            tmp->child[2] = rhs;
            return tmp;
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_lessthan:
        case t_largerthan:
        case t_lessequalthan:
        case t_largerequalthan:
        case t_notequal:
        case t_equal:
        case t_end:
        case t_eof:
            return NULL;         /*  epsilon production */
        default: error (t_addOp);
            return NULL;
    }
}

Node* term () {
    Node* lhs; //left hand side of term
    Node* rhs; //right hand side of term
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            lhs = factor ();
            rhs = factor_tail ();
            if(rhs != NULL){ //In case right hand side evaluates to epsilon
                rhs->child[0] = lhs;
                return rhs;
            }
            else
                return lhs;
        default: error (t_id);
            return NULL;
    }
}

Node* factor_tail () {
    string op = "";
    Node* lhs;
    Node* rhs; //right hand side of factor_tail
    Node* tmp; // node to be returned
    switch (input_token) {
        case t_mul:
        case t_div:
            op += mul_op ();
            lhs = factor();
            rhs = factor_tail();
            if(op == "*")
                tmp = new Node(op,0, t_mul);
            if(op == "/")
                tmp = new Node(op,0, t_div);
            tmp->setChildren(0, lhs, rhs, 0);
            return tmp;
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_while:
        case t_lessthan:
        case t_largerthan:
        case t_lessequalthan:
        case t_largerequalthan:
        case t_notequal:
        case t_equal:
        case t_end:
        case t_eof:
            return NULL;         /*  epsilon production */
        default: error (t_mulOp);
            return NULL;
    }
}

Node* factor () {
    Node* tmp; 
    switch (input_token) {
        case t_id :
            match (t_id);
            return new Node(old_image, 0, t_id);
        case t_literal:
            match (t_literal);
            return new Node(old_image, 0, t_literal);
        case t_lparen:
            match (t_lparen);
            tmp = expr();
            match (t_rparen);
            return tmp;
        default: error (t_id);
            return NULL;
    }
}

string rel_op(){
    switch(input_token) {
        case t_equal:
            match(t_equal);
            return "==";
        case t_notequal:
            match(t_notequal);
            return "!=";
        case t_lessthan:
            match(t_lessthan);
            return "<";
        case t_largerthan:
            match(t_largerthan);
            return ">";
        case t_lessequalthan:
            match(t_lessequalthan);
            return "<=";
        case t_largerequalthan:
            match(t_largerequalthan);
            return ">=";
        default: error (t_relop);
            return NULL;
    }
}

string add_op () {
    switch (input_token) {
        case t_add:
            match (t_add);
            return "+";
        case t_sub:
            match (t_sub);
            return "-";
        default: error (t_addOp);
            return NULL;
    }
}

string mul_op () {
    switch (input_token) {
        case t_mul:
            match (t_mul);
            return "*";
        case t_div:
            match (t_div);
            return "/";
        default: error (t_mulOp);
            return NULL;
    }
}

int main () {
    
    cout << "Syntax Tree Analysis, Input Program:" << endl;
    input_token = scan (); //Get first token
    numOfToken++;
    Node* tree = program();
    if(outputTree)
        printTree(tree);
    cout << endl;
}

//Outputs tree in pre order
void printTree(Node* root){
    if(root == NULL)
    {
        return;
    }
    else
    {
        if(root->child[0] != NULL || root->child[1] != NULL || root->child[2] != NULL || root->child[3] != NULL)
        {
            cout << "( " << root->image << " ";
            printTree(root->child[0]);
            printTree(root->child[1]);
            printTree(root->child[2]);
            printTree(root->child[3]);
            cout << ") ";
        }
        else
        {
            cout << " " << root->image << " ";
        }
    }
}

bool first(string procedure)
{
    if(procedure == "program")
    {
        if( input_token == t_eof || input_token == t_id || input_token == t_read || input_token == t_write || input_token == t_if || input_token == t_while)
            return true;
        else
            return false;
    }
    if(procedure == "stmt")
    {
        if( input_token == t_id || input_token == t_read || input_token == t_write || input_token == t_if || input_token == t_while)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    if(procedure == "cond")
    {
        if( input_token == t_id || input_token == t_literal || input_token == t_lparen)
            return true;
        else
            return false;
    }
    if(procedure == "expr")
    {
        if( input_token == t_id || input_token == t_literal || input_token == t_lparen)
            return true;
        else
            return false;
    }
    return false;
}

bool follow(string procedure)
{
    if(procedure == "stmt")
    {
        if( input_token == t_id || input_token == t_read || input_token == t_write || input_token == t_if || input_token == t_while || input_token == t_end || input_token == t_eof)
            return true;
        else
            return false;
    }
    if(procedure == "cond")
    {
        if( input_token == t_id || input_token == t_read || input_token == t_write || input_token == t_if || input_token == t_while || input_token == t_end)
            return true;
        else
            return false;
    }
    if(procedure == "expr")
    {
        if( input_token == t_equal || input_token == t_notequal || input_token == t_lessthan || input_token == t_largerthan || input_token == t_lessequalthan || input_token == t_largerequalthan || input_token == t_rparen || input_token == t_eof || input_token == t_end)
            return true;
        else
            return false;
    }
    return false;
}
