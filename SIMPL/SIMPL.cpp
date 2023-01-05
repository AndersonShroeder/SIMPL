
#include "ConsCommands.h"
#include <stdlib.h>
#include <crtdbg.h>



string read_file(){
    std::cout << "Type Filename: ";
    string filename;
    string input;
    std::getline(std::cin, filename);
    std::ifstream file;
    file.open(filename);

    if(file.fail()){
        std::cout << "File Failed To Open" << '\n';
        return "FAIL";
    }

    else{
        std::stringstream ss;
        ss << file.rdbuf();
        input = ss.str();
        return input;
    }
}

int main(){
    string input;
    string filename;
    Commands coms = Commands();
    VariableTable global = VariableTable();

    while (true){
        std::cout << "SIMPL > ";
        std::getline(std::cin, input);
        coms.check_inputs(input);

        if (coms.cont){
            continue;
        }

        if (input == "-readfile"){
            string output = read_file();
            if (output == "FAIL"){
                continue;  
            }
            input = output;
        }
        
        FileData structure = run(input, filename);
        auto tokens = structure.get_tokens();
        auto error = structure.get_error();
        Parser parser = Parser(tokens);
        auto ast = parser.parse();

        if (structure.check_error()){
            std::cout << error << '\n';
        }

        else{
            if (coms.toks){
                for (Token tok: tokens){
                    tok.print();
                }
            }

            if (coms.syntax){
                std::cout << (*ast).str() << '\n';
            }

            if (coms.interp){
                Interpreter interp = Interpreter(ast, &global);
                float out = interp.visit();
                std::cout << out << '\n';
                _CrtDumpMemoryLeaks();
                delete ast;
            }
        }
    }

    return 0;
}