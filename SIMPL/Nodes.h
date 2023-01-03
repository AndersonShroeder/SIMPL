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
        if (op_tok.get_type() == "PLUS"){
            return positive(table, node);
        }
        else if (op_tok.get_type() == "MINUS"){
            return negative(table, node);
        }
        else if (op_tok.get_type() == "NOT"){
            return negate(table, node);
        }

        return 0;
    }

    float positive(VariableTable& table, Node* node){
        return node->eval(table);
    }

    float negative(VariableTable& table, Node* node){
        return (node->eval(table))*(-1);
    }

    float negate(VariableTable& table, Node* node){
        return !(node->eval(table));
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
            return add(table, left_node, right_node);
        }
        else if (op_tok.get_type() == "MINUS"){
            return subtract(table, left_node, right_node);
        }
        else if (op_tok.get_type() == "DIV"){
            return divide(table, left_node, right_node);
        }
        else if (op_tok.get_type() == "MUL"){
            return multiply(table, left_node, right_node);
        }
        else if (op_tok.get_type() == "EXP"){
            return exponentiate(table, left_node, right_node);
        }
        //only works for ints - error catch here
        else if (op_tok.get_type() == "MOD"){
            return modulus(table, left_node, right_node);
        }

        //Logical Operators
        else if (op_tok.get_type() == "EQUIV"){
            return equals(table, left_node, right_node);
        }
        else if (op_tok.get_type() == "NOTEQ"){
            return notequals(table, left_node, right_node);
        }
        else if (op_tok.get_type() == "GT"){
            return greaterthan(table, left_node, right_node);
        }
        else if (op_tok.get_type() == "GTE"){
            return greaterthaneq(table, left_node, right_node);
        }
        else if (op_tok.get_type() == "LT"){
            return lessthan(table, left_node, right_node);
        }
        else if (op_tok.get_type() == "LTE"){
            return lessthaneq(table, left_node, right_node);
        }
        return 0;
    }

    float add(VariableTable& table, Node* left, Node* right){
        return (*left_node).eval(table) + (*right_node).eval(table);
    }

    float subtract(VariableTable& table, Node* left, Node* right){
        return (*left_node).eval(table) - (*right_node).eval(table);
    }

    float divide(VariableTable& table, Node* left, Node* right){
        return (*left_node).eval(table) / (*right_node).eval(table);
    }

    float multiply(VariableTable& table, Node* left, Node* right){
        return (*left_node).eval(table) * (*right_node).eval(table);
    }

    float exponentiate(VariableTable& table, Node* left, Node* right){
        return pow((*left_node).eval(table), (*right_node).eval(table));
    }

    float modulus(VariableTable& table, Node* left, Node* right){
        return int((*left_node).eval(table)) % int((*right_node).eval(table));
    }


    //Logical comparisons
    float equals(VariableTable& table, Node* left, Node* right){
        return int((*left_node).eval(table)) == int((*right_node).eval(table));
    }

    float notequals(VariableTable& table, Node* left, Node* right){
        return int((*left_node).eval(table)) != int((*right_node).eval(table));
    }

    float greaterthan(VariableTable& table, Node* left, Node* right){
        return int((*left_node).eval(table)) > int((*right_node).eval(table));
    }

    float greaterthaneq(VariableTable& table, Node* left, Node* right){
        return int((*left_node).eval(table)) >= int((*right_node).eval(table));
    }

    float lessthan(VariableTable& table, Node* left, Node* right){
        return int((*left_node).eval(table)) < int((*right_node).eval(table));
    }

    float lessthaneq(VariableTable& table, Node* left, Node* right){
        return int((*left_node).eval(table)) <= int((*right_node).eval(table));
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

class IfNode: public Node{
    std::vector<std::tuple<Node*, Node*>> cases;
    Node* else_case;

    public:
    IfNode(){};

    IfNode(std::vector<std::tuple<Node*, Node*>> cases, Node* else_case){
        this->cases = cases;
        this->else_case = else_case;
    }

    string str(){
        string s;
        string type = "If ";
        bool seen_if = false;
        for (std::tuple<Node*, Node*> case_: cases){
            if (seen_if){
                type = "Elif ";
            }
            s += type + (*(std::get<0>(case_))).str() + " Then " + (*(std::get<1>(case_))).str() + '\n';
            seen_if = true;
        }
        if (else_case != NULL){
            s += "Else " + (*else_case).str();
        }
        return s;
    }

    float eval(VariableTable& table){
        for (std::tuple<Node*, Node*> case_: cases){
            if ((*(std::get<0>(case_))).eval(table)){
                return (*(std::get<1>(case_))).eval(table);
            }
        }
        if (else_case != NULL){
            return (*else_case).eval(table);
        }
    }
};