#include "Constants.h"
#include "Lexer.h"

//base node class that contains appropraite methods for all nodes
class Node{

};

//node subclasses
//Number Node class
class NumberNode: public Node{
    Token tok;

    public:
    //default constructor
    NumberNode(){}

    //constructor
    // NumberNode(Token &token, int start, int end): Node(start, end){
    //     tok = token;
    // }

    NumberNode(Token& token){
        tok = token;
        std::cout << "Number Node Init " << tok.get_type() << '\n';
    }
    
    string str(){
        string s = tok.str();
        return s;
    }
};

//Operation Node for two values
class BinOpNode: public Node{
    NumberNode left_node;
    Token op_tok;
    NumberNode right_node;
    BinOpNode* lefty;
    bool op_flag = false;

    public:
    //default constructor
    BinOpNode(){}

    //constructor
    BinOpNode(NumberNode& left, NumberNode& right, Token& op_tok_){
        left_node = left;
        right_node = right;
        op_tok = op_tok_;
        op_flag = true;
    }

    //if there is no binary op and its just a number
    BinOpNode(NumberNode& left){
        left_node = left;
    }

    string str(){
        string s;
        if (op_flag){
            s = "(" + left_node.str() + ", " + op_tok.str() + ", " + right_node.str() + ")";  
        }
        else{
            s = "(" + left_node.str() + ")";
        }
        return s;   
        
    }

};
//parser

struct FinNode{
    BinOpNode left;
    BinOpNode right;
    Token op_tok;
    bool op_flag = false;

    FinNode(){};

    FinNode(BinOpNode& left_, BinOpNode& right_, Token& op_tok_){
        left = left_;
        right = right_;
        op_tok = op_tok_;
        op_flag = true;
    }

    FinNode(BinOpNode& left_){
        left = left_;
    }

    string str(){
        string s;
        if (op_flag){
            s = "(" + left.str() + ", " + op_tok.str() + ", " + right.str() + ")";
        }
        else{
            s = "(" + left.str() + ")";
        }
        return s;  
    }
};

struct ReFin{
    FinNode left;
    BinOpNode right;
    Token op_tok;
    bool op_flag = false;

    ReFin(FinNode& left_, BinOpNode& right_, Token& op_tok_){
        left = left_;
        right = right_;
        op_tok = op_tok_;
        op_flag = true;
    }

    ReFin(BinOpNode& left_){
        left = left_;
    }

    string str(){
        string s;
        if (op_flag){
            s = "(" + left.str() + ", " + op_tok.str() + ", " + right.str() + ")";
        }
        else{
            s = "(" + left.str() + ")";
        }
        return s;  
    }
};

//Parser class -> generates AST
class Parser{
    std::vector<Token> tokens;
    int tok_idx = -1;
    Token* current_tok;

    public:
    //default constructor
    Parser(){}

    //constructor
    Parser(std::vector<Token> &tokens_){
        tokens = tokens_;
        advance();
    }

    //advance method -> moves to next token to be parsed
    void advance(){
        tok_idx += 1;
        if (tok_idx < tokens.size()){
            current_tok = &tokens.at(tok_idx);
            std::cout << "advance" << '\n';
            std::cout << "type: " << (*current_tok).get_type() << '\n';
        }
    }

    FinNode parse(){
        FinNode res = expression();
        return res;
    }

    //Generates Factors -> finds a number node that is either an int or float and returns it
    NumberNode factor(){
        if ((*current_tok).get_type() == TT_INT || (*current_tok).get_type() == TT_FLOAT){
            NumberNode node = NumberNode((*current_tok));
            advance();
            return node;
        }
        return NumberNode((*current_tok));
    }

    //Generates Terms
    BinOpNode term(){
        NumberNode left = factor();
        NumberNode right;
        Token op_tok;
        bool flag = false;
        while ((*current_tok).get_type() == "MUL" || (*current_tok).get_type() == "DIV"){
            //get
            op_tok = (*current_tok);
            flag = true;
            advance();
            right = factor();
        }

        if (flag){
            return BinOpNode(left, right, op_tok);
        }

        return BinOpNode(left);
    }

    //Generates Expressions
    FinNode expression(){
        BinOpNode left = term();
        std::cout << "Expression start" << '\n';
        BinOpNode right;
        Token op_tok;
        bool flag;

        while ((*current_tok).get_type() == "PLUS" || (*current_tok).get_type() == "MINUS"){
            //get
            std::cout << "Expression If" << '\n';
            op_tok = (*current_tok);
            flag = true;
            advance();
            right = term();
        }
        
        if (flag){
            return FinNode(left, right, op_tok);
        }
        
        return FinNode(left);
    }
};