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

    virtual std::string str() = 0;
};

//Number Node class
class NumberNode: public Node{
    Token tok;

    public:
    //default constructor
    NumberNode(){}

    //constructor
    NumberNode(Token token, int start, int end): Node(start, end){
        tok = token;
    }

    NumberNode(Token token){
        tok = token;
    }

    string str(){
        string s = tok.str();
        return s;
    }
    
};

//Operation Node for two values
class BinOpNode: public Node{
    Node *left_node;
    Token op_tok;
    Node *right_node;

    public:
    //default constructor
    BinOpNode(){}

    //constructor
    BinOpNode(Node *left, Node *right, Token op_tok_): Node(){
        left_node = left;
        right_node = right;
        op_tok = op_tok_;
    }

    string str(){
        std::cout << "meow2" << '\n';
        string s1 = (*left_node).str();
        std::cout << s1 << '\n';
        string s2 = op_tok.str();
        std::cout << s2 << '\n';
        string s3 = (*right_node).str();
        std::cout << s3 << '\n';
        string s = "(" + s1 + ", " + s2 + ", " + s3 + ")";
        return s;     
    }

};

class Parser{
    //keeps track of the current token
    Token cur_tok;
    std::vector<Token> tokens;
    int tok_idx = -1;

    public:

    Parser(std::vector<Token> tokens_){
        tokens = tokens_;
        advance();
    }

    Node *parse(){
        return term();
    }

    //advances along input tokens
    void advance(){
        tok_idx += 1;
        if (tok_idx < tokens.size()){
            cur_tok = tokens.at(tok_idx);
            std::cout << cur_tok.str() << '\n';
        }
        else{
            std::cout << "end" << '\n';
        }
    }
    
    //check if the cur_tok is valid
    int valid(Token check_tok){
        if (cur_tok.get_type() == check_tok.get_type()){
            return 1;
        }
        return 0;
    }

    Node *factor(){
        if (valid(TT_FLOAT) || valid((TT_INT))){
            Node *node = new NumberNode(cur_tok, 0 ,0);
            advance();
            return node;
        }
        return (new NumberNode(cur_tok, 0 ,0));
    }

    Node *term(){
        Node *left = factor();
        Node *right;
        Token current_token;

        while (cur_tok.get_type() == TT_DIV || cur_tok.get_type() == TT_MUL){
            Token current_token = cur_tok;
            advance();
            Node *right = factor();
        }
        Node *node = new BinOpNode(left, right, current_token);
        return node;
    }

};