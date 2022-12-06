#include "Constants.h"
#include "Lexer.h"

//Default Node that is inherited by all other nodes -> contains position
class Node{
    Position pos_start;
    Position pos_end;

    public:
    //default constructor
    Node(){}

    //constructor
    Node(Position start, Position end){
        pos_start = start;
        pos_end = end;
    }


    //getters
    Position get_start(){
        return pos_start;
    }

    Position get_end(){
        return pos_end;
    }
};

//Number Node class
class NumberNode: public Node{
    Token tok;

    public:
    //default constructor
    NumberNode(){}

    //constructor
    NumberNode(Token token, Position start, Position end): Node(start, end){
        tok = token;
    }
};

//Operation Node for two values
class BinOpNode: public Node{
    Node left_node;
    Token op_tok;
    Node right_node;

    public:
    //default constructor
    BinOpNode(){}

    //constructor
    BinOpNode(NumberNode left, NumberNode right, Token op_tok_): Node(left.get_start(), right.get_end()){
        left_node = left;
        right_node = right;
        op_tok = op_tok_;
    }
};



//FIX THIS BELOW -> NON NULL INITIALIZATION COULD PRODUCE UNWANTED BEHAVIORS

//Parse Result Class -> keeps track of status parser
class ParseResult{
    Error error; 
    Node node;

    public:
    //default constructor

    void reg(){

    }
    
    void success(){

    }

    void failure(){

    }
};


//Parser class -> generates AST
class Parser{
    std::vector<Token> tokens;
    int tok_idx = -1;
    Token current_tok;

    public:
    //default constructor
    Parser(){}

    //constructor
    Parser(std::vector<Token> tokens_){
        tokens = tokens_;
        advance();
    }

    //advance method -> moves to next token to be parsed
    Token advance(){
        tok_idx += 1;
        if (tok_idx < tokens.size()){
            current_tok = tokens.at(tok_idx);
        }

        return current_tok;
    }

    void parse(){
        
    }

    //Generates Factors
    void factor(){
    }

    //Generates Terms
    void term(){

    }

    //Generates Expressions
    void expression(){
        
    }

    void binary_operation(){

    }
};