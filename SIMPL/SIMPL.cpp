#include "run.h"
#include <stdlib.h>

void view_settings(bool& value, string name){
    value = !value;
    if (value){
        std::cout << "=========================" << '\n' << name << " Enabled" << '\n' << "=========================" << '\n';
    }
    else{
        std::cout << "=========================" << '\n' << name << " Disabled" << '\n' << "=========================" << '\n';
    }
}

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
    bool toks = false;
    bool interp = true;
    bool syntax = true;
    VariableTable global = VariableTable();

    while (true){
        std::cout << "SIMPL > ";
        std::getline(std::cin, input);
        
        if (input == "-t"){
            view_settings(toks, "Token View");
            continue;
        }

        if (input == "-i"){
            view_settings(interp, "Interpretor");
            continue;
        }

        if (input == "-s"){
            view_settings(syntax, "AST View");
            continue;
        }

        // Run on file
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
            if (toks){
                for (Token tok: tokens){
                    tok.print();
                }
            }

            if (syntax){
                std::cout << (*ast).str() << '\n';
            }

            if (interp){
                Interpreter interp = Interpreter(ast, &global);
                float out = interp.visit();
                std::cout << out << '\n';
            }
        }
    }

    return 0;
}