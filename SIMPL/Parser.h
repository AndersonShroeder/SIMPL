#include "Constants.h"
#include "Lexer.h"

//base node class -> contains virtual string representation method
class Node{

    public:
    Node(){};

    virtual string str(){
        string s = "";
        return s;
    };

    virtual float eval(){
        int num = 0;
        return num;
    }
};

//Number Node class
class NumberNode: public Node{
    Token tok;

    public:
    //default constructor
    NumberNode(){}

    NumberNode(Token& token){
        tok = token;
    }
    
    string str(){
        string s = tok.str();
        return s;
    }

    float eval(){
        return tok.get_value();
    }
};

//Operation Node for two values
class BinOpNode: public Node{
    Node* left_node;
    Token op_tok;
    Node* right_node;

    public:
    //default constructor
    BinOpNode(){}

    //constructor
    BinOpNode(Node* left, Node* right, Token& op_tok_){
        left_node = left;
        right_node = right;
        op_tok = op_tok_;
    }

    string str(){
        string s = "(" + (*left_node).str() + ", " + op_tok.str() + ", " + (*right_node).str() + ")";
        return s;   
    }

    float eval(){
        //change to switch/case -> create enum wtih types
        if (op_tok.get_type() == "PLUS"){
            return (*left_node).eval() + (*right_node).eval();
        }
        else if (op_tok.get_type() == "MINUS"){
            return (*left_node).eval() - (*right_node).eval();
        }
        else if (op_tok.get_type() == "DIV"){
            return (*left_node).eval() / (*right_node).eval();
        }
        else if (op_tok.get_type() == "MUL"){
            return (*left_node).eval() * (*right_node).eval();
        }
        else if (op_tok.get_type() == "EXP"){
            return pow((*left_node).eval(), (*right_node).eval());
        }
        //only works for ints - error catch here
        else if (op_tok.get_type() == "MOD"){
            return int((*left_node).eval()) % int((*right_node).eval());
        }
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
        ++(*this);
    }

    //advance method -> moves to next token to be parsed
    void operator ++(){
        tok_idx += 1;
        if (tok_idx < tokens.size()){
            current_tok = &tokens.at(tok_idx);
        }
    }

    Node* parse(){
        Node* res = atom();
        return res;
    }

    //Generates Factors -> finds a number node that is either an int or float and returns it
    Node* factor(){
        if ((*current_tok).get_type() == TT_INT || (*current_tok).get_type() == TT_FLOAT){
            Node *node = new NumberNode((*current_tok));
            ++(*this);;
            return node;
        }
        else if ((*current_tok).get_type() == "LPAREN"){
            ++(*this);
            Node *node2 = atom();
            ++(*this); // accounts for right paren
            return node2;
        }

        return (new NumberNode((*current_tok)));
    }

    //Generates Terms
    Node* term(){
        std::set<string> operands = {"MUL", "DIV"};
        return binary_operation(operands, &Parser::factor);
    }

    //Generates Expressions
    Node* expression(){
        std::set<string> operands = {"PLUS", "MINUS"};
        return binary_operation(operands, &Parser::term);
    }

    Node* atom(){
        std::set<string> operands = {"EXP", "MOD"};
        return binary_operation(operands, &Parser::expression);
    }

    //Generalizes term/expr rules -> a set s is the accepted tokens to form either term/expr and func is term/expr function
    Node* binary_operation(std::set<string> s, Node* (Parser::*func)()){
        Node* left = (this->*func)();
        Node* right;
        Token op_tok;

        while (s.find((*current_tok).get_type()) != s.end()){
            std::cout << "meow" << '\n';
            op_tok = (*current_tok);
            ++(*this);
            right = (this->*func)();
            Node* node = new BinOpNode(left, right, op_tok);
            left = node;
        }
        
        return left;
    }

    
};