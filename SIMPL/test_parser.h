#include "Constants.h"
#include "Lexer.h"

//Default Node that is inherited by all other nodes -> contains position
class Node{
    int pos_start;
    int pos_end;

    public:
    //default constructor
    Node(){}

    //constructor
    Node(int start, int end){
        pos_start = start;
        pos_end = end;
    }


    //getters
    int get_start(){
        return pos_start;
    }

    int get_end(){
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

    public:
    //default constructor
    BinOpNode(){}

    //constructor
    BinOpNode(NumberNode& left, NumberNode& right, Token& op_tok_): Node(left.get_start(), right.get_end()){
        std::cout << "Binop Node Init " << left.str() << right.str() << op_tok.str() << '\n';
        left_node = left;
        right_node = right;
        op_tok = op_tok_;
    }

    string str(){
        string s = "(" + left_node.str() + ", " + op_tok.str() + ", " + right_node.str() + ")";
        return s;     
    }

};



//FIX THIS BELOW -> NON NULL INITIALIZATION COULD PRODUCE UNWANTED BEHAVIORS

struct FinNode{
    BinOpNode left;
    BinOpNode right;
    Token op_tok;

    FinNode(BinOpNode& left_, BinOpNode& right_, Token& op_tok_){
        left = left_;
        right = right_;
        op_tok = op_tok_;
    }

    string str(){
        string s = "(" + left.str() + ", " + op_tok.str() + ", " + right.str() + ")";
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
            std::cout << "advance" << '\n';
            current_tok = &tokens.at(tok_idx);
            std::cout << "type: " << (*current_tok).get_type() << '\n';
        }
    }

    FinNode parse(){
        FinNode res = expression();
        return res;
    }

    //Generates Factors -> finds a number node that is either an int or float and returns it
    NumberNode factor(){
        std::cout << "Factor " << (*current_tok).get_type() << '\n';
        if ((*current_tok).get_type() == TT_INT || (*current_tok).get_type() == TT_FLOAT){
            return NumberNode((*current_tok));
        }
        return NumberNode((*current_tok));
    }

    //Generates Terms
    BinOpNode term(){
        NumberNode left = factor();
        advance();
        NumberNode right;
        Token op_tok;

        //std::cout << "term " << left.str() << '\n';
        std::cout << (*current_tok).get_type() << '\n';
        if ((*current_tok).get_type() == "MUL" || (*current_tok).get_type() == "DIV"){
            std::cout << "term Innerloop" << '\n';
            //get
            op_tok = (*current_tok);
            advance();
            right = factor();
        }
        BinOpNode node = BinOpNode(left, right, op_tok);

        return node;
    }

    //Generates Expressions
    FinNode expression(){
        BinOpNode left = term();
        BinOpNode right;
        Token op_tok;

        std::cout << "expression " << left.str() << '\n';
        if ((*current_tok).get_type() == "PLUS" || (*current_tok).get_type() == "MINUS"){
            std::cout << "expresion Innerloop" << '\n';
            //get
            op_tok = (*current_tok);
            advance();
            right = term();
        }

        return FinNode(left, right, op_tok);
    }
};
