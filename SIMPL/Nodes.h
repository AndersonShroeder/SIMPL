#include "Lexer.h"

class VariableTable{
    public:
    std::unordered_map<string, string> symbols = {};
    VariableTable* parent=NULL;

    VariableTable(){}

    VariableTable(VariableTable* parent)
    {
        this->parent = parent;
    }

    // checks if the variable name is in local scope, if not go up through scopes and check
    string get(string name){
        if (contains(name)){
            return symbols.at(name);
        }

        else if (parent != NULL)
        {
            return parent->get(name);
        }

        std::cout << "MAKE ERROR - variable not in table" << '\n';
        return "";
        
    }

    VariableTable* check_for_var(string name){
        if (contains(name)){
            return this;
        }

        else if (parent != NULL)
        {
            return parent->check_for_var(name);
        }

        return NULL;
        
    }


    bool contains(string name)
    {
        return symbols.count(name);
    }


    void set(string name, string value)
    {
        VariableTable* ptr = check_for_var(name);
        if (ptr)
        {
            ptr->symbols[name] = value;
        }
        else
        {
            symbols[name] = value;
        }
    }

    void remove(string name){
        symbols.erase(name);
    }
};


//base node class -> contains virtual string representation method
class Node{
    public:

    Node(){};

    // virtual ~Node()
    // {
    //     std::cout << "Destruct" << '\n';
    // }

    virtual string str(){
        return "";
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
    std::shared_ptr<Node> node;

    public:
    UnaryOpNode(){}

    UnaryOpNode(Token op_tok, std::shared_ptr<Node> node){
        this->op_tok = op_tok;
        this->node = node;
    }

    string str(){
        string s = op_tok.str() + " " + (*node).str();
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

        else if (op_tok.get_type() == "INCREMENT")
        {
            return increment(table, node);
        }

        return 0;
    }

    float positive(VariableTable& table, std::shared_ptr<Node> node){
        return node->eval(table);
    }

    float negative(VariableTable& table, std::shared_ptr<Node> node){
        return (node->eval(table))*(-1);
    }

    float negate(VariableTable& table, std::shared_ptr<Node> node){
        return !(node->eval(table));
    }

    float increment(VariableTable& table, std::shared_ptr<Node> node)
    {
        float val = (node->eval(table)) + 1;
        return val;
    }
};

//Operation Node for two values
class BinOpNode: public Node{
    std::shared_ptr<Node> left_node;
    Token op_tok;
    std::shared_ptr<Node> right_node;

    public:
    //default constructor
    BinOpNode(){}

    //constructor
    BinOpNode(std::shared_ptr<Node> left, std::shared_ptr<Node> right, Token& op_tok){
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

    float add(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return (*left_node).eval(table) + (*right_node).eval(table);
    }

    float subtract(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return (*left_node).eval(table) - (*right_node).eval(table);
    }

    float divide(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return (*left_node).eval(table) / (*right_node).eval(table);
    }

    float multiply(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return (*left_node).eval(table) * (*right_node).eval(table);
    }

    float exponentiate(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return pow((*left_node).eval(table), (*right_node).eval(table));
    }

    float modulus(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return int((*left_node).eval(table)) % int((*right_node).eval(table));
    }


    //Logical comparisons
    float equals(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return int((*left_node).eval(table)) == int((*right_node).eval(table));
    }

    float notequals(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return int((*left_node).eval(table)) != int((*right_node).eval(table));
    }

    float greaterthan(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return int((*left_node).eval(table)) > int((*right_node).eval(table));
    }

    float greaterthaneq(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return int((*left_node).eval(table)) >= int((*right_node).eval(table));
    }

    float lessthan(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return int((*left_node).eval(table)) < int((*right_node).eval(table));
    }

    float lessthaneq(VariableTable& table, std::shared_ptr<Node> left, std::shared_ptr<Node> right){
        return int((*left_node).eval(table)) <= int((*right_node).eval(table));
    }
};

class VarAssignNode: public Node{
    string name;
    std::shared_ptr<Node> value;

    public:
    VarAssignNode();

    VarAssignNode(string name, std::shared_ptr<Node> value){
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
        return stof(table.get(tok.get_value()));
    }
};

//class to scope expressions
class Scope : public Node
{
    public:

    //contain the expressions to be executed in scope
    std::vector<std::shared_ptr<Node>> expressions;

    //contain scope variables
    VariableTable scope_table;
    
    //No default init because we must have expressions
    Scope(std::vector<std::shared_ptr<Node>> expressions)
    {
        this->expressions = expressions;
    }

    string str()
    {
        string s;
        s += "Scope {\n";

        for (auto i: expressions)
        {
            s += i->str();
            s += "\n";
        }
        s += "}\n";
        return s;
    }

    float eval(VariableTable& table)
    {
        scope_table = VariableTable(&table);
        for (auto i: expressions)
        {
            i->eval(scope_table);
        }

        return 0;
    }
};

class Argument: public Node
{
    //contain the expressions to be executed in scope
    std::vector<std::shared_ptr<Node>> arguments;

    //contain scope variables
    VariableTable scope_table;

    public:

    Argument(std::vector<std::shared_ptr<Node>> arguments)
    {
        this->arguments = arguments;
    }

    string str()
    {
        string s;
        s += "Arguments (";

        for (int i = 0; i < arguments.size(); ++i)
        {
            s += arguments.at(i)->str();
            
            if (i != arguments.size() - 1)
            {
                s += ", ";
            }
        }
        s += ")\n";
        return s;
    }

    std::shared_ptr<Node> get(int idx)
    {
        return arguments.at(idx);
    }
};


class IfNode: public Node{
    std::vector<std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>>> cases;
    std::shared_ptr<Node> else_case;
    VariableTable local_table;

    public:
    IfNode(){};

    IfNode(std::vector<std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>>> cases, std::shared_ptr<Node> else_case){
        this->cases = cases;
        this->else_case = else_case;
    } 

    string str(){
        string s;
        string type = "If ";
        bool seen_if = false;
        for (std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>> case_: cases){
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
        for (std::tuple<std::shared_ptr<Node>, std::shared_ptr<Node>> case_: cases)
        {
            //get argument table to be used as local_scope -> 0 will always be an argument node
            local_table = VariableTable(table);
            if ((*(std::get<0>(case_))).eval(local_table)){
                return (*(std::get<1>(case_))).eval(local_table);
            }
        }
        if (else_case != NULL){
            return (*else_case).eval(table);
        }

        return 0;
    }
};

class ForNode: public Node
{
    std::shared_ptr<Node> var_assign;
    std::shared_ptr<Node> end_value;
    std::shared_ptr<Node> step_value;
    std::shared_ptr<Node> arguments;
    std::shared_ptr<Node> execution;

    public:
    ForNode(){}

    //  The input argument must have a size of three
    ForNode(std::shared_ptr<Argument> arguments, std::shared_ptr<Node> execution)
    {
    this->var_assign = arguments->get(0);
    this->end_value = arguments->get(1);
    this->step_value = arguments->get(2);
    this->execution = execution;
    this->arguments = arguments;
    }

    string str()
    {
        string s = "For " + arguments->str() +  "Do: " + execution->str();
        return s;
    }

    float eval(VariableTable& table)
    {  
        VariableTable local_table = VariableTable(&table);
        var_assign->eval(local_table);
        while ((end_value->eval(local_table)))
        {
            execution->eval(local_table);
            step_value->eval(local_table);
        }

        return 0;
    }

};

class WhileNode: public Node
{
    std::shared_ptr<Node> condition_node;
    std::shared_ptr<Node> body_node;

    public:
    WhileNode(){}

    WhileNode(std::shared_ptr<Node> condition_node, std::shared_ptr<Node> body_node)
    {
        this->condition_node = condition_node;
        this->body_node = body_node;
    }

    string str(){
        string s = "While (" + (*condition_node).str() + ") Do: " + (*body_node).str();
        return s;
    }

    float eval(VariableTable& table)
    {
        while ((*condition_node).eval(table))
        {
            (*body_node).eval(table);
        }

        return 0;
    }

};

class FuncDefNode: public Node
{
    Token var_name_tok;
    std::shared_ptr<Node> args;
    std::shared_ptr<Node> scope;

    public:
    FuncDefNode(){};

    FuncDefNode(Token var_name_tok, std::shared_ptr<Node> args, std::shared_ptr<Node> scope)
    {
        this->var_name_tok = var_name_tok;
        this->args = args;
        this->scope = scope;
    }

    string str()
    {
        string s = "Created Function(" + var_name_tok.get_value() + ")\n" + args->str() + scope->str();
        return s;
    }
};

class CallNode: public Node
{
    std::shared_ptr<Node> node_to_call;
    std::shared_ptr<Node> args;

    public:
    CallNode(){};

    CallNode(std::shared_ptr<Node> node_to_call, std::shared_ptr<Node> args)
    {
        this->node_to_call = node_to_call;
        this->args = args;
    }
};


