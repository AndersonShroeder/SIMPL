
#include "ConsCommands.h"
#include <stdlib.h>

//TODO

//Syntax enforcement

//Fix Run function

//reading from file
//  1. semi-colons mark end of expression
//  2. 

//Switch Parser/Nodes to stack allocation
//  1. pass a default initialized Node 

//Error Detection
//  1. Some sort of wrapper that automatically checks for correct {}/()
//  2. Log position of error
//  3. Log line that produced error

std::vector<string> read_file(){
    
    string filename;
    std::vector<string> input;
    string line;

    std::cout << "Type Filename: ";
    std::getline(std::cin, filename);

    std::ifstream file(filename);

    if (file.fail()){
        std::cout << "File Failed To Open" << '\n';
        return {};
    }

    else{
        while (std::getline(file, line))
        {
            input.push_back(line);
        }

        return input;
    }
}

int main(){
    string input;
    string filename;
    ConsoleInterface console = ConsoleInterface();
    VariableTable global = VariableTable();

    while (true){
        std::cout << "SIMPL > ";
        std::getline(std::cin, input);
        console.check_inputs(input);

        if (console.cont){
            continue;
        }
        
        FileData structure = run(input, filename);
        std::vector<Token> tokens = structure.get_tokens();
        Parser parser = Parser(tokens);
        auto ast = parser.parse();

        if (structure.check_error()){
            std::cout << structure.get_error() + '\n' + structure.get_position().str() << '\n';
        }

        else{
            if (console.toks){
                for (Token tok: tokens){
                    tok.print();
                }
            }

            if (console.syntax){
                std::cout << (*ast).str() << '\n';
            }

            if (console.interp){
                Interpreter interp = Interpreter(ast, &global);
                float out = interp.visit();
                std::cout << out << '\n';
                delete ast;
            }
        }
    }

    return 0;
}