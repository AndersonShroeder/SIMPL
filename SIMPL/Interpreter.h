#include "Parser.h"
#include "Variables.h"


class Interpreter{
    Node* ast;
    VariableTable* table;

    public:
    Interpreter(){}

    Interpreter(Node* ast, VariableTable* table){
        this->ast = ast;
        this->table = table;
    }

    float visit(){
        return (*ast).eval(*table);
    };

};