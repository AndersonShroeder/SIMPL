#include "Parser.h"


class Interpreter{
    Node* ast;

    public:
    Interpreter(){}

    Interpreter(Node* ast_){
        ast = ast_;
    }

    int visit(){
        return (*ast).eval();
    };

};