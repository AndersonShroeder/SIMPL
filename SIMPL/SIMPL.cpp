
#include "ConsCommands.h"
#include <stdlib.h>
#include <chrono>

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

struct Timer
{
    std::chrono::time_point<std::chrono::steady_clock> start, end;
    std::chrono::duration<float> duration;

    Timer()
    {
        start = std::chrono::steady_clock::now();
    }

   ~Timer()
   {
    end = std::chrono::steady_clock::now();
    duration = end - start;

    float ms = duration.count() * 1000;
    std::cout << "Executed in " << ms << "ms" << '\n';
   } 

};


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
        
        Timer timer;

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
            }
        }
    }

    return 0;
}