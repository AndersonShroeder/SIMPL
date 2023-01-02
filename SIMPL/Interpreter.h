#include "Parser.h"


class Interpreter{
    Node* ast;

    public:
    Interpreter(){}

    Interpreter(Node* ast_){
        ast = ast_;
    }

    float visit(){
        return (*ast).eval();
    };

};