#include "Constants.h"
#include "Lexer.h"

class VariableTable{
    std::unordered_map<string, string> symbols = {};
    std::unordered_map<string, string> parent;

    public:
    VariableTable(){}

    string get(string name){
        if (symbols.count(name)){
            return symbols.at(name);
        }
        else if (parent.count(name)){
            return symbols.at(name);
        }
        else{
            std::cout << "MAKE ERROR - variable not in table" << '\n';
            return "";
        }
    }

    void set(string name, string value){
        symbols[name] = value;
    }

    void remove(string name){
        symbols.erase(name);
    }
};

//base node class -> contains virtual string representation method
class Node{
    public:
    Node(){};

    virtual string str(){
        string s = "";
        return s;
    };

    virtual float eval(VariableTable& table){
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

    NumberNode(Token& tok){
        this->tok = tok;
    }
    
    string str(){
        string s = tok.str();
        return s;
    }

    float eval(VariableTable& table){
        return stof(tok.get_value());
    }
};

class UnaryOpNode: public Node{
    Token op_tok;
    Node* node;

    public:
    UnaryOpNode(){}

    UnaryOpNode(Token op_tok, Node* node){
        this->op_tok = op_tok;
        this->node = node;
    }

    string str(){
        string s = op_tok.str() + (*node).str();
        return s;
    }

    float eval(VariableTable& table){
        return 0;
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
    BinOpNode(Node* left, Node* right, Token& op_tok){
        left_node = left;
        right_node = right;
        this->op_tok = op_tok;
    }

    string str(){
        string s = "(" + (*left_node).str() + ", " + op_tok.str() + ", " + (*right_node).str() + ")";
        return s;   
    }

    float eval(VariableTable& table){
        //change to switch/case -> create enum wtih types
        if (op_tok.get_type() == "PLUS"){
            return (*left_node).eval(table) + (*right_node).eval(table);
        }
        else if (op_tok.get_type() == "MINUS"){
            return (*left_node).eval(table) - (*right_node).eval(table);
        }
        else if (op_tok.get_type() == "DIV"){
            return (*left_node).eval(table) / (*right_node).eval(table);
        }
        else if (op_tok.get_type() == "MUL"){
            return (*left_node).eval(table) * (*right_node).eval(table);
        }
        else if (op_tok.get_type() == "EXP"){
            return pow((*left_node).eval(table), (*right_node).eval(table));
        }
        //only works for ints - error catch here
        else if (op_tok.get_type() == "MOD"){
            return int((*left_node).eval(table)) % int((*right_node).eval(table));
        }
        return 0;
    }
};

class VarAssignNode: public Node{
    string name;
    Node* value;

    public:
    VarAssignNode();

    VarAssignNode(string name, Node* value){
        this->name = name;
        this->value = value;
    }

    string str(){
        string s = "(" + name + " = " + (*value).str() + ")";
        return s;
    }

    float eval(VariableTable& table){
        float val = (*value).eval(table);
        table.set(name, std::to_string(val));
        return val;
    }
};

class VarAccessNode: public Node{
    Token tok;

    public:
    VarAccessNode(){}

    VarAccessNode(Token& tok){
        this->tok = tok;
    }

    string str(){
        string s = tok.get_type() + "(" + tok.get_value() + ")";
        return s;
    }

    float eval(VariableTable& table){
        return std::stof(table.get(tok.get_value()));
    }
};