#include "Parser.h"
//#include "Variables.h"

class Interpreter
{
    std::shared_ptr<Node> ast;
    VariableTable *table;

public:
    Interpreter() {}

    Interpreter(std::shared_ptr<Node> ast, VariableTable *table)
    {
        this->ast = ast;
        this->table = table;
    }

    float visit()
    {
        return (*ast).eval(*table);
    };
};