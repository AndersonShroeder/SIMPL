#include "run.h"
#include <stdlib.h>

int main(){
    string input;
    string filename = "<stdout>";
    bool toks = true;
    bool interp = true;
    VariableTable global = VariableTable();

    while (true){
        std::getline(std::cin, input);
        if (input == "-t"){
            toks = !toks;
            if (toks){
                std::cout << "=========================" << '\n' << "Token View Enabled" << '\n' << "=========================" << '\n';
            }
            else{
                std::cout << "=========================" << '\n' << "Token view Disabled" << '\n' << "=========================" << '\n';
            }
            continue;
        }

        if (input == "-i"){
            interp = !interp;
            if (interp){
                std::cout << '\n' << "=========================" << '\n' << "Interpretor Enabled" << '\n' << "=========================" << '\n';
            }
            else{
                std::cout << '\n' << "=========================" << '\n' << "Interpretor Disabled" << '\n' << "=========================" << '\n';
            }
            continue;
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
            if (toks){
                for (Token tok: tokens){
                    tok.print();
                }
            }

            if (interp){
                std::cout << (*ast).str() << '\n';
                Interpreter interp = Interpreter(ast, &global);
                float out = interp.visit();
                std::cout << out << '\n';
            }
            
     
        }
    }

    return 0;
}